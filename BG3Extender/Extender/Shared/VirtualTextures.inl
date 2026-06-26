#include <CoreLib/JsonLibs.h>
#include <Extender/Shared/VirtualTextureMerge.inl>

BEGIN_SE()

std::optional<STDString> MergeGTS(vt::GTSStitchedFileGroup const& group)
{
    DEBUG("Creating merged virtual texture group #%d", group.Index);

    vt::GTSStitchedFile stitched;
    stitched.TileSets = group.TileSets;
    stitched.Index = group.Index;

    vt::MergedTileSetGeometryCalculator geom;
    geom.TileSets = stitched.TileSets;
    if (!geom.DoAutoPlacement()) {
        ERR("(#%d) Failed to calculate merged tileset geometry, virtual textures will not be available!", group.Index);
        return {};
    }

    DEBUG("(#%d) Merged geometry: %d x %d tiles (%d x %d px)",
        group.Index,
        geom.TotalWidth, geom.TotalHeight,
        geom.TotalWidth * 128, geom.TotalHeight * 128
    );

    stitched.Init(geom.TotalWidth, geom.TotalHeight);
    if (stitched.Build()) {
        DEBUG("(#%d) Built merged GTS: %s", group.Index, stitched.OutputPath.c_str());
        return stitched.OutputPath;
    } else {
        ERR("(#%d) Merged tile set build failed, virtual textures will not be available!", group.Index);
        return {};
    }
}

struct GTSMapping
{
    std::optional<STDString> MappedPath;
};

bool MergeGTS(HashMap<STDString, GTSMapping>& relativePaths)
{
    DEBUG("Computing merged virtual texture groups");

    Array<vt::GTSFile*> tileSets;
    for (auto const& path : relativePaths) {
        auto reader = GetStaticSymbols().MakeFileReader(path.Key(), PathRootType::Data);
        if (reader.IsLoaded()) {
            auto gts = GameAlloc<vt::GTSFile>();
            auto absPath = GetStaticSymbols().ToPath(path.Key(), PathRootType::Data);
            gts->DataPath = path.Key();
            gts->AbsolutePath = absPath;
            gts->Buf.resize((uint32_t)reader.Size());
            std::copy(static_cast<uint8_t*>(reader.Buf()), static_cast<uint8_t*>(reader.Buf()) + reader.Size(), gts->Buf.begin());
            char const* reason{ nullptr };
            try {
                gts->Read();
                tileSets.push_back(gts);
            } catch (std::runtime_error& e) {
                ERR("Failed to load VT '%s': %s", path.Key().c_str(), e.what());
            }
        } else {
            ERR("Referenced VT tileset does not exist: %s", path.Key().c_str());
        }
    }

    // We need at least one loaded tileset to proceed with stitching
    if (tileSets.empty()) {
        ERR("Unable to merge tileset, no source tilesets loaded");
        return false;
    }

    auto groups = vt::ComputeGroups(tileSets);
    DEBUG("Building %d output tile sets", groups.size());

    for (auto const& group : groups) {
        auto mergedPath = MergeGTS(group);
        if (mergedPath) {
            for (auto gts : group.TileSets) {
                for (auto& input : relativePaths) {
                    if (input.Key() == gts->DataPath) {
                        input.Value().MappedPath = mergedPath;
                        break;
                    }
                }
            }
        }
    }

    return true;
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
    Array<resource::VirtualTextureResource*> preloads;

    for (auto const& res : bank->Resources) {
        auto tex = static_cast<resource::VirtualTextureResource*>(res.Value);
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
                preloads.push_back(tex);
            }

            tex->TileSetFileName = FixedString(gtsGuid->ToString());
            pendingRemaps.remove(tex->GTexFileName);
        }
    }

    for (auto preload : preloads) {
        preload->Load(*GetStaticSymbols().ls__gGlobalResourceManager);
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
        HashMap<STDString, GTSMapping> sourceGts;
        for (auto const& path : sourceTileSets_) {
            sourceGts.set(STDString(path.GetStringView()), GTSMapping{});
        }

        if (MergeGTS(sourceGts)) {
            for (auto& path : gtsPaths_) {
                auto mapping = sourceGts.try_get(STDString(path.Value().GetStringView()));
                if (mapping && mapping->MappedPath) {
                    path.Value() = FixedString(*mapping->MappedPath);
                }
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

    HashMap<STDString, GTSMapping> paths;
    for (auto const& path : vtManager->TileSets) {
        paths.set(path.Value(), GTSMapping{});
    }

    if (!MergeGTS(paths)) {
        return;
    }

    mergedTileSetId_ = Guid::Generate();
    auto tileSetGuidStr = FixedString{ mergedTileSetId_.ToString() };
    vtManager->TileSets.clear();
    // ...
    // vtManager->TileSets.set(mergedTileSetId_, *outputGts);

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
