#include <GameDefinitions/VirtualTextureFormat.h>

BEGIN_NS(vt)

FourCCNode FourCCNode::FindNext(uint32_t tag)
{
	auto cc = (uint8_t*)Node;
	auto end = cc + Size;
	while (cc < end) {
		auto meta = (GTSFourCCMetadata*)cc;
		if (meta->FourCC == _byteswap_ulong(tag)) {
			return FourCCNode(meta, (uint32_t)(end - cc));
		}

		cc += sizeof(GTSFourCCMetadata);
		uint32_t valueSize = meta->ValueLength();
		if (meta->ExtendedLength == 1) {
			cc += 4;
		}

		cc += valueSize;

		if (((std::uintptr_t)cc % 4) != 0)
		{
			cc += 4 - ((std::uintptr_t)cc % 4);
		}
	}

	return FourCCNode(nullptr, 0);
}

FourCCNode FourCCNode::Advance()
{
	if (Size == 0) return FourCCNode(nullptr, 0);

	auto cc = (uint8_t*)Node;
	auto end = cc + Size;

	cc += sizeof(GTSFourCCMetadata);
	uint32_t valueSize = Node->ValueLength();
	if (Node->ExtendedLength == 1) {
		cc += 4;
	}

	cc += valueSize;

	if (((std::uintptr_t)cc % 4) != 0)
	{
		cc += 4 - ((std::uintptr_t)cc % 4);
	}

	if (cc < end) {
		return FourCCNode((GTSFourCCMetadata*)cc, (uint32_t)(end - cc));
	} else {
		return FourCCNode(nullptr, 0);
	}
}

FourCCNode FourCCNode::Enter(uint32_t tag)
{
	auto parent = FindNext(tag);
	if (parent.Node == nullptr || parent.Node->Format != 1) return FourCCNode(nullptr, 0);

	return FourCCNode((GTSFourCCMetadata*)parent.Node->ValuePtr(), parent.Node->ValueLength());
}

int32_t FourCCNode::ReadInt(uint32_t tag)
{
	auto val = FindNext(tag);
	if (val.Node == nullptr || val.Node->Format != 3) return 0;

	return *(int32_t const*)val.Node->ValuePtr();
}

bool FourCCNode::ReadBinary(uint32_t tag, void* buf, uint32_t size)
{
	auto val = FindNext(tag);
	if (val.Node == nullptr || (val.Node->Format != 8 && val.Node->Format != 0x0D)) return 0;

	auto ccVal = val.Node->ValuePtr();
	auto ccSize = val.Node->ValueLength();
	if (ccSize == size) {
		memcpy(buf, ccVal, size);
		return true;
	} else {
		return false;
	}
}

STDWString FourCCNode::ReadString(uint32_t tag)
{
	auto val = FindNext(tag);
	if (val.Node == nullptr || val.Node->Format != 2) return {};

	auto ccVal = val.Node->ValuePtr();
	auto ccSize = val.Node->ValueLength();
	return STDWString((wchar_t const*)ccVal, ccSize / 2 - 1);
}

struct GTSFile
{
	FixedString Path;
	Array<uint8_t> Buf;

	GTSHeader* Header;
	std::span<GTSTileSetLayer> Layers;
	std::span<GTSTileSetLevel> Levels;
	Array<std::span<uint32_t>> PerLevelFlatTileIndices;
	std::span<GTSParameterBlockHeader> ParameterBlocks;
	Array<GTSBCParameterBlock*> ParameterBlockBlobs;
	FourCCNode FourCC;
	std::span<GTSPageFileInfo> PageFiles;
	std::span<GTSPackedTileID> PackedTileIDs;
	std::span<GTSFlatTileInfo> FlatTileInfos;
	Array<FourCCTextureMeta> Textures;

    uint32_t MergedX{ 0 };
	uint32_t MergedY{ 0 };
	uint32_t PageFileOffset{ 0 };

	bool ReadHeader(char const*& reason)
	{
		auto buf = Buf.raw_buf();
		Header = reinterpret_cast<GTSHeader*>(buf);
		if (Header->Magic != GTSHeader::GRPGMagic || Header->CurrentVersion != GTSHeader::CurrentVersion) {
			reason = "Incorrect GTS magic number or version";
			return false;
		}

		if (Header->TileWidth != 0x90 || Header->TileHeight != 0x90 || Header->TileBorder != 8) {
			reason = "Tiles must be 128x128 with max anisotropy of 8";
			return false;
		}

		if (Header->TileWidth != 0x90 || Header->TileHeight != 0x90 || Header->TileBorder != 8) {
			reason = "Tiles must be 128x128 with max anisotropy of 8";
			return false;
		}

		if (Header->PageSize != 1024 * 1024) {
			reason = "Only page size of 1MB is supported";
			return false;
		}

		if (Header->NumLayers != 3) {
			reason = "Tile set must have 3 layers";
			return false;
		}

		if (Header->NumLevels == 0) {
			reason = "Tile set must have at least one mip level";
			return false;
		}

		if (Header->NumLevels > 12) {
			reason = "At most 12 mip levels are supported";
			return false;
		}

		return true;
	}

	bool ReadMetadata(char const*& reason)
	{
		auto buf = Buf.raw_buf();
		Layers = std::span<GTSTileSetLayer>(reinterpret_cast<GTSTileSetLayer*>(buf + Header->LayersOffset), Header->NumLayers);
		Levels = std::span<GTSTileSetLevel>(reinterpret_cast<GTSTileSetLevel*>(buf + Header->LevelsOffset), Header->NumLevels);
		PerLevelFlatTileIndices.resize(Header->NumLevels);

		if (Levels[0].Width > 0x1000 || Levels[0].Height > 0x1000) {
			reason = "Tile set cannot be larger than 4096x4096 tiles";
			return false;
		}

		for (uint32_t i = 0; i < Header->NumLevels; i++)
		{
			auto off = Levels[i].FlatTileIndicesOffset;
			auto sz = Levels[i].Height * Levels[i].Width * Header->NumLayers;
			PerLevelFlatTileIndices[i] = std::span<uint32_t>(reinterpret_cast<uint32_t*>(buf + off), sz);

			for (auto index : PerLevelFlatTileIndices[i]) {
				if ((index & 0x80000000u) == 0 && index >= Header->NumFlatTileInfos) {
					reason = "Invalid index in per-level flat tile index buffer";
					return false;
				}
			}
		}

		ParameterBlocks = std::span<GTSParameterBlockHeader>(reinterpret_cast<GTSParameterBlockHeader*>(buf + Header->ParameterBlockHeadersOffset), Header->ParameterBlockHeadersCount);

		ParameterBlockBlobs.resize((uint32_t)ParameterBlocks.size());
		for (uint32_t i = 0; i < Header->ParameterBlockHeadersCount; i++)
		{
			if (ParameterBlocks[i].Codec != 9) {
				reason = "Parameter block with non-BC codec found";
				return false;
			}

			if (ParameterBlocks[i].ParameterBlockSize != sizeof(GTSBCParameterBlock)) {
				reason = "Invalid BC parameter block size";
				return false;
			}

			ParameterBlockBlobs[i] = reinterpret_cast<GTSBCParameterBlock*>(buf + ParameterBlocks[i].FileInfoOffset);
		}

		return true;
	}

	bool ReadTiles(char const*& reason)
	{
		auto buf = Buf.raw_buf();
		PageFiles = std::span<GTSPageFileInfo>(reinterpret_cast<GTSPageFileInfo*>(buf + Header->PageFileMetadataOffset), Header->NumPageFiles);
		PackedTileIDs = std::span<GTSPackedTileID>(reinterpret_cast<GTSPackedTileID*>(buf + Header->PackedTileIDsOffset), Header->NumPackedTileIDs);
		FlatTileInfos = std::span<GTSFlatTileInfo>(reinterpret_cast<GTSFlatTileInfo*>(buf + Header->FlatTileInfoOffset), Header->NumFlatTileInfos);

		for (auto const& tileInfo : FlatTileInfos) {
			if (tileInfo.PackedTileIndex >= Header->NumPackedTileIDs) {
				reason = "Invalid packed tile ID in flat tile info buffer";
				return false;
			}

			if (tileInfo.PageFileIndex >= Header->NumPageFiles) {
				reason = "Invalid page file reference in flat tile info buffer";
				return false;
			}
		}

		for (auto const& packedTile : PackedTileIDs) {
			if (packedTile.Layer() >= Header->NumLayers
				|| packedTile.Level() >= Header->NumLevels
				|| packedTile.X() >= Levels[packedTile.Level()].Width
				|| packedTile.Y() >= Levels[packedTile.Level()].Height) {
				reason = "Invalid tile reference in packed tile buffer";
				return false;
			}
		}

		return true;
	}

	bool ReadFourCC(char const*& reason)
	{
		auto buf = Buf.raw_buf();
		FourCC = FourCCNode(reinterpret_cast<GTSFourCCMetadata*>(buf + Header->FourCCListOffset), Header->FourCCListSize);

		auto meta = FourCC.Enter('META');
		auto atls = meta.Enter('ATLS');
		auto txts = atls.Enter('TXTS');

		if (txts.Node == nullptr) {
			reason = "Texture FourCC metadata missing";
			return false;
		}

		while (txts.Size > 0) {
			auto txtr = txts.Enter('TXTR');

			FourCCTextureMeta meta;
			meta.File = this;
			meta.Name = txtr.ReadString('NAME');
			meta.Width = txtr.ReadInt('WDTH');
			meta.Height = txtr.ReadInt('HGHT');
			meta.X = txtr.ReadInt('XXXX');
			meta.Y = txtr.ReadInt('YYYY');
			Textures.push_back(meta);

			txts = txts.Advance();
		}

		return true;
	}

	bool Read(char const*& reason)
	{
		if (!ReadHeader(reason)) return false;
		if (!ReadMetadata(reason)) return false;
		if (!ReadFourCC(reason)) return false;
		if (!ReadTiles(reason)) return false;

		return true;
	}
};


class MergedTileSetGeometryCalculator
{
public:
    Array<GTSFile*> TileSets;

	uint32_t PlacementTileWidth{ 1 };
	uint32_t PlacementTileHeight{ 1 };
	uint32_t PlacementGridWidth{ 0 };
	uint32_t PlacementGridHeight{ 0 };
    Array<GTSFile*> PlacementGrid;

	uint32_t TotalWidth{ 0 };
	uint32_t TotalHeight{ 0 };
	uint32_t NextPageFileOffset{ 0 };

    void ResizePlacementGrid(uint32_t w, uint32_t h)
    {
        PlacementGridWidth = w;
        PlacementGridHeight = h;
        PlacementGrid.resize(w * h);
		std::fill(PlacementGrid.begin(), PlacementGrid.end(), nullptr);
		NextPageFileOffset = 0;
    }

    bool GrowPlacementGrid()
    {
		auto w = PlacementGridWidth * PlacementTileWidth;
		auto h = PlacementGridHeight * PlacementTileHeight;

		if (w >= 0x1000 && h >= 0x1000) {
			ERR("Placement grid exceeds maximal GTS size: %d x %d!", w, h);
			return false;
		}

        if (w <= h) {
            ResizePlacementGrid(PlacementGridWidth * 2, PlacementGridHeight);
        } else {
            ResizePlacementGrid(PlacementGridWidth, PlacementGridHeight * 2);
        }

		return true;
    }

    bool TryToPlaceTileSet(GTSFile* tileSet, uint32_t tileSetX, uint32_t tileSetY)
    {
        auto width = tileSet->Levels[0].Width / PlacementTileWidth;
        auto height = tileSet->Levels[0].Height / PlacementTileHeight;

        for (auto y = tileSetY; y < tileSetY + height; y++) {
            for (auto x = tileSetX; x < tileSetX + width; x++) {
                if (PlacementGrid[x + y * PlacementGridWidth] != nullptr) {
                    return false;
                }
            }
        }

        tileSet->MergedX = tileSetX * PlacementTileWidth;
        tileSet->MergedY = tileSetY * PlacementTileHeight;

		tileSet->PageFileOffset = NextPageFileOffset;
		NextPageFileOffset += (uint32_t)tileSet->PageFiles.size();

        for (auto y = tileSetY; y < tileSetY + height; y++) {
            for (auto x = tileSetX; x < tileSetX + width; x++) {
                PlacementGrid[x + y * PlacementGridWidth] = tileSet;
            }
        }

        return true;
    }

    bool TryToPlaceTileSet(GTSFile* tileSet)
    {
        auto width = tileSet->Levels[0].Width / PlacementTileWidth;
        auto height = tileSet->Levels[0].Height / PlacementTileHeight;

        for (uint32_t y = 0; y < PlacementGridHeight - height + 1; y++) {
            for (uint32_t x = 0; x < PlacementGridWidth - width + 1; x++) {
                if (TryToPlaceTileSet(tileSet, x, y)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool PlaceAllTileSets()
    {
        for (auto tileSet : TileSets) {
            if (!TryToPlaceTileSet(tileSet)) {
                return false;
            }
        }

        return true;
    }

    bool DoAutoPlacement()
    {
        uint32_t startingX = 0;
        uint32_t startingY = 0;

        for (auto tileSet : TileSets) {
            PlacementTileWidth = std::min(PlacementTileWidth, std::max(1u, tileSet->Levels[0].Width));
            PlacementTileHeight = std::min(PlacementTileHeight, std::max(1u, tileSet->Levels[0].Height));
            startingX = std::max(startingX, tileSet->Levels[0].Width);
            startingY = std::max(startingY, tileSet->Levels[0].Height);
        }

        ResizePlacementGrid(
			std::max(1u, startingX / PlacementTileWidth), 
			std::max(1u, startingY / PlacementTileHeight)
		);

        while (!PlaceAllTileSets())
        {
			if (!GrowPlacementGrid()) {
				return false;
			}
        }

        TotalWidth = PlacementTileWidth * PlacementGridWidth;
        TotalHeight = PlacementTileHeight * PlacementGridHeight;
		return true;
    }
};

struct FourCCWriter
{
	Array<uint8_t> Buf;
	Array<uint32_t> Stack;
	uint32_t Offset{ 0 };

	template <class T>
	T* Advance()
	{
		RequestBytes(sizeof(T));
		auto ptr = (T*)(Buf.raw_buf() + Offset);
		Offset += sizeof(T);
		return ptr;
	}

	void RequestBytes(uint32_t bytes)
	{
		if (Buf.size() - Offset < bytes) {
			Buf.resize(Buf.size() + bytes + 0x1000);
		}
	}

	void BeginElement(uint32_t cc, uint8_t format)
	{
		Stack.push_back(Offset);
		auto meta = Advance<GTSFourCCMetadata>();
		meta->FourCC = _byteswap_ulong(cc);
		meta->Format = format;
		meta->ExtendedLength = 1;
		meta->Length = 0;
		*Advance<uint32_t>() = 0;
	}

	void BeginNode(uint32_t cc)
	{
		BeginElement(cc, 1);
	}

	void WriteElement(uint32_t cc, uint8_t format, void const* val, uint32_t size)
	{
		auto meta = Advance<GTSFourCCMetadata>();
		meta->FourCC = _byteswap_ulong(cc);
		meta->Format = format;
		meta->ExtendedLength = 0;
		meta->Length = (uint16_t)size;

		RequestBytes(size);
		memcpy(Buf.raw_buf() + Offset, val, size);
		Offset += size;

		while ((Offset % 4) != 0)
		{
			*Advance<uint8_t>() = 0;
		}
	}

	void Write(uint32_t cc, uint32_t val)
	{
		WriteElement(cc, 3, &val, sizeof(uint32_t));
	}

	void Write(uint32_t cc, STDWString val)
	{
		WriteElement(cc, 2, val.c_str(), (val.size() + 1) * 2);
	}

	void EndNode()
	{
		auto off = Stack.pop_last();
		auto meta = (GTSFourCCMetadata*)(Buf.raw_buf() + off);
		auto len = Offset - off - sizeof(GTSFourCCMetadata) - sizeof(uint32_t);
		meta->Length = (uint16_t)len;
		auto len2 = (uint32_t*)(Buf.raw_buf() + off + sizeof(GTSFourCCMetadata));
		*len2 = ((uint32_t)len >> 16);
	}
};

struct GTSStitchedFile
{
	Array<GTSFile*> TileSets;
	STDString OutputPath;

	GTSHeader Header;
	Array<GTSTileSetLayer> Layers;
	Array<GTSTileSetLevel> Levels;
	Array<Array<uint32_t>> PerLevelFlatTileIndices;
	Array<GTSParameterBlockHeader> ParameterBlocks;
	Array<GTSBCParameterBlock> ParameterBlockBlobs;
	Array<GTSPageFileInfo> PageFiles;
	Array<GTSPackedTileID> PackedTileIDs;
	Array<GTSFlatTileInfo> FlatTileInfos;
	Array<FourCCTextureMeta> Textures;
	FourCCWriter FourCC;

	uint32_t CurFlatTileOffset{ 0 };
	uint32_t CurPackedTileOffset{ 0 };

	void Init(uint32_t width, uint32_t height)
	{
		memset(&Header, 0, sizeof(Header));

		// The GTS reader always expects a mip count matching this calculation, 
		// even if the NumLevels in the header is different.
		uint32_t levels = 0;
		auto dim = std::min(width, height);
		while (dim) {
			levels++;
			dim >>= 1;
		}

		Layers.resize(3);
		Levels.resize(levels);

		auto levelWidth = width;
		auto levelHeight = height;
		for (uint32_t i = 0; i < levels; i++)
		{
			auto& level = Levels[i];
			level.Width = levelWidth;
			level.Height = levelHeight;

			levelWidth = std::max(1u, levelWidth >> 1);
			levelHeight = std::max(1u, levelHeight >> 1);
		}

		PerLevelFlatTileIndices.resize(levels);
		for (uint32_t i = 0; i < levels; i++)
		{
			auto& level = Levels[i];
			PerLevelFlatTileIndices[i].resize(level.Width * level.Height * Layers.size());
			// Fill with "no data" indices to handle gaps where we didnt place any tile sets
			std::fill(PerLevelFlatTileIndices[i].begin(), PerLevelFlatTileIndices[i].end(), 0xffffffffu);
		}

		for (auto tileSet : TileSets) {
			DEBUG("Adding source GTS: %s (%d x %d tiles)", tileSet->Path.GetString(), tileSet->Levels[0].Width, tileSet->Levels[0].Height);
			AddTileSet(tileSet);
		}
	}

	void StitchTileSetIndices(GTSTileSetLevel& srcLevel, GTSTileSetLevel& dstLevel,
		std::span<uint32_t>& srcIndices, Array<uint32_t>& dstIndices,
		uint32_t offsetX, uint32_t offsetY, uint32_t offsetFlatTile)
	{
		for (uint32_t y = 0; y < srcLevel.Height; y++) {
			for (uint32_t x = 0; x < srcLevel.Width; x++) {
				for (uint32_t layer = 0; layer < Layers.size(); layer++) {
					auto tile = srcIndices[layer + x * Layers.size() + y * srcLevel.Width * Layers.size()];

					if (tile != 0xffffffffu) {
						// Shift flat tile index by the number of flat tiles in preceding tile sets
						tile += offsetFlatTile;
					}

					dstIndices[layer + (x + offsetX) * Layers.size() + (y + offsetY) * dstLevel.Width * Layers.size()] = tile;
				}
			}
		}
	}

	void AddTileSet(GTSFile* tileSet)
	{
		STDWString gtsDir;
		auto gtsPath = tileSet->Path.GetStringView();
		auto sep = gtsPath.find_last_of('/');
		if (sep != StringView::npos) {
			gtsDir = FromUTF8(gtsPath.substr(0, sep + 1));
		}

		for (uint32_t i = 0; i < Layers.size(); i++) {
			Layers[i] = tileSet->Layers[i];
		}

		for (uint32_t i = 0; i < std::min(Levels.size(), (uint32_t)tileSet->Levels.size()); i++) {
			StitchTileSetIndices(
				tileSet->Levels[i],
				Levels[i],
				tileSet->PerLevelFlatTileIndices[i],
				PerLevelFlatTileIndices[i],
				tileSet->MergedX >> i,
				tileSet->MergedY >> i,
				CurFlatTileOffset
			);
		}

		for (uint32_t i = 0; i < tileSet->ParameterBlocks.size(); i++) {
			bool found{ false };
			for (uint32_t j = 0; i < ParameterBlocks.size(); j++) {
				if (ParameterBlocks[j].ParameterBlockID == tileSet->ParameterBlocks[i].ParameterBlockID) {
					found = true;
					break;
				}
			}

			if (!found) {
				ParameterBlocks.push_back(tileSet->ParameterBlocks[i]);
				ParameterBlockBlobs.push_back(*tileSet->ParameterBlockBlobs[i]);
			}
		}

		for (auto const& pf : tileSet->PageFiles) {
			GTSPageFileInfo mergedPf = pf;
			STDWString pfPath = mergedPf.FileNameBuf;
			pfPath = gtsDir + pfPath;
			wcscpy_s(mergedPf.FileNameBuf, pfPath.c_str());
			PageFiles.push_back(mergedPf);
		}

		for (auto const& tile : tileSet->PackedTileIDs) {
			GTSPackedTileID mergedTile(
				tile.Layer(),
				tile.Level(),
				tile.X() + (tileSet->MergedX >> tile.Level()),
				tile.Y() + (tileSet->MergedY >> tile.Level())
			);

			PackedTileIDs.push_back(mergedTile);
		}

		for (auto const& tile : tileSet->FlatTileInfos) {
			GTSFlatTileInfo mergedTile = tile;
			mergedTile.PageFileIndex += tileSet->PageFileOffset;
			mergedTile.PackedTileIndex += CurPackedTileOffset;
			FlatTileInfos.push_back(mergedTile);
		}

		for (auto const& tex : tileSet->Textures) {
			FourCCTextureMeta mergedTex = tex;
			mergedTex.X += tex.File->MergedX * (tileSet->Header->TileWidth - 2*tileSet->Header->TileBorder);
			mergedTex.Y += tex.File->MergedY * (tileSet->Header->TileHeight - 2 * tileSet->Header->TileBorder);
			Textures.push_back(mergedTex);
		}

		CurFlatTileOffset += (uint32_t)tileSet->FlatTileInfos.size();
		CurPackedTileOffset += (uint32_t)tileSet->PackedTileIDs.size();
	}

	void BuildFourCC()
	{
		FourCC.BeginNode('META');

		FourCC.BeginNode('ATLS');
		FourCC.BeginNode('TXTS');

		uint8_t srgb[] = {
			0x01, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00
		};

		uint8_t thmb[] = {
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		};

		wchar_t const* LayerNames[3] = {
			L"Albedo", L"Normal", L"Physical"
		};

		for (auto const& tex : Textures) {
			FourCC.BeginNode('TXTR');
			FourCC.Write('NAME', tex.Name);
			FourCC.Write('WDTH', tex.Width);
			FourCC.Write('HGHT', tex.Height);
			FourCC.Write('XXXX', tex.X);
			FourCC.Write('YYYY', tex.Y);
			FourCC.Write('ADDR', L"None");
			FourCC.WriteElement('SRGB', 8, srgb, sizeof(srgb));
			FourCC.WriteElement('THMB', 0xD, thmb, sizeof(thmb));
			FourCC.EndNode(); // TXTR
		}

		FourCC.EndNode(); // TXTS
		FourCC.EndNode(); // ATLS

		FourCC.Write('PROJ', L"");

		FourCC.BeginNode('LINF');
		for (uint32_t i = 0; i < Layers.size(); i++) {
			FourCC.BeginNode('LAYR');
			FourCC.Write('INDX', i);
			FourCC.Write('TYPE', L"BC3");
			FourCC.Write('NAME', LayerNames[i]);
			FourCC.EndNode(); // LAYR
		}
		FourCC.EndNode(); // LINF

		FourCC.BeginNode('INFO');

		FourCC.BeginNode('COMP');
		
		FourCC.BeginNode('CMPW');
		FourCC.Write('MAJR', 5);
		FourCC.Write('MINR', 0);
		FourCC.EndNode(); // CMPW
		
		FourCC.BeginNode('BLDV');
		FourCC.Write('MAJR', 5);
		FourCC.Write('MINR', 1);
		FourCC.Write('BINF', L"ScriptExtender");
		FourCC.EndNode(); // BLDV
		
		FourCC.EndNode(); // COMP

		FourCC.Write('DATE', L"02-08-2023 07:49:30.7662814 PM +02:00");
		FourCC.Write('BLKS', L"4096");
		FourCC.Write('TILE', L"Software");
		FourCC.Write('BDPR', L"default");
		FourCC.Write('LTMP', 0);

		FourCC.EndNode(); // INFO

		FourCC.EndNode(); // META
	}

	bool Build()
	{
		BuildFourCC();

		Header.Magic = GTSHeader::GRPGMagic;
		Header.Version = GTSHeader::CurrentVersion;
		Header.GUID = TileSets[0]->Header->GUID;
		Header.NumLayers = Layers.size();
		Header.NumLevels = Levels.size();
		Header.TileWidth = TileSets[0]->Header->TileWidth;
		Header.TileHeight = TileSets[0]->Header->TileHeight;
		Header.TileBorder = TileSets[0]->Header->TileBorder;
		Header.NumFlatTileInfos = FlatTileInfos.size();
		Header.NumPackedTileIDs = PackedTileIDs.size();
		Header.PageSize = TileSets[0]->Header->PageSize;
		Header.NumPageFiles = PageFiles.size();
		Header.FourCCListSize = FourCC.Offset;
		Header.ParameterBlockHeadersCount = ParameterBlocks.size();
		Header.ThumbnailsOffset = 0;

		OutputPath = "SEMergedTileSet.gts";
		auto gtsPath = GetStaticSymbols().ToPath(OutputPath, PathRootType::Data, true);
		std::ofstream f(gtsPath.c_str(), std::ios::out | std::ios::binary);

		if (!f.good()) {
			f.close();

			OutputPath = "SEMergedTileSet_";
			OutputPath += std::to_string(GetCurrentProcessId());
			OutputPath += ".gts";
			gtsPath = GetStaticSymbols().ToPath(OutputPath, PathRootType::Data, true);
			f.open(gtsPath.c_str(), std::ios::out | std::ios::binary);

			if (!f.good()) {
				ERR("Unable to write merged tileset file '%s'!", OutputPath.c_str());
				return false;
			}
		}

		f.write((char const*)&Header, sizeof(Header));

		Header.LayersOffset = (uint32_t)f.tellp();
		f.write((char const*)Layers.raw_buf(), sizeof(GTSTileSetLayer) * Layers.size());

		for (uint32_t i = 0; i < Levels.size(); i++) {
			Levels[i].FlatTileIndicesOffset = (uint32_t)f.tellp();
			f.write((char const*)PerLevelFlatTileIndices[i].raw_buf(), sizeof(uint32_t) * PerLevelFlatTileIndices[i].size());
		}

		Header.LevelsOffset = (uint32_t)f.tellp();
		f.write((char const*)Levels.raw_buf(), sizeof(GTSTileSetLevel) * Levels.size());

		for (uint32_t i = 0; i < ParameterBlocks.size(); i++) {
			ParameterBlocks[i].FileInfoOffset = (uint32_t)f.tellp();
			f.write((char const*)&ParameterBlockBlobs[i], sizeof(GTSBCParameterBlock));
		}

		Header.ParameterBlockHeadersOffset = (uint32_t)f.tellp();
		f.write((char const*)ParameterBlocks.raw_buf(), sizeof(GTSParameterBlockHeader) * ParameterBlocks.size());

		Header.PageFileMetadataOffset = (uint32_t)f.tellp();
		f.write((char const*)PageFiles.raw_buf(), sizeof(GTSPageFileInfo) * PageFiles.size());

		Header.FourCCListOffset = (uint32_t)f.tellp();
		f.write((char const*)FourCC.Buf.raw_buf(), FourCC.Offset);

		Header.ThumbnailsOffset = (uint32_t)f.tellp();
		GTSThumbnailInfoHeader thumb;
		memset(&thumb, 0, sizeof(thumb));
		f.write((char const*)&thumb, sizeof(GTSThumbnailInfoHeader));

		Header.PackedTileIDsOffset = (uint32_t)f.tellp();
		f.write((char const*)PackedTileIDs.raw_buf(), sizeof(GTSPackedTileID) * PackedTileIDs.size());

		Header.FlatTileInfoOffset = (uint32_t)f.tellp();
		f.write((char const*)FlatTileInfos.raw_buf(), sizeof(GTSFlatTileInfo) * FlatTileInfos.size());

		f.seekp(0, std::ios::beg);
		f.write((char const*)&Header, sizeof(Header));

		return true;
	}
};

END_NS()
