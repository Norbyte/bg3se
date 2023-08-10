#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Stats/Stats.h>
#include <shared_mutex>

BEGIN_SE()

class StatLoadOrderHelper
{
public:
	void OnLoadStarted();
	void OnLoadFinished();
	void OnStatFileOpened();
	void OnStatFileOpened(Path const& path);
	void UpdateModDirectoryMap();

	FixedString GetStatsEntryMod(FixedString statId) const;
	std::vector<CRPGStats_Object*> GetStatsLoadedBefore(FixedString modId) const;

private:
	struct StatsEntryModMapping
	{
		FixedString Mod;
		void* PreParseBuf;
	};

	std::shared_mutex modMapMutex_;
	std::unordered_map<STDString, FixedString> modDirectoryToModMap_;
	std::unordered_map<FixedString, StatsEntryModMapping> statsEntryToModMap_;
	FixedString statLastTxtMod_;
	bool loadingStats_{ false };
};

END_SE()
