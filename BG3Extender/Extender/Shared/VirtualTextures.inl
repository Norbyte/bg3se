#include <CoreLib/JsonLibs.h>
#include <Extender/Shared/VirtualTextureMerge.inl>

BEGIN_SE()

std::optional<STDString> MergeGTS(Array<STDString> const& relativePaths)
{
    DEBUG("Creating merged virtual texture tile set");

    vt::GTSStitchedFile stitched;
    for (auto const& path : relativePaths) {
        auto reader = GetStaticSymbols().MakeFileReader(path, PathRootType::Data);
        if (reader.IsLoaded()) {
            auto gts = GameAlloc<vt::GTSFile>();
            auto absPath = GetStaticSymbols().ToPath(path, PathRootType::Data);
            gts->DataPath = path;
            gts->AbsolutePath = absPath;
            gts->Buf.resize((uint32_t)reader.Size());
            std::copy(static_cast<uint8_t*>(reader.Buf()), static_cast<uint8_t*>(reader.Buf()) + reader.Size(), gts->Buf.begin());
            char const* reason{ nullptr };
            try {
                gts->Read();
                stitched.TileSets.push_back(gts);
            } catch (std::runtime_error& e) {
                ERR("Failed to load '%s': %s", path.c_str(), e.what());
            }
        } else {
            ERR("Referenced VT tileset does not exist: %s", path.c_str());
        }
    }

    // We need at least one loaded tileset to proceed with stitching
    if (stitched.TileSets.empty()) {
        ERR("Unable to merge tileset, no source tilesets loaded");
        return {};
    }

    vt::MergedTileSetGeometryCalculator geom;
    geom.TileSets = stitched.TileSets;
    if (!geom.DoAutoPlacement()) {
        ERR("Failed to calculate merged tileset geometry, virtual textures will not be available!");
        return {};
    }

    DEBUG("Merged geometry: %d x %d tiles (%d x %d px)",
        geom.TotalWidth, geom.TotalHeight,
        geom.TotalWidth * 128, geom.TotalHeight * 128
    );

    stitched.Init(geom.TotalWidth, geom.TotalHeight);
    if (stitched.Build()) {
        DEBUG("Built merged GTS: %s", stitched.OutputPath.c_str());
        return stitched.OutputPath;
    } else {
        ERR("Merged tile set build failed, virtual textures will not be available!");
        return {};
    }
}


void VirtualTextureHelpers::Load()
{
    std::lock_guard _(lock_);

    MergeModVTs();
    RebuildIfNecessary();
    BindSEVirtualTextures();
}


void VirtualTextureHelpers::BindSEVirtualTextures()
{
    auto pendingRemaps = gtsPaths_;

    auto banks = GetStaticSymbols().GetCurrentResourceBank();
    auto vtManager = (*GetStaticSymbols().ls__gGlobalResourceManager)->VirtualTextureManager;
    auto bank = banks->Container.Banks[(unsigned)ResourceBankType::VirtualTexture];
    resource::VirtualTextureResource* firstTex{ nullptr };

    for (auto const& res : bank->Resources) {
        auto tex = static_cast<resource::VirtualTextureResource*>(res.Value);
        auto remap = pendingRemaps.try_get(tex->GTexFileName);
        if (remap) {
            firstTex = tex;
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
        Array<STDString> sourceGts;
        for (auto const& path : sourceTileSets_) {
            sourceGts.push_back(STDString(path.GetStringView()));
        }

        auto outputPath = MergeGTS(sourceGts);
        if (outputPath) {
            FixedString outputPathFS{ *outputPath };
            for (auto& path : gtsPaths_) {
                path.Value() = outputPathFS;
            }

            built_ = true;
        } else {
            gtsPaths_.clear();
            sourceTileSets_.clear();
        }
    }
}

// NOTE --------------------------------------
// This is currently WIP, as tile sets get preloaded before we reach the end of LoadModule,
// so its too late to remove tilesets at this point.
//
void VirtualTextureHelpers::MergeModVTs()
{
    if (!gExtender->GetConfig().MergeVirtualTextures) {
        return;
    }

    auto vtManager = (*GetStaticSymbols().ls__gGlobalResourceManager)->VirtualTextureManager;
    if (vtManager->TileSets.size() < 2) {
        return;
    }

    Array<STDString> paths;
    for (auto const& path : vtManager->TileSets) {
        paths.push_back(path.Value());
    }

    auto outputGts = MergeGTS(paths);
    if (!outputGts) {
        return;
    }

    mergedTileSetId_ = Guid::Generate();
    auto tileSetGuidStr = FixedString{ mergedTileSetId_.ToString() };
    vtManager->TileSets.clear();
    vtManager->TileSets.set(mergedTileSetId_, *outputGts);

    auto banks = GetStaticSymbols().GetCurrentResourceBank();
    auto bank = banks->Container.Banks[(unsigned)ResourceBankType::VirtualTexture];
    resource::VirtualTextureResource* firstTex{ nullptr };

    for (auto const& res : bank->Resources) {
        auto tex = static_cast<resource::VirtualTextureResource*>(res.Value);
        if (tex->TileSetFileName) {
            tex->TileSetFileName = tileSetGuidStr;
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
    for (auto const& mod : modManager->LoadOrderedModules) {
        auto virtualTextureConfig = "Mods/" + mod.Info.Directory + "/ScriptExtender/VirtualTextures.json";
        auto reader = GetStaticSymbols().MakeFileReader(virtualTextureConfig);

        if (reader.IsLoaded()) {
            DEBUG("Loading virtual texture mappings: %s", virtualTextureConfig.c_str());
            auto configText = reader.ToString();

            rapidjson::Document root;
            if (root.Parse(configText.c_str()).HasParseError()) {
                OsiError("Unable to parse virtual texture configuration for mod '" << mod.Info.Name << "'");
            } else {
                auto mappings = root.FindMember("Mappings");
                if (mappings != root.MemberEnd() && mappings->value.IsArray()) {
                    for (auto const& mapping : mappings->value.GetArray()) {
                        if (mapping.IsObject() 
                            && mapping.HasMember("GTexName")
                            && mapping.HasMember("GTS")
                            && mapping["GTexName"].IsString() 
                            && mapping["GTS"].IsString()) {
                            auto gTex = STDString(mapping["GTexName"].GetString());
                            auto gts = STDString(mapping["GTS"].GetString());
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

END_SE()
