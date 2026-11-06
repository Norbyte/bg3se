#include <External/lz4/lib/lz4.h>

#define STB_DXT_STATIC
#define STB_DXT_IMPLEMENTATION
#include <External/stb/stb_dxt.h>

BEGIN_NS(vt)

// Encodes an RGBA8 image into BC3 blocks; width and height must be multiples of 4
void EncodeBC3(uint8_t const* pixels, uint32_t width, uint32_t height, uint8_t* bc)
{
    uint8_t block[16 * 4];
    for (uint32_t y = 0; y < height; y += 4) {
        for (uint32_t x = 0; x < width; x += 4) {
            for (uint32_t row = 0; row < 4; row++) {
                memcpy(block + row * 16, pixels + ((y + row) * width + x) * 4, 16);
            }

            stb_compress_dxt_block(bc, block, 1, STB_DXT_NORMAL);
            bc += 16;
        }
    }
}

// Encodes a BC3 block where all pixels have the specified RGBA8 color
void EncodeUniformBC3Block(uint8_t const* color, uint8_t* bc)
{
    uint8_t block[16 * 4];
    for (uint32_t i = 0; i < 16; i++) {
        memcpy(block + i * 4, color, 4);
    }

    stb_compress_dxt_block(bc, block, 1, STB_DXT_NORMAL);
}

// Compresses an encoded tile using the LZ4 block format
void CompressTileLZ4(std::span<uint8_t const> tile, Array<uint8_t>& compressed)
{
    compressed.resize((uint32_t)LZ4_compressBound((int)tile.size()));
    auto compressedSize = LZ4_compress_default((char const*)tile.data(), (char*)compressed.data(), (int)tile.size(), (int)compressed.size());
    VT_EXPECT(compressedSize > 0);
    compressed.resize(compressedSize);
}

// Parameter block describing BC tiles compressed with CompressTileLZ4()
GTSBCParameterBlock MakeLZ4ParameterBlock(uint32_t dataType)
{
    GTSBCParameterBlock bc;
    memset(&bc, 0, sizeof(bc));
    bc.Version = 0x238e;
    strcpy_s(bc.Compression1, "lz4");
    strcpy_s(bc.Compression2, "lz40.1.0");
    bc.DataType = (uint8_t)dataType;
    bc.FourCC = 0x20334342u;
    bc.SaveMip = 1;
    return bc;
}

bool IsLZ4ParameterBlock(GTSBCParameterBlock const& bc, uint32_t dataType)
{
    return bc.DataType == dataType
        && bc.SaveMip == 1
        && strncmp(bc.Compression1, "lz4", 16) == 0
        && strncmp(bc.Compression2, "lz40.1.0", 16) == 0;
}

// Simplified GTP writer for storing tiles generated at runtime.
// Chunks are always appended to the last page; there is no backfilling or deduplication.
class GTPFileWriter
{
public:
    Guid Checksum;

    GTPFileWriter(uint32_t pageSize)
        : Checksum(Guid::Generate()),
        pageSize_(pageSize)
    {}

    uint32_t NumPages() const
    {
        return pages_.size();
    }

    // Returns the page and chunk index of the added chunk
    std::pair<uint16_t, uint16_t> AddChunk(GTSCodec codec, uint32_t parameterBlockId, std::span<uint8_t const> data)
    {
        // Chunk offset table entry + chunk header + payload
        auto chunkSize = (uint32_t)(sizeof(uint32_t) + sizeof(GTPChunkHeader) + data.size());
        if (pages_.empty() || pages_[pages_.size() - 1].Budget + chunkSize > pageSize_) {
            auto& newPage = pages_.push_back(Page{});
            // Chunk count, preceded by the file header on the first page
            newPage.Budget = (uint32_t)(sizeof(uint32_t) + (pages_.size() == 1 ? sizeof(GTPHeader) : 0));
            newPage.Chunks.reserve(pageSize_);
            VT_EXPECT(newPage.Budget + chunkSize <= pageSize_);
        }

        auto& page = pages_[pages_.size() - 1];
        GTPChunkHeader header{ codec, parameterBlockId, (uint32_t)data.size() };
        auto offset = page.Chunks.size();
        page.ChunkOffsets.push_back(offset);
        page.Chunks.resize(offset + (uint32_t)(sizeof(header) + data.size()));
        memcpy(page.Chunks.data() + offset, &header, sizeof(header));
        memcpy(page.Chunks.data() + offset + sizeof(header), data.data(), data.size());
        page.Budget += chunkSize;

        return { (uint16_t)(pages_.size() - 1), (uint16_t)(page.ChunkOffsets.size() - 1) };
    }

    bool Write(std::ofstream& f)
    {
        Array<uint8_t> buf;
        buf.resize(pageSize_);

        for (uint32_t i = 0; i < pages_.size(); i++) {
            auto const& page = pages_[i];
            memset(buf.data(), 0, pageSize_);

            uint32_t offset = 0;
            if (i == 0) {
                GTPHeader header{ GTPHeader::GRAPMagic, GTPHeader::CurrentVersion, Checksum };
                memcpy(buf.data(), &header, sizeof(header));
                offset += (uint32_t)sizeof(header);
            }

            uint32_t numChunks = page.ChunkOffsets.size();
            memcpy(buf.data() + offset, &numChunks, sizeof(uint32_t));
            offset += (uint32_t)sizeof(uint32_t);

            // Chunk offsets are relative to the start of the page
            uint32_t chunksOffset = offset + numChunks * (uint32_t)sizeof(uint32_t);
            for (auto chunkOffset : page.ChunkOffsets) {
                uint32_t pageOffset = chunksOffset + chunkOffset;
                memcpy(buf.data() + offset, &pageOffset, sizeof(uint32_t));
                offset += sizeof(uint32_t);
            }

            memcpy(buf.data() + chunksOffset, page.Chunks.data(), page.Chunks.size());
            f.write((char const*)buf.data(), pageSize_);
        }

        f.flush();
        return f.good();
    }

private:
    struct Page
    {
        Array<uint8_t> Chunks;
        Array<uint32_t> ChunkOffsets;
        uint32_t Budget{ 0 };
    };

    uint32_t pageSize_;
    Array<Page> pages_;
};

END_NS()
