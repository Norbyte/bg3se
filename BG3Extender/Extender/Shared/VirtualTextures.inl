#include "json/json.h"

BEGIN_SE()

STDString VirtualTextureHelpers::GetVirtualTexturePath(unsigned int textureLayerConfig, std::optional<char> gtsSuffix, bool a4, 
	FixedString const& gTexId, bool isLoad)
{
	{
		std::lock_guard _(lock_);
		auto gtsMapping = gtsPaths_.find(gTexId);
		if (gtsMapping != gtsPaths_.end()) {
			auto path = *GetStaticSymbols().ls__PathRoots[(unsigned)PathRootType::Data]
				+ "/" + gtsMapping.Value().GetString();
			if (isLoad) {
				DEBUG("Loading GTex '%s' from %s", gTexId.GetString(), gtsMapping.Value().GetString());
			}

			return path;
		}
	}

	auto path = *GetStaticSymbols().ls__PathRoots[(unsigned)PathRootType::Data]
		+ "/Generated/Public/VirtualTextures/Albedo_Normal_Physical";

	if (gtsSuffix) {
		path += '_';
		path += *gtsSuffix;
	}

	path += ".gts";
	return path;
}

int32_t VirtualTextureHelpers::IncRefGTS(VirtualTextureManager* vt, unsigned int textureLayerConfig, std::optional<char> gtsSuffix, bool a4, FixedString const& gTexId)
{
	SRWLockPin _(&vt->GTSLock);

	auto path = GetVirtualTexturePath(textureLayerConfig, gtsSuffix, a4, gTexId, true);

	auto gts = vt->LoadedGTS.find(path);
	if (gts != vt->LoadedGTS.end()) {
		gts.Value().RefCount++;
		return gts.Value().GTSHandle;
	}

	auto textureConfig = vt->LayerConfigs.find(TextureLayerConfigId(textureLayerConfig, (uint8_t)a4));
	if (textureConfig != vt->LayerConfigs.end()) {
		auto gtsHandle = vt->LoadGTS(textureLayerConfig, path, -1, textureConfig.Value());
		if (gtsHandle != -1) {
			VirtualTextureManager::GTSInfo gtsInfo{ gtsHandle, 2 };
			vt->LoadedGTS.insert(path, gtsInfo);
			return gtsHandle;
		}
	}

	ERR("Unable to load tile set '%s'!", path.c_str());

	return -1;
}

void VirtualTextureHelpers::DecRefGTS(VirtualTextureManager* vt, unsigned int textureLayerConfig, std::optional<char> gtsSuffix, bool a4, FixedString const& gTexId)
{
	SRWLockPin _(&vt->GTSLock);

	auto path = GetVirtualTexturePath(textureLayerConfig, gtsSuffix, a4, gTexId, false);

	auto gts = vt->LoadedGTS.find(path);
	if (gts == vt->LoadedGTS.end()) {
		return;
	}

	if (gts.Value().RefCount-- <= 1) {
		vt->ReleaseGTS(textureLayerConfig, gts.Value().GTSHandle, true, true);
		vt->LoadedGTS.erase(gts);
	}
}

void VirtualTextureHelpers::Load()
{
	std::lock_guard _(lock_);

	gtsPaths_.clear();

	auto modManager = GetStaticSymbols().GetModManagerClient();
	for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
		auto dir = mod.Info.Directory;
		auto virtualTextureConfig = "Mods/" + dir + "/ScriptExtender/VirtualTextures.json";
		auto reader = GetStaticSymbols().MakeFileReader(virtualTextureConfig);

		if (reader.IsLoaded()) {
			DEBUG("Loading virtual texture mappings: %s", virtualTextureConfig.c_str());

			Json::CharReaderBuilder factory;
			auto jsonReader = std::unique_ptr<Json::CharReader>(factory.newCharReader());

			Json::Value root;
			std::string errs;
			auto configText = reader.ToString();
			if (!jsonReader->parse(configText.c_str(), configText.c_str() + configText.size(), &root, &errs)) {
				OsiError("Unable to parse virtual texture configuration for mod '" << mod.Info.Name << "': " << errs);
			} else {
				auto mappings = root["Mappings"];
				if (mappings.isArray()) {
					for (auto const& mapping : mappings) {
						if (mapping.isObject() && mapping["GTexName"].isString() && mapping["GTS"].isString()) {
							auto gTex = mapping["GTexName"].asString();
							auto gts = mapping["GTS"].asString();
							if (!gTex.empty() && !gts.empty()) {
								gtsPaths_.Set(FixedString(gTex), FixedString(gts));
							}
						}
					}
				}
			}
		}
	}
	if (gtsPaths_.size() > 0) {
		DEBUG("Loaded %d virtual texture mappings", gtsPaths_.size());
	}
}

bool VirtualTextureHelpers::OnTextureLoad(resource::LoadableResource::LoadProc* next, resource::LoadableResource* self, ResourceManager* mgr)
{
	auto res = static_cast<resource::VirtualTextureResource*>(self);
	auto vt = mgr->VirtualTextureManager;

	std::optional<char> gtsSuffix;
	if (vt->UseChunkedVirtualTextures) {
		gtsSuffix = res->GTexFileName.GetString()[0];
	}

	res->GTSHandle = IncRefGTS(vt, res->VirtualTextureLayerConfig, gtsSuffix, res->field_52, res->GTexFileName);

	if (res->GTSHandle == -1) {
		// Load a dummy GTS to ensure the game doesn't crash
		FixedString placeholderTex{ "00fb1fe11e94bc9fad61170f5551017f" };
		res->GTSHandle = IncRefGTS(vt, res->VirtualTextureLayerConfig, '0', res->field_52, placeholderTex);
		res->GTexFileName = placeholderTex;
	} else if (res->Prefetch && res->PrefetchMipLevel >= 0) {
		res->GraphineTextureData = vt->LoadTexture(res->GTSHandle, res->GTexFileName, 1);
	}

	res->LoadedVirtualTextureLayerConfig = res->VirtualTextureLayerConfig;
	res->LoadedField52 = res->field_52;
	res->GTSSuffix = gtsSuffix;
	return res->GTSHandle != -1;
}

bool VirtualTextureHelpers::OnTextureUnload(resource::LoadableResource::UnloadProc* next, resource::LoadableResource* self, ResourceManager* mgr)
{
	auto res = static_cast<resource::VirtualTextureResource*>(self);
	auto vt = mgr->VirtualTextureManager;

	if (res->GraphineTextureData != nullptr) {
		vt->UnloadTexture(res->GTSHandle, res->GraphineTextureData);
		res->GraphineTextureData = nullptr;
	}

	DecRefGTS(vt, res->LoadedVirtualTextureLayerConfig, res->GTSSuffix, res->LoadedField52, res->GTexFileName);
	res->GTSHandle = -1;
	return true;
}

#if defined(VT_DEBUG_TRANSCODE)
int64_t VirtualTextureHelpers::OnTranscode(resource::LoadableResource::TranscodeProc* next, void* self, void* transcodeData, void* source, __int64 sourceSize, int width, int height, void* pSaveMip, void* paramBlock, int a9, void* destination, int expectedBCFormat)
{
	auto res = next(self, transcodeData, source, sourceSize, width, height, pSaveMip, paramBlock, a9, destination, expectedBCFormat);
	auto tc = (BCTranscodeData*)transcodeData;
	auto pb = (GTSBCParameterBlock*)paramBlock;
	auto sm = (SaveMipData*)pSaveMip;
	if (res != 0) {
		DEBUG("Transcode fail: %d; DataType: %d, BC: %d, SM: %d", res, pb->DataType, tc->BCFormat, sm->SaveMip);
	}
	return res;
}
#endif

END_SE()
