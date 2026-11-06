BEGIN_NS(vt)

// Copies a rectangle between RGBA8 images; strides are in pixels
void CopyRect(uint8_t const* src, uint32_t srcStride, uint32_t srcX, uint32_t srcY,
    uint8_t* dst, uint32_t dstStride, uint32_t dstX, uint32_t dstY, uint32_t width, uint32_t height)
{
    for (uint32_t y = 0; y < height; y++) {
        memcpy(dst + ((dstY + y) * dstStride + dstX) * 4, src + ((srcY + y) * srcStride + srcX) * 4, width * 4);
    }
}

// 2x2 box filter; width and height are the source dimensions
void Downsample2x(uint8_t const* src, uint32_t width, uint32_t height, uint8_t* dst)
{
    for (uint32_t y = 0; y < height; y += 2) {
        for (uint32_t x = 0; x < width; x += 2) {
            for (uint32_t c = 0; c < 4; c++) {
                auto p = src + (y * width + x) * 4 + c;
                *dst++ = (uint8_t)((p[0] + p[4] + p[width * 4] + p[width * 4 + 4] + 2) >> 2);
            }
        }
    }
}

// Generates tiles that are missing from the merged tile set (ie. mip levels that are smaller
// than the last level of a source tile set) by downsampling the tiles of the previous level
class GTSMipGenerator
{
public:
    struct GeneratedTile
    {
        uint32_t Level;
        uint32_t X;
        uint32_t Y;
        uint32_t Layer;
        uint16_t PageIndex{ 0 };
        uint16_t ChunkIndex{ 0 };
    };

    GTPFileWriter PageFile;
    Array<GeneratedTile> Tiles;
    // Parameter blocks used by generated tiles that are not present in the tile set
    Array<GTSParameterBlockHeader> ParameterBlocks;
    Array<GTSParameterBlockDesc> ParameterBlockBlobs;

    GTSMipGenerator(GTSStitchedFile& tileSet)
        : PageFile(tileSet.Header.PageSize),
        tileSet_(tileSet)
    {}

    void Generate()
    {
        levelCache_.resize(tileSet_.Levels.size());
        pageFiles_.resize(tileSet_.PageFiles.size());

        for (uint32_t level = 1; level < tileSet_.Levels.size(); level++) {
            GenerateLevel(level);
            // Generating the next level only needs tiles from this level
            levelCache_[level - 1].clear();
        }
    }

private:
    GTSStitchedFile& tileSet_;
    // RGBA contents (excluding borders) of decoded and generated tiles on each level
    Array<HashMap<uint32_t, Array<uint8_t>>> levelCache_;
    Array<GameUniquePtr<GTPFileReader>> pageFiles_;

    void GenerateLevel(uint32_t level)
    {
        auto const& lvl = tileSet_.Levels[level];
        auto const& prevLvl = tileSet_.Levels[level - 1];
        auto const& indices = tileSet_.PerLevelFlatTileIndices[level];
        auto numLayers = tileSet_.Layers.size();
        Array<GeneratedTile> levelTiles;

        // 2x2 tiles of the previous level (excluding borders), ie. a 256x256 px RGBA image
        Array<uint8_t> srcPixels;
        srcPixels.resize((RawTileSize * 2) * (RawTileSize * 2) * 4);

        for (uint32_t y = 0; y < lvl.Height; y++) {
            for (uint32_t x = 0; x < lvl.Width; x++) {
                for (uint32_t layer = 0; layer < numLayers; layer++) {
                    // Flat tile indices are stored row-major with the layers of a tile next to each other.
                    // Only tiles without any data are generated; this excludes tiles flagged with
                    // 0x80000000, as those fall back to a lower mip level of a source tile set.
                    if (indices[layer + (x + y * lvl.Width) * numLayers] != 0xffffffffu) {
                        continue;
                    }

                    // Tile (x, y) covers tiles (2x .. 2x+1, 2y .. 2y+1) of the previous level;
                    // bit 0 of i selects the column, bit 1 selects the row:
                    //   i = 0: top left, 1: top right, 2: bottom left, 3: bottom right
                    uint8_t const* srcTiles[4]{ nullptr };
                    bool hasData{ false };
                    for (uint32_t i = 0; i < 4; i++) {
                        auto srcX = x * 2 + (i & 1);
                        auto srcY = y * 2 + (i >> 1);
                        // Level sizes are clamped to 1 tile, so if the previous level is only
                        // 1 tile wide (or high), the right (or bottom) source tiles don't exist
                        if (srcX < prevLvl.Width && srcY < prevLvl.Height) {
                            srcTiles[i] = GetTile(level - 1, srcX, srcY, layer);
                            hasData = hasData || srcTiles[i] != nullptr;
                        }
                    }

                    // Gaps between the placed tile sets stay empty
                    if (!hasData) {
                        continue;
                    }

                    // Missing source tiles remain transparent black
                    memset(srcPixels.data(), 0, srcPixels.size());
                    for (uint32_t i = 0; i < 4; i++) {
                        if (srcTiles[i] != nullptr) {
                            CopyRect(srcTiles[i], RawTileSize, 0, 0, srcPixels.data(), RawTileSize * 2,
                                (i & 1) * RawTileSize, (i >> 1) * RawTileSize, RawTileSize, RawTileSize);
                        }
                    }

                    // 256x256 -> 128x128 px; borders are added in EncodeTile()
                    Array<uint8_t> pixels;
                    pixels.resize(RawTileSize * RawTileSize * 4);
                    Downsample2x(srcPixels.data(), RawTileSize * 2, RawTileSize * 2, pixels.data());
                    levelCache_[level].set(GTSPackedTileID(layer, level, x, y).Val, std::move(pixels));
                    levelTiles.push_back(GeneratedTile{ level, x, y, layer });
                }
            }
        }

        // Borders need the contents of adjacent tiles, so tiles can only be encoded
        // after all tiles on this level were generated
        for (auto& tile : levelTiles) {
            EncodeTile(tile);
            Tiles.push_back(tile);
        }
    }

    void EncodeTile(GeneratedTile& tile)
    {
        auto const& lvl = tileSet_.Levels[tile.Level];
        // 144x144 px padded tile; areas without an adjacent tile keep a transparent black border
        Array<uint8_t> pixels;
        pixels.resize(PaddedTileSize * PaddedTileSize * 4);

        // Along each axis, the padded tile is assembled from 3 tiles (d = offset of the source tile):
        //   d = -1: last 8 px of the previous tile   [120, 128) -> border       [0, 8)
        //   d =  0: all 128 px of the tile itself    [0, 128)   -> contents     [8, 136)
        //   d =  1: first 8 px of the next tile      [0, 8)     -> border       [136, 144)
        // Combining both axes gives a 3x3 grid; the corner regions come from the diagonal neighbours.
        auto srcOffset = [](int32_t d) { return d < 0 ? RawTileSize - TileBorderSize : 0u; };
        auto dstOffset = [](int32_t d) { return d < 0 ? 0u : (d == 0 ? TileBorderSize : TileBorderSize + RawTileSize); };
        auto extent = [](int32_t d) { return d == 0 ? RawTileSize : TileBorderSize; };

        for (int32_t dy = -1; dy <= 1; dy++) {
            for (int32_t dx = -1; dx <= 1; dx++) {
                auto x = (int32_t)tile.X + dx;
                auto y = (int32_t)tile.Y + dy;
                // Tiles on the edge of the tile set have no neighbours on that side
                if (x < 0 || y < 0 || x >= (int32_t)lvl.Width || y >= (int32_t)lvl.Height) {
                    continue;
                }

                auto src = GetTile(tile.Level, x, y, tile.Layer);
                if (src != nullptr) {
                    CopyRect(src, RawTileSize, srcOffset(dx), srcOffset(dy),
                        pixels.data(), PaddedTileSize, dstOffset(dx), dstOffset(dy), extent(dx), extent(dy));
                }
            }
        }

        // Chunk contents: BC3 encoded 144x144 px tile, followed by its 72x72 px embedded mip
        Array<uint8_t> bc;
        bc.resize(BCTileBytes + BCTileMipBytes);
        EncodeBC3(pixels.data(), PaddedTileSize, PaddedTileSize, bc.data());

        // The embedded mip covers the same area as the tile (including borders) at half resolution,
        // ie. a 64x64 px tile with 4 px borders on the next level
        Array<uint8_t> mipPixels;
        mipPixels.resize(TileMipSize * TileMipSize * 4);
        Downsample2x(pixels.data(), PaddedTileSize, PaddedTileSize, mipPixels.data());
        EncodeBC3(mipPixels.data(), TileMipSize, TileMipSize, bc.data() + BCTileBytes);

        Array<uint8_t> compressed;
        CompressTileLZ4(std::span<uint8_t const>(bc.data(), bc.size()), compressed);

        auto parameterBlockId = GetParameterBlock(tileSet_.Layers[tile.Layer].DataType);
        auto [pageIndex, chunkIndex] = PageFile.AddChunk(GTSCodec::BC, parameterBlockId, std::span<uint8_t const>(compressed.data(), compressed.size()));
        tile.PageIndex = pageIndex;
        tile.ChunkIndex = chunkIndex;
    }

    // Returns the RGBA contents (excluding borders) of a tile, or nullptr if the tile has no data.
    // The returned buffer remains valid until the cache of the level is cleared.
    uint8_t const* GetTile(uint32_t level, uint32_t x, uint32_t y, uint32_t layer)
    {
        auto& cache = levelCache_[level];
        auto key = GTSPackedTileID(layer, level, x, y).Val;
        if (auto pixels = cache.try_get(key)) {
            return pixels->data();
        }

        auto index = tileSet_.PerLevelFlatTileIndices[level][layer + (x + y * tileSet_.Levels[level].Width) * tileSet_.Layers.size()];
        // Missing tile, or fallback to a lower mip level
        if ((index & 0x80000000u) != 0) {
            return nullptr;
        }

        VT_EXPECT(index < tileSet_.FlatTileInfos.size());
        auto const& tileInfo = tileSet_.FlatTileInfos[index];
        auto pageFile = GetPageFile(tileInfo.PageFileIndex);
        if (pageFile == nullptr) {
            return nullptr;
        }

        Array<uint8_t> bc;
        pageFile->ReadTile(tileInfo.PageIndex, tileInfo.ChunkIndex, bc);

        Array<uint8_t> pixels;
        pixels.resize(RawTileSize * RawTileSize * 4);
        DecodeBC3(bc.data(), PaddedTileSize, TileBorderSize, TileBorderSize, RawTileSize, RawTileSize, pixels.data());
        return cache.set(key, std::move(pixels))->data();
    }

    GTPFileReader* GetPageFile(uint32_t index)
    {
        VT_EXPECT(index < pageFiles_.size());
        auto& pageFile = pageFiles_[index];
        if (!pageFile) {
            auto path = ToUTF8(tileSet_.PageFiles[index].FileNameBuf);
            pageFile = MakeUnique<GTPFileReader>(tileSet_.PageFileOwners[index], GetStaticSymbols().MakeFileReader(path, PathRootType::Data));
            if (pageFile->IsLoaded()) {
                pageFile->ReadHeader();
            } else {
                WARN("Unable to open page file '%s'; its tiles will be skipped during mip generation", path.c_str());
            }
        }

        return pageFile->IsLoaded() ? pageFile.get() : nullptr;
    }

    // Finds or adds an LZ4 compressed BC parameter block for the specified layer data type
    uint32_t GetParameterBlock(uint32_t dataType)
    {
        uint32_t nextId{ 1 };
        auto find = [&](Array<GTSParameterBlockHeader> const& headers, Array<GTSParameterBlockDesc> const& blobs) -> std::optional<uint32_t> {
            for (uint32_t i = 0; i < headers.size(); i++) {
                auto bc = std::get_if<GTSBCParameterBlock>(&blobs[i]);
                if (bc != nullptr && IsLZ4ParameterBlock(*bc, dataType)) {
                    return headers[i].ParameterBlockID;
                }

                nextId = std::max(nextId, headers[i].ParameterBlockID + 1);
            }

            return {};
        };

        if (auto id = find(tileSet_.ParameterBlocks, tileSet_.ParameterBlockBlobs)) {
            return *id;
        }

        if (auto id = find(ParameterBlocks, ParameterBlockBlobs)) {
            return *id;
        }

        ParameterBlocks.push_back(GTSParameterBlockHeader{
            .ParameterBlockID = nextId,
            .Codec = GTSCodec::BC,
            .ParameterBlockSize = sizeof(GTSBCParameterBlock),
            .FileInfoOffset = 0
        });
        ParameterBlockBlobs.push_back(MakeLZ4ParameterBlock(dataType));
        return nextId;
    }
};

void GTSStitchedFile::BuildMips()
{
    GTSMipGenerator generator(*this);
    try {
        generator.Generate();
    } catch (std::runtime_error& e) {
        ERR("(#%d) Failed to generate missing mip levels: %s", Index, e.what());
        return;
    }

    // All mip levels are available in the source tile sets
    if (generator.Tiles.empty()) {
        return;
    }

    STDString baseName = OutputPath.substr(0, OutputPath.find_last_of('.'));
    baseName += "_Mips";

    STDString pageFilePath;
    std::ofstream f;
    if (!OpenOutputFile(baseName, ".gtp", pageFilePath, f) || !generator.PageFile.Write(f)) {
        ERR("(#%d) Unable to write mip page file '%s'!", Index, pageFilePath.c_str());
        return;
    }

    // Page file is written next to the GTS, so its path is relative to the GTS directory
    GTSPageFileInfo pageFile;
    memset(&pageFile, 0, sizeof(pageFile));
    wcscpy_s(pageFile.FileNameBuf, FromUTF8(pageFilePath).c_str());
    pageFile.NumPages = generator.PageFile.NumPages();
    pageFile.Checksum = generator.PageFile.Checksum;
    pageFile.F = 2;

    auto pageFileIndex = (uint16_t)PageFiles.size();
    PageFiles.push_back(pageFile);
    PageFileOwners.push_back(nullptr);

    for (uint32_t i = 0; i < generator.ParameterBlocks.size(); i++) {
        ParameterBlocks.push_back(generator.ParameterBlocks[i]);
        ParameterBlockBlobs.push_back(generator.ParameterBlockBlobs[i]);
    }

    for (auto const& tile : generator.Tiles) {
        PerLevelFlatTileIndices[tile.Level][tile.Layer + (tile.X + tile.Y * Levels[tile.Level].Width) * Layers.size()] = FlatTileInfos.size();
        FlatTileInfos.push_back(GTSFlatTileInfo{
            .PageFileIndex = pageFileIndex,
            .PageIndex = tile.PageIndex,
            .ChunkIndex = tile.ChunkIndex,
            .D = 1,
            .PackedTileIndex = PackedTileIDs.size()
        });
        PackedTileIDs.push_back(GTSPackedTileID(tile.Layer, tile.Level, tile.X, tile.Y));
    }

    DEBUG("(#%d) Generated %d tiles for missing mip levels: %s", Index, generator.Tiles.size(), pageFilePath.c_str());
}

END_NS()
