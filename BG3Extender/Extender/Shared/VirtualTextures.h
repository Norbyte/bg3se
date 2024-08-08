#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Resources.h>
#include <mutex>

BEGIN_SE()

#if defined(VT_DEBUG_TRANSCODE)
#pragma pack(push, 1)
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
#pragma pack(pop)

struct BCTranscodeData
{
	__int16 Version;
	GTSBCParameterBlock* ParameterBlock;
	char NeedsDecompression;
	__int64 DecompressFunc;
	int field_20;
	char BCField3;
	int BCFormat;
	BYTE SaveMip;
	void* DecodeBuffer;
	char field_38;
	__int64 field_40;
	__int64 field_48;
};

struct SaveMipData
{
	char SaveMip;
	__int64 SourceBuffer;
	int SourceSize;
	int Width;
	int Height;
};
#endif

class VirtualTextureHelpers
{
public:
	void Load();

	bool OnTextureLoad(resource::LoadableResource::LoadProc* next, resource::LoadableResource* self, ResourceManager* mgr);
	bool OnTextureUnload(resource::LoadableResource::UnloadProc* next, resource::LoadableResource* self, ResourceManager* mgr);
#if defined(VT_DEBUG_TRANSCODE)
	int64_t OnTranscode(resource::LoadableResource::TranscodeProc* next, void* self, void* transcodeData, void* source, __int64 sourceSize, int width, int height, void* pSaveMip, void* paramBlock, int a9, void* destination, int expectedBCFormat);
#endif

private:
	HashMap<FixedString, FixedString> gtsPaths_;
	std::unordered_set<FixedString> sourceTileSets_;
	bool built_{ false };
	std::mutex lock_;

	int32_t IncRefGTS(VirtualTextureManager* vt, unsigned int textureLayerConfig, std::optional<char> gtsSuffix, bool a4, FixedString const& gTexId);
	void DecRefGTS(VirtualTextureManager* vt, unsigned int textureLayerConfig, std::optional<char> gtsSuffix, bool a4, FixedString const& gTexId);
	STDString GetVirtualTexturePath(unsigned int textureLayerConfig, std::optional<char> gtsSuffix, bool a4, FixedString const& gTexId, bool isLoad);
	bool Stitch();
	HashMap<FixedString, FixedString> CollectRemaps();
	bool NeedsRebuild(std::unordered_set<FixedString> const& newTileSets);
};

END_SE()
