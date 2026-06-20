#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Stats/Common.h>

BEGIN_NS(stats)

struct ItemTypeBoostGroup
{
    TrackedCompactSet<FixedString> Boosts;
    int32_t field_10;
};

struct ItemTypeLevelGroup
{
    int32_t Type;
    Array<ItemTypeBoostGroup*> BoostGroups;
};

struct ItemType
{
    STDString Color;
    FixedString Name2;
    float field_1C;
    int32_t Index;
    FixedString Name3;
    FixedString Name4;
    Array<ItemTypeLevelGroup*> LevelGroups;
    FixedString Name;

    inline FixedString const& GetElementName() const
    {
        return Name2;
    }
};


struct EquipmentGroup
{
    FixedString Name;
    TrackedCompactSet<FixedString> Equipment;
};

struct EquipmentSet
{
    FixedString Name;
    uint8_t WeaponSetType{ 0 };
    Vector<EquipmentGroup> EquipmentGroups;

    inline FixedString const& GetElementName() const
    {
        return Name;
    }
};

struct SpellSet
{
    FixedString Name;
    AbilityId Ability{ AbilityId::None };
    uint32_t LearningStrategy{ 0 };
    TrackedCompactSet<FixedString> Spells;

    inline FixedString const& GetElementName() const
    {
        return Name;
    }
};

struct NameGroupName
{
    int Frequency{ 1 };
    TranslatedString Name;
    TranslatedString Name2;
};

struct NameGroup
{
    FixedString Name;
    Array<NameGroupName*> Names;
    Array<NameGroupName*> NamesCool;
};

struct NameGroupLink
{
    int Frequency{ 1 };
    FixedString NameGroup;
    int Type{ 0 };
    FixedString ItemName;
};

struct RootGroup
{
    int32_t Frequency{ 1 };
    FixedString RootGroup;
    FixedString field_10;
    Array<NameGroupLink*> NameGroups;
};

struct LevelGroup
{
    int32_t MinLevel{ 0 };
    int32_t MaxLevel{ 0 };
    FixedString Name;
    Array<RootGroup*> RootGroups;
};

struct ItemGroup
{
    FixedString Name;
    int32_t field_4{ 0 };
    Array<LevelGroup*> LevelGroups;
};


struct ItemCombinationPreviewData
{
    FixedString Name;
    FixedString Type;
    FixedString StatsID;
    FixedString UniqueName;
    FixedString Tooltip;
};

struct ItemCombinationResultItem
{
    FixedString Result;
    int32_t Amount{ 1 };
};

struct ItemCombinationResult
{
    Array<ItemCombinationResultItem> Items;
    FixedString PreviewStatsID;
    FixedString PreviewIcon;
    TranslatedString field_18;
    FixedString PreviewTooltip;
};

struct ItemCombinationIngredient
{
    FixedString Object;
    uint32_t IngredientType{ 0 };
    uint32_t TransformType{ 0 };
    uint8_t CombineType{ 0 };
};

struct ItemCombination
{
    FixedString Name;
    Array<ItemCombinationIngredient> Ingredients;
    Array<ItemCombinationResult> Results;
    uint8_t AlchemyCombineType;
    Guid DyeColorPresetResource;

    inline FixedString const& GetElementName() const
    {
        return Name;
    }
};

struct ItemCombinationPropertyEntry
{
    FixedString ID;
    uint32_t Type{ 0 };
    FixedString Result;
};

struct ItemCombinationProperty
{
    FixedString Name;
    FixedString Icon;
    FixedString Tooltip;
    Array<ItemCombinationPropertyEntry> Entries;
};


struct TreasureSubTableCategory
{
    int32_t Index;
    int32_t Frequency;
    std::array<uint16_t, 7> Frequencies;
    std::array<uint16_t, 7> Frequencies2;
    bool IsTreasureTable;
    bool IsTreasureTable2;
};

struct TreasureSubTableDropCount
{
    int32_t Chance;
    int32_t Amount;
};


struct TreasureSubTable
{
    Array<TreasureSubTableCategory*> Categories;
    Array<int32_t> CategoryFrequencies;
    int32_t TotalFrequency{ 0 };
    Array<TreasureSubTableDropCount> DropCounts;
    Array<int32_t> Amounts;
    int32_t TotalCount{ 0 };
    int32_t StartLevel{ 0 };
    int32_t EndLevel{ 0 };
    FixedString field_54;
};


struct TreasureTable
{
    FixedString Name;
    int MinLevel;
    int MaxLevel;
    bool IgnoreLevelDiff;
    bool UseTreasureGroupContainers;
    bool CanMerge;
    Array<TreasureSubTable*> SubTables;

    inline FixedString const& GetElementName() const
    {
        return Name;
    }
};

struct TreasureCategoryItem
{
    FixedString Name;
    int Priority;
    int MinAmount;
    int MaxAmount;
    int ActPart;
    int Unique;
    int MinLevel;
    int MaxLevel;
};

struct TreasureCategory
{
    FixedString Category;
    Vector<TreasureCategoryItem*> Items;
    uint64_t Unknown[3];

    inline FixedString const& GetElementName() const
    {
        return Category;
    }
};

END_NS()
