BEGIN_NS(vt)

#pragma pack(push, 1)
struct GTSHeader
{
    static constexpr uint32_t GRPGMagic = 0x47505247; // 'GRPG'
    static constexpr uint32_t CurrentVersion = 5;

    uint32_t Magic;
    uint32_t Version;
    uint32_t Unused;
    Guid GUID;
    uint32_t NumLayers;
    uint64_t LayersOffset;
    uint32_t NumLevels;
    uint64_t LevelsOffset;
    int32_t TileWidth;
    int32_t TileHeight;
    int32_t TileBorder;

    uint32_t I2; // Some tile count?
    uint32_t NumFlatTileInfos;
    uint64_t FlatTileInfoOffset;
    uint32_t I6;
    uint32_t I7;

    uint32_t NumPackedTileIDs;
    uint64_t PackedTileIDsOffset;

    uint32_t M;
    uint32_t N;
    uint32_t O;
    uint32_t P;
    uint32_t Q;
    uint32_t R;
    uint32_t S;

    uint32_t PageSize;
    uint32_t NumPageFiles;
    uint64_t PageFileMetadataOffset;

    uint32_t FourCCListSize;
    uint64_t FourCCListOffset;

    uint32_t ParameterBlockHeadersCount;
    uint64_t ParameterBlockHeadersOffset;

    uint64_t ThumbnailsOffset;
    uint32_t XJJ;
    uint32_t XKK;
    uint32_t XLL;
    uint32_t XMM;
};


struct GTSTileSetLayer
{
    uint32_t DataType;
    int32_t B; // -1
};


struct GTSTileSetLevel
{
    uint32_t Width; // Width in tiles
    uint32_t Height; // Height in tiles
    uint64_t FlatTileIndicesOffset; // Flat tiles offset in file
};


struct GTSParameterBlockHeader
{
    uint32_t ParameterBlockID;
    uint32_t Codec;
    uint32_t ParameterBlockSize;
    uint64_t FileInfoOffset;
};


struct GTSBCParameterBlock
{
    uint16_t Version;
    char Compression1[16];
    char Compression2[16];

    uint32_t B;
    uint8_t C1;
    uint8_t C2;
    uint8_t BCField3;
    uint8_t DataType;
    uint16_t D;
    uint32_t FourCC;
    uint8_t E1;
    uint8_t SaveMip;
    uint8_t E3;
    uint8_t E4;
    uint32_t F;
};


struct GTSUniformParameterBlock
{
    uint16_t Version;
    uint16_t A_Unused;
    uint32_t Width;
    uint32_t Height;
    uint32_t DataType;
};


struct GTSPageFileInfo
{
    wchar_t FileNameBuf[256];

    uint32_t NumPages;
    Guid Checksum;
    uint32_t F; // 2
};


struct GTSFourCCMetadata
{
    uint32_t FourCC;
    uint8_t Format;
    uint8_t ExtendedLength;
    uint16_t Length;

    inline uint32_t ValueLength() const
    {
        uint32_t valueSize = Length;
        if (ExtendedLength == 1) {
            valueSize |= (*(uint32_t*)(this + 1) << 16);
        }

        return valueSize;
    }

    inline uint8_t const* ValuePtr() const
    {
        auto val = (uint8_t const*)(this + 1);
        if (ExtendedLength == 1) {
            val += 4;
        }

        return val;
    }
};


struct GTSThumbnailInfoHeader
{
    uint32_t NumThumbnails;
    uint32_t A;
    uint32_t B;
};


struct GTSThumbnailInfo
{
    Guid GUID;
    uint64_t OffsetInFile;
    uint32_t CompressedSize;
    uint32_t Unknown1;
    uint16_t Unknown2;
    uint16_t Unknown3;
};

struct GTSPackedTileID
{
    uint32_t Val;

    inline GTSPackedTileID(uint32_t layer, uint32_t level, uint32_t x, uint32_t y)
        : Val((layer & 0xF)
            | ((level & 0xF) << 4)
            | ((y & 0xFFF) << 8)
            | ((x & 0xFFF) << 20))
    {}

    inline uint32_t Layer() const
    {
        return Val & 0x0F;
    }

    inline uint32_t Level() const
    {
        return (Val >> 4) & 0x0F;
    }

    inline uint32_t Y() const
    {
        return (Val >> 8) & 0x0FFF;
    }

    inline uint32_t X() const
    {
        return Val >> 20;
    }
};

struct GTSFlatTileInfo
{
    uint16_t PageFileIndex; // Index of file in PageFileInfos
    uint16_t PageIndex; // Index of 1MB page
    uint16_t ChunkIndex; // Index of entry within page
    uint16_t D; // Always 1?
    uint32_t PackedTileIndex; // Index of tile in PackedTileIDs
};
#pragma pack(pop)

struct FourCCTextureMeta
{
    struct GTSFile* File;
    STDWString Name;
    uint32_t X;
    uint32_t Y;
    uint32_t Width;
    uint32_t Height;
};

struct FourCCNode
{
    GTSFourCCMetadata* Node;
    uint32_t Size;

    FourCCNode FindNext(uint32_t tag);
    FourCCNode Advance();
    FourCCNode Enter(uint32_t tag);
    int32_t ReadInt(uint32_t tag);
    bool ReadBinary(uint32_t tag, void* buf, uint32_t size);
    STDWString ReadString(uint32_t tag);
};

END_NS()
