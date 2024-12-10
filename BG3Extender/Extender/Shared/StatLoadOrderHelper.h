#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Stats/Stats.h>
#include <shared_mutex>

BEGIN_NS(stats)

class StatLoadOrderHelper
{
public:
    struct StatsEntryModMapping
    {
        // First mod we've seen this entry in
        FixedString FirstMod;
        // Most recent mod we've seen this entry in
        FixedString LastMod;
        void* PreParseBuf;
    };

    void OnLoadStarted();
    void OnLoadFinished();
    void OnStatFileOpened();
    void OnStatFileOpened(Path const& path);
    void UpdateModDirectoryMap();

    StatsEntryModMapping const* GetStatsEntryMod(FixedString statId) const;
    std::vector<Object*> GetStatsLoadedBefore(FixedString modId) const;

private:
    std::shared_mutex modMapMutex_;
    std::unordered_map<STDString, FixedString> modDirectoryToModMap_;
    std::unordered_map<FixedString, StatsEntryModMapping> statsEntryToModMap_;
    FixedString statLastTxtMod_;
    bool loadingStats_{ false };
};

END_NS()
