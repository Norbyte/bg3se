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

	bool Stitch();
	HashMap<FixedString, FixedString> CollectRemaps();
	bool NeedsRebuild(std::unordered_set<FixedString> const& newTileSets);
	void RebuildIfNecessary();
};

#if defined(__APPLE__)
class MacOSVirtualTextureHelpers : public VirtualTextureHelpers
{
public:
    void Load();
    bool Stitch();
    HashMap<FixedString, FixedString> CollectRemaps();
    bool NeedsRebuild(std::unordered_set<FixedString> const& newTileSets);
    void RebuildIfNecessary();
};
#endif

END_SE()
