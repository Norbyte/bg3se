#include "json/json.h"
#include <Extender/Shared/VirtualTextureMerge.inl>

BEGIN_SE()

void VirtualTextureHelpers::Load()
{
	std::lock_guard _(lock_);

	RebuildIfNecessary();

	auto pendingRemaps = gtsPaths_;

	auto banks = GetStaticSymbols().GetCurrentResourceBank();
	auto vtManager = (*GetStaticSymbols().ls__gGlobalResourceManager)->VirtualTextureManager;
	auto bank = banks->Container.Banks[(unsigned)ResourceBankType::VirtualTexture];
	resource::VirtualTextureResource* firstTex{ nullptr };

	for (auto const& res : bank->Resources) {
		auto tex = static_cast<resource::VirtualTextureResource*>(res.Value);
		firstTex = tex;
		auto remap = pendingRemaps.try_get(tex->GTexFileName);
		if (remap) {
			auto gtsGuid = gtsToGuid_.try_get(*remap);
			if (gtsGuid == nullptr) {
				auto newGuid = Guid::Generate();
				gtsGuid = gtsToGuid_.set(*remap, newGuid);
				auto path = *GetStaticSymbols().ls__PathRoots[(unsigned)PathRootType::Data];
				path += "/";
				path += remap->GetStringView();
				vtManager->TileSets.set(newGuid, path);
			}

			tex->TileSetFileName = FixedString(gtsGuid->ToString());
			pendingRemaps.remove(tex->GTexFileName);
		}
	}

	if (firstTex != nullptr) {
		firstTex->Load(*GetStaticSymbols().ls__gGlobalResourceManager);
	}

	for (auto const& remap : pendingRemaps) {
		WARN("No resource found for virtual texture with GTexName '%s', can't remap", remap.Key().GetString());
	}
}


void VirtualTextureHelpers::RebuildIfNecessary()
{
	auto remaps = CollectRemaps();

	std::unordered_set<FixedString> gtsFiles;
	for (auto const& path : remaps) {
		gtsFiles.insert(path.Value());
	}

	// Check if we need to rebuild, or the merged set already matches the requested one
	if (built_ && !NeedsRebuild(gtsFiles)) {
		DEBUG("Merged tileset is already up to date, skipping build");
		return;
	}

	gtsPaths_ = std::move(remaps);
	sourceTileSets_ = gtsFiles;
	built_ = false;

	if (gtsPaths_.size() > 0) {
		DEBUG("Loaded %d virtual texture mappings", gtsPaths_.size());
	}

	if (sourceTileSets_.size() > 1) {
		built_ = Stitch();
		if (!built_) {
			gtsPaths_.clear();
			sourceTileSets_.clear();
		}
	}
}

bool VirtualTextureHelpers::NeedsRebuild(std::unordered_set<FixedString> const& newTileSets)
{
	if (newTileSets.size() != sourceTileSets_.size()) {
		return true;
	}

	for (auto const& path : newTileSets) {
		if (sourceTileSets_.find(path) == sourceTileSets_.end()) {
			return true;
		}
	}

	return false;
}

HashMap<FixedString, FixedString> VirtualTextureHelpers::CollectRemaps()
{
	HashMap<FixedString, FixedString> remaps;

	auto modManager = GetStaticSymbols().GetModManagerClient();
	for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
		auto virtualTextureConfig = "Mods/" + mod.Info.Directory + "/ScriptExtender/VirtualTextures.json";
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
								remaps.set(FixedString(gTex), FixedString(gts));
							}
						}
					}
				}
			}
		}
	}

	return remaps;
}

bool VirtualTextureHelpers::Stitch()
{
	if (sourceTileSets_.size() < 2) {
		// No need to stitch if we only have 1 tile set
		ERR("Trying to stitch VT with only a single tile set loaded?");
		return false;
	}

	DEBUG("Creating merged virtual texture tile set");

	vt::GTSStitchedFile stitched;
	for (auto const& path : sourceTileSets_) {
		auto reader = GetStaticSymbols().MakeFileReader(path, PathRootType::Data);
		if (reader.IsLoaded()) {
			auto gts = GameAlloc<vt::GTSFile>();
			gts->Path = path;
			gts->Buf.resize((uint32_t)reader.Size());
			std::copy(reinterpret_cast<uint8_t*>(reader.Buf()), reinterpret_cast<uint8_t*>(reader.Buf()) + reader.Size(), gts->Buf.begin());
			char const* reason{ nullptr };
			if (!gts->Read(reason)) {
				ERR("Failed to load '%s': %s", path.GetString(), reason ? reason : "");
			} else {
				stitched.TileSets.push_back(gts);
			}
		}
	}

	vt::MergedTileSetGeometryCalculator geom;
	geom.TileSets = stitched.TileSets;
	if (!geom.DoAutoPlacement()) {
		ERR("Failed to calculate merged tileset geometry, virtual textures will not be available!");
		return false;
	}

	DEBUG("Merged geometry: %d x %d tiles (%d x %d px)",
		geom.TotalWidth, geom.TotalHeight,
		geom.TotalWidth * 128, geom.TotalHeight * 128
	);

	stitched.Init(geom.TotalWidth, geom.TotalHeight);
	if (stitched.Build()) {
		DEBUG("Built merged GTS: %s", stitched.OutputPath.c_str());

		FixedString outputPath{ stitched.OutputPath };
		for (auto& path : gtsPaths_) {
			path.Value() = outputPath;
		}

		return true;
	} else {
		ERR("Merged tile set build failed, virtual textures will not be available!");
		return false;
	}
}

#if defined(__APPLE__)
void MacOSVirtualTextureHelpers::Load()
{
	std::lock_guard _(lock_);

	RebuildIfNecessary();

	auto pendingRemaps = gtsPaths_;

	auto banks = GetStaticSymbols().GetCurrentResourceBank();
	auto vtManager = (*GetStaticSymbols().ls__gGlobalResourceManager)->VirtualTextureManager;
	auto bank = banks->Container.Banks[(unsigned)ResourceBankType::VirtualTexture];
	resource::VirtualTextureResource* firstTex{ nullptr };

	for (auto const& res : bank->Resources) {
		auto tex = static_cast<resource::VirtualTextureResource*>(res.Value);
		firstTex = tex;
		auto remap = pendingRemaps.try_get(tex->GTexFileName);
		if (remap) {
			auto gtsGuid = gtsToGuid_.try_get(*remap);
			if (gtsGuid == nullptr) {
				auto newGuid = Guid::Generate();
				gtsGuid = gtsToGuid_.set(*remap, newGuid);
				auto path = *GetStaticSymbols().ls__PathRoots[(unsigned)PathRootType::Data];
				path += "/";
				path += remap->GetStringView();
				vtManager->TileSets.set(newGuid, path);
			}

			tex->TileSetFileName = FixedString(gtsGuid->ToString());
			pendingRemaps.remove(tex->GTexFileName);
		}
	}

	if (firstTex != nullptr) {
		firstTex->Load(*GetStaticSymbols().ls__gGlobalResourceManager);
	}

	for (auto const& remap : pendingRemaps) {
		WARN("No resource found for virtual texture with GTexName '%s', can't remap", remap.Key().GetString());
	}
}

void MacOSVirtualTextureHelpers::RebuildIfNecessary()
{
	auto remaps = CollectRemaps();

	std::unordered_set<FixedString> gtsFiles;
	for (auto const& path : remaps) {
		gtsFiles.insert(path.Value());
	}

	// Check if we need to rebuild, or the merged set already matches the requested one
	if (built_ && !NeedsRebuild(gtsFiles)) {
		DEBUG("Merged tileset is already up to date, skipping build");
		return;
	}

	gtsPaths_ = std::move(remaps);
	sourceTileSets_ = gtsFiles;
	built_ = false;

	if (gtsPaths_.size() > 0) {
		DEBUG("Loaded %d virtual texture mappings", gtsPaths_.size());
	}

	if (sourceTileSets_.size() > 1) {
		built_ = Stitch();
		if (!built_) {
			gtsPaths_.clear();
			sourceTileSets_.clear();
		}
	}
}

bool MacOSVirtualTextureHelpers::NeedsRebuild(std::unordered_set<FixedString> const& newTileSets)
{
	if (newTileSets.size() != sourceTileSets_.size()) {
		return true;
	}

	for (auto const& path : newTileSets) {
		if (sourceTileSets_.find(path) == sourceTileSets_.end()) {
			return true;
		}
	}

	return false;
}

HashMap<FixedString, FixedString> MacOSVirtualTextureHelpers::CollectRemaps()
{
	HashMap<FixedString, FixedString> remaps;

	auto modManager = GetStaticSymbols().GetModManagerClient();
	for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
		auto virtualTextureConfig = "Mods/" + mod.Info.Directory + "/ScriptExtender/VirtualTextures.json";
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
								remaps.set(FixedString(gTex), FixedString(gts));
							}
						}
					}
				}
			}
		}
	}

	return remaps;
}

bool MacOSVirtualTextureHelpers::Stitch()
{
	if (sourceTileSets_.size() < 2) {
		// No need to stitch if we only have 1 tile set
		ERR("Trying to stitch VT with only a single tile set loaded?");
		return false;
	}

	DEBUG("Creating merged virtual texture tile set");

	vt::GTSStitchedFile stitched;
	for (auto const& path : sourceTileSets_) {
		auto reader = GetStaticSymbols().MakeFileReader(path, PathRootType::Data);
		if (reader.IsLoaded()) {
			auto gts = GameAlloc<vt::GTSFile>();
			gts->Path = path;
			gts->Buf.resize((uint32_t)reader.Size());
			std::copy(reinterpret_cast<uint8_t*>(reader.Buf()), reinterpret_cast<uint8_t*>(reader.Buf()) + reader.Size(), gts->Buf.begin());
			char const* reason{ nullptr };
			if (!gts->Read(reason)) {
				ERR("Failed to load '%s': %s", path.GetString(), reason ? reason : "");
			} else {
				stitched.TileSets.push_back(gts);
			}
		}
	}

	vt::MergedTileSetGeometryCalculator geom;
	geom.TileSets = stitched.TileSets;
	if (!geom.DoAutoPlacement()) {
		ERR("Failed to calculate merged tileset geometry, virtual textures will not be available!");
		return false;
	}

	DEBUG("Merged geometry: %d x %d tiles (%d x %d px)",
		geom.TotalWidth, geom.TotalHeight,
		geom.TotalWidth * 128, geom.TotalHeight * 128
	);

	stitched.Init(geom.TotalWidth, geom.TotalHeight);
	if (stitched.Build()) {
		DEBUG("Built merged GTS: %s", stitched.OutputPath.c_str());

		FixedString outputPath{ stitched.OutputPath };
		for (auto& path : gtsPaths_) {
			path.Value() = outputPath;
		}

		return true;
	} else {
		ERR("Merged tile set build failed, virtual textures will not be available!");
		return false;
	}
}
#endif

END_SE()
