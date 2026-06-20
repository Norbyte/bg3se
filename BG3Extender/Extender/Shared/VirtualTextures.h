#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Resources.h>
#include <mutex>

BEGIN_SE()

class VirtualTextureHelpers
{
public:
    void Load();

private:
    HashMap<FixedString, FixedString> gtsPaths_;
    HashMap<FixedString, Guid> gtsToGuid_;
    std::unordered_set<FixedString> sourceTileSets_;
    bool built_{ false };
    std::mutex lock_;
    Guid mergedTileSetId_;

    HashMap<FixedString, FixedString> CollectRemaps();
    bool NeedsRebuild(std::unordered_set<FixedString> const& newTileSets);
    void RebuildIfNecessary();
    void BindSEVirtualTextures();
    void MergeModVTs();
};

END_SE()
