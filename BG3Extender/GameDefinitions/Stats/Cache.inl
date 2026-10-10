#include <GameDefinitions/Stats/Cache.h>

BEGIN_NS(stats)

StatStructureCache gStatStructureCache;

void StatEnumerationCache::Build(RPGEnumeration const& enumeration)
{
	if (enumeration.Values.size() == 0) {
		return;
	}

	int32_t maxValue = 0;
	for (auto const& kv : enumeration.Values) {
		maxValue = std::max(maxValue, kv.Value);
	}

	// Cap value list size in case the enum contains irrationally large values
	maxValue = std::min(maxValue, (int32_t)enumeration.Values.size() * 10);
	Labels.resize(maxValue + 1);

	for (auto const& kv : enumeration.Values) {
		if (kv.Value >= 0 && kv.Value <= maxValue) {
			Labels[kv.Value] = kv.Key;
			LabelToIndex.set(kv.Key, kv.Value);
		} else {
			WARN("Label '%s' (%d) in enumeration '%s' is out of range and will be ignored", kv.Key.GetString(), kv.Value, enumeration.Name.GetString());
		}
	}
}

void StatModifierCache::Build(uint32_t index, Modifier const& modifier, StatStructureCacheData& cache)
{
	auto stats = GetStaticSymbols().GetStats();
	auto valueList = stats->ModifierValueLists.GetByHandle(modifier.EnumerationIndex);

	Index = index;
	Name = modifier.Name;
	TypeName = valueList->Name;
	Type = valueList->GetPropertyType();
	Enum = cache.Enumerations[modifier.EnumerationIndex].get();
}

void StatModifierListCache::Build(ModifierList const& modifierList, StatStructureCacheData& cache)
{
	Modifiers.resize(modifierList.Attributes.Values.size());
	for (uint32_t i = 0; i < modifierList.Attributes.Values.size(); i++) {
		auto const& modifier = *modifierList.Attributes.Values[i];
		Modifiers[i].Build(i, modifier, cache);
		NameToModifier.set(modifier.Name, &Modifiers[i]);
	}
}

void StatValuePools::Clear()
{
	FixedStrings.clear();
	Int64s.clear();
	Floats.clear();
	Guids.clear();
	TranslatedStrings.clear();
}

void StatValuePools::Refresh()
{
	auto stats = GetStaticSymbols().GetStats();

	FixedStrings.clear();
	for (uint32_t i = 1; i < stats->FixedStrings.size(); i++) {
		FixedStrings.set(stats->FixedStrings[i], (int32_t)i);
	}

	Int64s.clear();
	for (uint32_t i = 1; i < stats->Int64s.size(); i++) {
		Int64s.set(*stats->Int64s[i], (int32_t)i);
	}

	Floats.clear();
	for (uint32_t i = 1; i < stats->Floats.size(); i++) {
		Floats.set(stats->Floats[i], (int32_t)i);
	}

	Guids.clear();
	for (uint32_t i = 1; i < stats->GUIDs.size(); i++) {
		Guids.set(stats->GUIDs[i], (int32_t)i);
	}

	TranslatedStrings.clear();
	for (uint32_t i = 1; i < stats->TranslatedStrings.size(); i++) {
		TranslatedStrings.set(stats->TranslatedStrings[i], (int32_t)i);
	}
}

StatModifierCache const* StatStructureCache::GetCachedAttribute(uint32_t modifierListIndex, FixedString const& attribute)
{
	RebuildIfNecessary();
	auto modifierList = GetModifierList(modifierListIndex);
	if (!modifierList) return nullptr;

	return modifierList->NameToModifier.get_or_default(attribute);
}

std::optional<int32_t> StatStructureCache::GetCachedCondition(STDString const& condition)
{
	RebuildIfNecessary();
	auto idx = liveData_->ConditionToIndex.try_get(condition);
	return idx ? *idx : std::optional<int32_t>{};
}

void StatStructureCache::Invalidate()
{
	invalidated_ = true;
}

void StatStructureCache::OnStatsLoaded()
{
	values_.Refresh();
}

void StatStructureCache::RebuildIfNecessary()
{
	if (invalidated_) {
		lock_.WriteLock();
		if (invalidated_) {
			Rebuild();
		}
		lock_.WriteUnlock();
	}
}

void StatStructureCache::Rebuild()
{
	buildData_.reset(GameAlloc<StatStructureCacheData>());

	auto stats = GetStaticSymbols().GetStats();
	buildData_->Modifiers.resize(stats->ModifierLists.Values.size());
	buildData_->Enumerations.resize(stats->ModifierValueLists.Values.size());

	for (uint32_t i = 0; i < stats->ModifierValueLists.Values.size(); i++) {
		CacheEnumeration(i, *stats->ModifierValueLists.Values[i]);
	}

	for (uint32_t i = 0; i < stats->ModifierLists.Values.size(); i++) {
		CacheModifierList(i, *stats->ModifierLists.Values[i]);
	}

	for (uint32_t i = 0; i < stats->Conditions.size(); i++) {
		buildData_->ConditionToIndex.set(stats->Conditions[i], i);
	}

	previousData_.reset();
	liveData_.swap(previousData_);
	buildData_.swap(liveData_);
	invalidated_ = false;
}

StatModifierListCache const* StatStructureCache::GetModifierList(uint32_t modifierListIndex)
{
	if (modifierListIndex >= liveData_->Modifiers.size()) {
		return nullptr;
	}

	return liveData_->Modifiers[modifierListIndex].get();
}

void StatStructureCache::CacheModifierList(uint32_t modifierListIndex, ModifierList const& modifierList)
{
	auto cache = MakeUnique<StatModifierListCache>();
	cache->Build(modifierList, *buildData_);

	buildData_->Modifiers[modifierListIndex] = std::move(cache);
}

void StatStructureCache::CacheEnumeration(uint32_t valueListIndex, RPGEnumeration const& enumeration)
{
	auto cache = MakeUnique<StatEnumerationCache>();
	cache->Build(enumeration);

	buildData_->Enumerations[valueListIndex] = std::move(cache);
}

END_NS()
