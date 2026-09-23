#pragma once

#include <GameDefinitions/Stats/Stats.h>

BEGIN_NS(stats)

struct StatStructureCacheData;

struct StatEnumerationCache
{
    Array<FixedString> Labels;
    HashMap<FixedStringNoRef, int32_t> LabelToIndex;

    void Build(RPGEnumeration const& modifier);
};

struct StatModifierCache
{
    uint32_t Index;
    FixedString Name;
    FixedString TypeName;
    RPGEnumerationType Type;
    StatEnumerationCache const* Enum;

    void Build(uint32_t index, Modifier const& modifier, StatStructureCacheData& cache);
};

struct StatModifierListCache
{
    Array<StatModifierCache> Modifiers;
    HashMap<FixedStringNoRef, StatModifierCache*> NameToModifier;

    void Build(ModifierList const& modifierList, StatStructureCacheData& cache);
};

struct StatStructureCacheData
{
    Array<GameUniquePtr<StatModifierListCache>> Modifiers;
    Array<GameUniquePtr<StatEnumerationCache>> Enumerations;
};

struct StatValuePools
{
    void Clear();
    void Refresh();

    HashMap<FixedStringNoRef, int32_t> FixedStrings;
    HashMap<int64_t, int32_t> Int64s;
    HashMap<float, int32_t> Floats;
    HashMap<Guid, int32_t> Guids;
    HashMap<TranslatedString, int32_t> TranslatedStrings;
};

class StatStructureCache
{
public:
    StatModifierCache const* GetCachedAttribute(uint32_t modifierListIndex, FixedString const& attribute);
    void Invalidate();
    void OnStatsLoaded();

    inline StatValuePools& GetValuePool()
    {
        return values_;
    }

private:
    StatValuePools values_;
    GameUniquePtr<StatStructureCacheData> liveData_;
    GameUniquePtr<StatStructureCacheData> buildData_;
    GameUniquePtr<StatStructureCacheData> previousData_;
    SRWLock lock_;
    std::atomic<bool> invalidated_{ true };

    StatModifierListCache const* GetModifierList(uint32_t modifierListIndex);
    void CacheModifierList(uint32_t modifierListIndex, ModifierList const& modifierList);
    void CacheEnumeration(uint32_t valueListIndex, RPGEnumeration const& enumeration);
    void Rebuild();
    void RebuildIfNecessary();
};

extern StatStructureCache gStatStructureCache;

END_NS()
