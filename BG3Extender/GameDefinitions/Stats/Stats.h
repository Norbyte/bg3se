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

struct ItemType
{
    // UNMAPPED
    FixedString Name;

    inline FixedString const& GetElementName() const
    {
        return Name;
    }
};

struct ItemTypeManager : public CNamedElementManager<ItemType>
{
    uint64_t Unknown;
};

/*
struct CSkillSet
{
    FixedString Name;
    ObjectSet<FixedString> Skills;
};

struct CSkillSetManager : public CNamedElementManager<CSkillSet>
{
    CSkillSet * ParsedSkillSet;
};

struct CEquipmentGroup
{
    FixedString Name;
    ObjectSet<FixedString> Equipment;
};

struct CEquipmentSet
{
    FixedString Name;
    Vector<CEquipmentGroup*> Groups;
};

struct CEquipmentSetManager : public CNamedElementManager<CEquipmentSet>
{
    CEquipmentSet * ParsedEquipmentSet;
};
*/

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

/*
struct CNameGroupName
{
    int field_0;
    TranslatedString Name;
    TranslatedString Name2;
};


struct CNameGroup
{
    FixedString Name;
    ObjectSet<CNameGroupName*> Names;
    ObjectSet<CNameGroupName*> NamesCool;
};


struct CNameGroupLink
{
    int field_0;
    FixedString NameGroup;
    int NoneCoolSuffix;
    FixedString ItemName;
};


struct CRootGroup
{
    int MinLevel;
    int MaxLevel;
    FixedString RootGroup;
    FixedString field_10;
    ObjectSet<CNameGroupLink*> NameGroupLinks;
};


struct CLevelGroup
{
    int MinLevel;
    int MaxLevel;
    FixedString Name;
    ObjectSet<CRootGroup*> RootGroups;
};


struct CItemGroup
{
    FixedString Name;
    ObjectSet<CLevelGroup*> LevelGroups;
};


struct CItemProgressionManager : public ProtectedGameObject<CItemProgressionManager>
{
    LegacyMap<FixedString, CItemGroup*> ItemGroups;
    LegacyMap<FixedString, CNameGroup*> NameGroups;
    CItemGroup* CurrentItemGroup;
    CLevelGroup* CurrentLevelGroup;
    CRootGroup* CurrentRootGroup;
    CNameGroup* CurrentNameGroup;
};


MARK_ALLOCATABLE(CSkillSet);
MARK_ALLOCATABLE(CEquipmentGroup);
MARK_ALLOCATABLE(CEquipmentSet);*/
MARK_ALLOCATABLE(TreasureSubTable);
MARK_ALLOCATABLE(TreasureSubTableCategory);
MARK_ALLOCATABLE(TreasureTable);
MARK_ALLOCATABLE(TreasureCategory);
MARK_ALLOCATABLE(TreasureCategoryItem);
/*MARK_ALLOCATABLE(CNameGroupName);
MARK_ALLOCATABLE(CNameGroup);
MARK_ALLOCATABLE(CNameGroupLink);
MARK_ALLOCATABLE(CRootGroup);
MARK_ALLOCATABLE(CLevelGroup);
MARK_ALLOCATABLE(CItemGroup);


extern CRPGStatsVMTMappings gCRPGStatsVMTMappings;
*/

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
    uint64_t Unkn1[9];
    HashMap<FixedString, float>* ExtraData;
    LegacyMap<FixedString, FixedString> field_290;
    LegacyMap<FixedString, void*> field_2A8;
    LegacyMap<FixedString, void*> field_2C0;
    LegacyMap<FixedString, void*> field_2D8;
    LegacyRefMap<FixedString, void*> field_2F0;
    FixedString TreasureRarities[7];
    TrackedCompactSet<FixedString> FixedStrings;
    Array<int64_t*> Int64s;
    Array<Guid> GUIDs;
    Array<float> Floats;
    Array<TranslatedString> TranslatedStrings;
    void* EquipmentSetManager;
    void* SpellSetManager;
    void* ItemProgressionManager;
    void* ItemCombinationManager;
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
    /*void SyncObjectFromServer(MsgS2CSyncStat const& msg);*/
    void SyncWithPrototypeManager(Object* object);
    /*void BroadcastSyncAll();*/

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
