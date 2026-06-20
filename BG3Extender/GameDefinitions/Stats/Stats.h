#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Stats/Common.h>
#include <GameDefinitions/Stats/Functors.h>

BEGIN_NS(stats)

enum class RPGEnumerationType
{
    Int,
    Int64,
    Float,
    FixedString,
    Enumeration,
    Flags,
    GUID,
    StatsFunctors,
    Conditions,
    RollConditions,
    Requirements,
    // Legacy DOS2 type, unused
    MemorizationRequirements,
    TranslatedString,
    Unknown
};

struct RPGEnumeration : public ProtectedGameObject<RPGEnumeration>
{
    FixedString Name;
    LegacyMap<FixedString, int32_t> Values;

    static bool IsFlagType(FixedString const& typeName);
    RPGEnumerationType GetPropertyType() const;

    inline FixedString const& GetElementName() const
    {
        return Name;
    }
};

struct Modifier : public Noncopyable<Modifier>
{
    int32_t EnumerationIndex{ -1 };
    int32_t LevelMapIndex{ -1 };
    int32_t UnknownZero{ 0 };
    FixedString Name;

    inline FixedString const& GetElementName() const
    {
        return Name;
    }
};

struct ModifierList : public Noncopyable<ModifierList>
{
    CNamedElementManager<Modifier> Attributes;
    FixedString Name;

    Modifier* GetAttributeInfo(FixedString const& name, int * attributeIndex) const;

    inline FixedString const& GetElementName() const
    {
        return Name;
    }
};

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

struct ItemTypeManager : public CNamedElementManager<ItemType>
{
    uint64_t Unknown;
};

struct EquipmentGroup
{
    FixedString Name;
    TrackedCompactSet<FixedString> Equipment;
};

struct EquipmentSet
{
    FixedString Name;
    BYTE WeaponSetType;
    Vector<EquipmentGroup> EquipmentGroups;
};

struct SpellSet
{
    FixedString Name;
    AbilityId Ability;
    uint32_t LearningStrategy;
    TrackedCompactSet<FixedString> Spells;
};

struct NameGroupName
{
    int Frequency;
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
    int Frequency;
    FixedString NameGroup;
    int Type;
    FixedString ItemName;
};

struct RootGroup
{
    int32_t Frequency;
    FixedString RootGroup;
    FixedString field_10;
    Array<NameGroupLink*> NameGroups;
};

struct LevelGroup
{
    int32_t MinLevel;
    int32_t MaxLevel;
    FixedString Name;
    Array<RootGroup*> RootGroups;
};

struct ItemGroup
{
    FixedString Name;
    int32_t field_4;
    Array<LevelGroup*> LevelGroups;
};


struct ItemProgressionManager : ProtectedGameObject<ItemProgressionManager>
{
    LegacyMap<FixedString, ItemGroup*> ItemGroups;
    LegacyMap<FixedString, NameGroup*> NameGroups;
    ItemGroup* CurrentItemGroup;
    LevelGroup* CurrentLevelGroup;
    RootGroup* CurrentRootGroup;
    NameGroup* CurrentNameCool;
};

struct ItemCombinationResultItem
{
    FixedString Result;
    int32_t Amount;
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
    uint32_t IngredientType;
    uint32_t TransformType;
    uint8_t CombineType;
};

struct ItemCombination
{
    FixedString Name;
    Array<ItemCombinationIngredient> Ingredients;
    Array<ItemCombinationResult> Results;
    uint8_t AlchemyCombineType;
    Guid DyeColorPresetResource;
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

struct RPGStats_Treasure_Object_Info
{
    FixedString ItemType;
    FixedString StatsId;
    __int64 field_10;
    int field_18;
    int Amount;
};

struct RNG
{
    uint32_t* Numbers;
    int field_8;
    int NumSteps;
    int field_10;
    int field_14;
    int field_18;
    int Min;
    int field_20;
    bool Initialized;
};

struct PreParsedDataBuffer
{
    ScratchString Buf;
    FixedString Name;
    void* field_28;
};

struct ItemColor
{
    uint32_t Color[3];
};

struct BloodType
{
    FixedString Info[9];
};

struct RPGStats : public ProtectedGameObject<RPGStats>
{
    using LoadProc = void(RPGStats* self, Array<STDString>* paths);
    using ParseStructureFolderProc = void(RPGStats* self, Array<STDString>* paths);

    struct VMTMappings
    {
        bool VMTsMapped{ false };
        void* ObjectVMT{ nullptr };

        void Update();
    };

    static VMTMappings sVMTMappings;

    CNamedElementManager<RPGEnumeration> ModifierValueLists;
    CNamedElementManager<ModifierList> ModifierLists;
    CNamedElementManager<Object> Objects;
    CNamedElementManager<TreasureCategory> TreasureCategories;
    CNamedElementManager<TreasureTable> TreasureTables;
    ItemTypeManager ItemTypes;
    LegacyMap<FixedString, Functors*> StatsFunctors;
    RPGEnumeration* CurrentParsedValueList;
    Modifier* CurrentParsedType;
    TreasureTable* CurrentTreasureTable;
    TreasureCategory* CurrentTreasureCategory;
    uint64_t RNG[5];
    HashMap<FixedString, float>* ExtraData;
    LegacyMap<FixedString, FixedString> CategoryMappings;
    LegacyMap<FixedString, int32_t> WeaponCounters;
    LegacyMap<FixedString, int32_t> SpellbookCounters;
    LegacyMap<FixedString, int32_t> ArmorCounters;
    LegacyRefMap<FixedString, void*> field_2F0;
    FixedString TreasureRarities[7];
    TrackedCompactSet<FixedString> FixedStrings;
    Array<int64_t*> Int64s;
    Array<Guid> GUIDs;
    Array<float> Floats;
    Array<TranslatedString> TranslatedStrings;
    CNamedElementManager<EquipmentSet>* EquipmentSetManager;
    CNamedElementManager<SpellSet>* SpellSetManager;
    ItemProgressionManager* ItemProgressionManager;
    CNamedElementManager<ItemCombination>* ItemCombinationManager;
    void* CurrentDataBuffer;
    FixedString CurrentDataBufferPath;
    LegacyMap<FixedString, int32_t> PreParsedDataBufferMap;
    Array<PreParsedDataBuffer*> PreParsedDataBuffers;
    LegacyMap<FixedString, BloodType*> BloodTypes;
    bool field_3E0;
    bool IsLoading;
    bool Loaded;
    uint32_t FastLock;
    Array<STDString> Conditions;

    Modifier * GetModifierInfo(FixedString const& modifierListName, FixedString const& modifierName);
    ModifierList * GetTypeInfo(Object * object);

    bool ObjectExists(FixedString const& statsId, FixedString const& type);
    std::optional<Object*> CreateObject(FixedString const& name, FixedString const& type);
    std::optional<Object*> CreateObject(FixedString const& name, int32_t modifierListIndex);
    Functors* ConstructFunctorSet(FixedString const& propertyName);
    Functor* ConstructFunctor(FunctorId action);
    void SyncWithPrototypeManager(Object* object);

    std::optional<FixedString*> GetFixedString(int stringId);
    FixedString* GetOrCreateFixedString(int& stringId);
    std::optional<int64_t*> GetInt64(int int64Id);
    int64_t* GetOrCreateInt64(int& int64Id);
    std::optional<float*> GetFloat(int floatId);
    float* GetOrCreateFloat(int& floatId);
    std::optional<Guid*> GetGuid(int guidId);
    Guid* GetOrCreateGuid(int& guidId);
    std::optional<TranslatedString*> GetTranslatedString(int tsId);
    TranslatedString* GetOrCreateTranslatedString(int& tsId);
    std::optional<STDString*> GetConditions(int conditionsId);
    int GetOrCreateConditions(STDString const& conditions);

    std::optional<int> EnumLabelToIndex(FixedString const& enumName, char const* enumLabel);
    FixedString EnumIndexToLabel(FixedString const& enumName, int index);
};

Object * StatFindObject(char const * name, bool warnOnError = true);
Object * StatFindObject(int index);

END_NS()
