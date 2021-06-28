#include <Extender/Shared/StatLoadOrderHelper.h>

BEGIN_SE()

void StatLoadOrderHelper::OnLoadStarted()
{
	loadingStats_ = true;
	statLastTxtMod_ = FixedString{};
	statsEntryToModMap_.clear();
	UpdateModDirectoryMap();
}

void StatLoadOrderHelper::OnLoadFinished()
{
	OnStatFileOpened();
	loadingStats_ = false;
}

void StatLoadOrderHelper::UpdateModDirectoryMap()
{
	std::unique_lock _(modMapMutex_);
	modDirectoryToModMap_.clear();

	/*auto modManager = GetModManagerClient();
	if (modManager) {
		for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
			auto directory = ToUTF8(mod.Info.Directory);
			modDirectoryToModMap_.insert(std::make_pair(directory, mod.Info.ModuleUUID));
		}
	}*/
}

void StatLoadOrderHelper::OnStatFileOpened()
{
	/*auto stats = GetStaticSymbols().GetStats();
	auto const& bufs = stats->PreParsedDataBufferMap;
	bufs.Iterate([this, stats](auto const& key, auto const& preParseBufIdx) {
		auto preParseBuf = stats->PreParsedDataBuffers[(uint32_t)preParseBufIdx];
		auto entry = statsEntryToModMap_.find(key);
		if (entry == statsEntryToModMap_.end()) {
			StatsEntryModMapping mapping;
			mapping.Mod = statLastTxtMod_;
			mapping.PreParseBuf = preParseBuf;
			statsEntryToModMap_.insert(std::make_pair(key, mapping));
		} else if (entry->second.PreParseBuf != preParseBuf) {
			entry->second.Mod = statLastTxtMod_;
			entry->second.PreParseBuf = preParseBuf;
		}
	});*/
}

static std::regex sStatPathRegex(".*/Public/(.*)/Stats/Generated/.*.txt$");

void StatLoadOrderHelper::OnStatFileOpened(Path const& path)
{
	if (!loadingStats_) return;

	std::cmatch match;
	if (std::regex_match(path.Name.c_str(), match, sStatPathRegex)) {
		std::unique_lock lock(modMapMutex_);

		auto modIt = modDirectoryToModMap_.find(match[1].str().c_str());
		if (modIt != modDirectoryToModMap_.end()) {
			statLastTxtMod_ = modIt->second;
			OnStatFileOpened();
		} else {
			WARN("Unable to resolve mod while loading stats .txt: %s", path.Name.c_str());
		}
	}
}

FixedString StatLoadOrderHelper::GetStatsEntryMod(FixedString statId) const
{
	auto entryIt = statsEntryToModMap_.find(statId);
	if (entryIt != statsEntryToModMap_.end()) {
		return entryIt->second.Mod;
	} else {
		return {};
	}
}

std::vector<CRPGStats_Object*> StatLoadOrderHelper::GetStatsLoadedBefore(FixedString modId) const
{
	std::unordered_set<FixedString> modsLoadedBefore;
	auto state = gExtender->GetCurrentExtensionState();
	if (!state) return {};

	bool modIdFound{ false };
	for (auto const& mod : state->GetModManager()->BaseModule.LoadOrderedModules) {
		modsLoadedBefore.insert(mod.Info.ModuleUUIDString);
		if (mod.Info.ModuleUUIDString == modId) {
			modIdFound = true;
			break;
		}
	}

	if (!modIdFound) {
		OsiError("Couldn't fetch stat entry list - mod " << modId << " is not loaded.");
		return {};
	}

	std::vector<CRPGStats_Object*> statsLoadedBefore;
	auto stats = GetStaticSymbols().GetStats();
	for (auto const& object : stats->Objects.Primitives) {
		auto statEntryMod = GetStatsEntryMod(object->Name);
		if (statEntryMod && modsLoadedBefore.find(statEntryMod) != modsLoadedBefore.end()) {
			statsLoadedBefore.push_back(object);
		}
	}

	return statsLoadedBefore;
}


END_SE()
