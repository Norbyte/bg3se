#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Resources.h>
#include <mutex>

BEGIN_SE()

class VirtualTextureHelpers
{
public:
	void Load();

	bool OnTextureLoad(resource::LoadableResource::LoadProc* next, resource::LoadableResource* self, ResourceManager* mgr);
	bool OnTextureUnload(resource::LoadableResource::UnloadProc* next, resource::LoadableResource* self, ResourceManager* mgr);

private:
	MultiHashMap<FixedString, FixedString> gtsPaths_;
	std::mutex lock_;

	int32_t IncRefGTS(VirtualTextureManager* vt, unsigned int textureLayerConfig, std::optional<char> gtsSuffix, bool a4, FixedString const& gTexId);
	void DecRefGTS(VirtualTextureManager* vt, unsigned int textureLayerConfig, std::optional<char> gtsSuffix, bool a4, FixedString const& gTexId);
	STDString GetVirtualTexturePath(unsigned int textureLayerConfig, std::optional<char> gtsSuffix, bool a4, FixedString const& gTexId, bool isLoad);
};

END_SE()
