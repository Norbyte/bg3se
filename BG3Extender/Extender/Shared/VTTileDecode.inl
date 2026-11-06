#include <External/lz4/lib/lz4.h>
#include <External/FastLZ/fastlz.h>

#define BCDEC_STATIC
#define BCDEC_IMPLEMENTATION
#include <External/bcdec/bcdec.h>

BEGIN_NS(vt)

// Decodes a region of a BC3 image into an RGBA8 image; coordinates must be multiples of 4
void DecodeBC3(uint8_t const* bc, uint32_t bcWidth, uint32_t srcX, uint32_t srcY, uint32_t width, uint32_t height, uint8_t* pixels)
{
    for (uint32_t y = 0; y < height; y += 4) {
        for (uint32_t x = 0; x < width; x += 4) {
            auto block = bc + (((srcY + y) / 4) * (bcWidth / 4) + (srcX + x) / 4) * 16;
            bcdec_bc3(block, pixels + (y * width + x) * 4, width * 4);
        }
    }
}

// Reads tiles from a GTP page file. Only the file header is validated upfront; the chunk tables
// of pages are parsed on-demand when a tile is requested.
class GTPFileReader
{
public:
    GTPFileReader(GTSFile* tileSet, FileReaderPin&& reader)
        : tileSet_(tileSet),
        reader_(std::move(reader))
    {}

    bool IsLoaded() const
    {
        return reader_.IsLoaded();
    }

    void ReadHeader()
    {
        data_ = static_cast<uint8_t const*>(reader_.Buf());
        size_ = reader_.Size();
        pageSize_ = tileSet_->Header->PageSize;

        VT_EXPECT(size_ >= sizeof(GTPHeader));
        auto header = reinterpret_cast<GTPHeader const*>(data_);
        VT_EXPECT(header->Magic == GTPHeader::GRAPMagic);
        VT_EXPECT(header->Version == GTPHeader::CurrentVersion);
        numPages_ = (uint32_t)((size_ + pageSize_ - 1) / pageSize_);
    }

    // Reads a BC3 tile (including borders, excluding the embedded mip)
    void ReadTile(uint32_t pageIndex, uint32_t chunkIndex, Array<uint8_t>& tile)
    {
        auto chunk = FindChunk(pageIndex, chunkIndex);
        auto data = reinterpret_cast<uint8_t const*>(chunk + 1);
        auto paramBlock = tileSet_->FindParameterBlock(chunk->ParameterBlockID);
        VT_EXPECT(paramBlock != nullptr);

        tile.resize(BCTileBytes + BCTileMipBytes);

        switch (chunk->Codec) {
        case GTSCodec::Uniform:
        {
            VT_EXPECT(std::holds_alternative<GTSUniformParameterBlock>(*paramBlock) && chunk->Size == 4);
            EncodeUniformBC3Block(data, tile.data());
            for (uint32_t offset = 16; offset < BCTileBytes; offset += 16) {
                memcpy(tile.data() + offset, tile.data(), 16);
            }
            break;
        }

        case GTSCodec::BC:
        {
            VT_EXPECT(std::holds_alternative<GTSBCParameterBlock>(*paramBlock));
            auto const& bc = std::get<GTSBCParameterBlock>(*paramBlock);

            int decompressedSize{ 0 };
            if (strncmp(bc.Compression1, "lz4", 16) == 0) {
                decompressedSize = LZ4_decompress_safe((char const*)data, (char*)tile.data(), (int)chunk->Size, (int)tile.size());
            } else if (strncmp(bc.Compression1, "lz77", 16) == 0) {
                decompressedSize = fastlz_decompress(data, (int)chunk->Size, tile.data(), (int)tile.size());
            } else if (strncmp(bc.Compression1, "raw", 16) == 0) {
                decompressedSize = (int)std::min(chunk->Size, tile.size());
                memcpy(tile.data(), data, decompressedSize);
            } else {
                VT_EXPECT(false && "Unsupported tile compression method");
            }

            VT_EXPECT(decompressedSize >= (int)BCTileBytes);
            break;
        }

        default:
            VT_EXPECT(false && "Unsupported tile codec");
            break;
        }

        tile.resize(BCTileBytes);
    }

private:
    GTSFile* tileSet_;
    FileReaderPin reader_;
    uint8_t const* data_{ nullptr };
    uint64_t size_{ 0 };
    uint32_t pageSize_{ 0 };
    uint32_t numPages_{ 0 };

    GTPChunkHeader const* FindChunk(uint32_t pageIndex, uint32_t chunkIndex)
    {
        VT_EXPECT(pageIndex < numPages_);
        uint64_t pageStart = (uint64_t)pageIndex * pageSize_;
        uint64_t pageSize = std::min<uint64_t>(pageSize_, size_ - pageStart);
        auto page = data_ + pageStart;

        // Chunk offset table is preceded by the file header on the first page
        uint64_t tableOffset = (pageIndex == 0) ? sizeof(GTPHeader) : 0;
        VT_RANGE_CHECK(pageSize, tableOffset, 1, uint32_t);
        auto numChunks = *reinterpret_cast<uint32_t const*>(page + tableOffset);
        VT_EXPECT(chunkIndex < numChunks);
        VT_RANGE_CHECK(pageSize, tableOffset + sizeof(uint32_t), numChunks, uint32_t);
        uint64_t chunkOffset = reinterpret_cast<uint32_t const*>(page + tableOffset + sizeof(uint32_t))[chunkIndex];

        VT_RANGE_CHECK(pageSize, chunkOffset, 1, GTPChunkHeader);
        auto chunk = reinterpret_cast<GTPChunkHeader const*>(page + chunkOffset);
        VT_RANGE_CHECK(pageSize, chunkOffset + sizeof(GTPChunkHeader), chunk->Size, uint8_t);
        return chunk;
    }
};

END_NS()
