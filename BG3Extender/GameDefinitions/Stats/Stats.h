#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
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
	Unknown
};

struct RPGEnumeration : public ProtectedGameObject<RPGEnumeration>
{
	FixedString Name;
	Map<FixedString, int32_t> Values;

	static bool IsFlagType(FixedString const& typeName);
	RPGEnumerationType GetPropertyType() const;
};

struct Modifier : public ProtectedGameObject<Modifier>
{
	int32_t EnumerationIndex;
	int32_t LevelMapIndex;
	int32_t UnknownZero;
	FixedString Name;
};

struct ModifierList
{
	CNamedElementManager<Modifier> Attributes;
	FixedString Name;

	Modifier* GetAttributeInfo(FixedString const& name, int * attributeIndex) const;
};

struct ExtraData : public ProtectedGameObject<ExtraData>
{
	Map<FixedString, float> Properties;
};


struct ItemTypeManager : public CNamedElementManager<uint64_t>
{
	uint64_t Unknown;
};

struct SpellPrototype : public Noncopyable<SpellPrototype>
{
	using InitProc = void(SpellPrototype* self, FixedString const& spellId);

	int StatsObjectIndex;
	SpellType SpellTypeId;
	FixedString SpellId;
	uint8_t SpellSchool;
	uint64_t SpellFlags;
	uint8_t SpellActionType;
	uint8_t SpellAnimationType;
	uint8_t SpellJumpType;
	uint8_t SpellHitAnimationType;
	uint8_t SpellAnimationIntentType;
	uint32_t LineOfSightFlags;
	uint32_t SpellCategoryFlags;
	int Level;
	int PowerLevel;
	bool HasMemoryCost;
	FixedString SpellContainerId;
	int RechargeValuesFrom;
	int RechargeValuesTo;
	int8_t DiceValue;
	int8_t Cooldown;
	uint32_t WeaponTypes;
	DescriptionInfo Description;
	uint8_t AiFlags;
	SpellPrototype* ParentPrototype;
	ObjectSet<SpellPrototype*> ChildPrototypes;
	Array<void*> UseCosts2;
	Array<void*> RitualCosts2;
	Array<void*> DualWieldingUseCosts2;
	Array<void*> CastTextEvents;
	ObjectSet<void*> UseCosts;
	ObjectSet<void*> DualWieldingUseCosts;
	ObjectSet<void*> RitualCosts;
	int VerbalIntent;
	Array<void*> SpellAnimationNoneMagic;
	Array<void*> DualWieldingSpellAnimationNoneMagic;
	FixedString PrepareEffect;
	FixedString PrepareSound;
	bool field_1F8;
	Array<void*> AlternativeCastTextEvents;
	uint8_t SourceLimbIndex;
	Array<void*> ContainerSpells;

	Object* GetStats() const;
};

struct SpellPrototypeManager : public ProtectedGameObject<SpellPrototypeManager>
{
	void* VMT;
	VirtualMultiHashMap<FixedString, SpellPrototype*> Spells;
	Array<FixedString> SpellNames;
	bool Initialized;

	void SyncStat(Object* object);
	bool SyncStat(Object* object, SpellPrototype* proto);
};


struct StatusPrototype : public Noncopyable<StatusPrototype>
{
	using InitProc = void (StatusPrototype* self, FixedString const& statusId, uint8_t flags);

	int StatsObjectIndex{ -1 };
	StatusType StatusId;
	FixedString StatusName;
	uint32_t StatusPropertyFlags;
	uint32_t StatusGroups;
	DescriptionInfo Description;
	uint8_t LEDEffect;
	uint8_t TickType;
	uint8_t Flags;
	ObjectSet<SurfaceType>* AbsorbSurfaceTypes{ nullptr };
	Array<Guid> Boosts;
	int16_t RemoveEvents;
	uint8_t HitAnimationType;

	Object* GetStats() const;
};

struct StatusPrototypeManager : public ProtectedGameObject<StatusPrototypeManager>
{
	void* VMT;
	Map<FixedString, StatusPrototype*> Statuses;
	bool Initialized;

	void SyncStat(Object* object);
	bool SyncStat(Object* object, StatusPrototype* proto);
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

struct TreasureSubTable
{
	struct Category
	{
		int32_t Index;
		int32_t Frequency;
		uint16_t Frequencies[7];
		uint16_t Frequencies2[7];
		bool IsTreasureTable;
		bool IsTreasureTable2;
	};

	struct DropCount
	{
		int32_t Chance;
		int32_t Amount;
	};

	Array<Category*> Categories;
	Array<int32_t> CategoryFrequencies;
	int32_t TotalFrequency;
	Array<DropCount> DropCounts;
	Array<int32_t> Amounts;
	int32_t TotalCount;
	int32_t StartLevel;
	int32_t EndLevel;
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
};

struct TreasureCategory
{
	struct Item
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

	FixedString Category;
	Vector<Item*> Items;
	uint64_t Unknown[3];
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
	Map<FixedString, CItemGroup*> ItemGroups;
	Map<FixedString, CNameGroup*> NameGroups;
	CItemGroup* CurrentItemGroup;
	CLevelGroup* CurrentLevelGroup;
	CRootGroup* CurrentRootGroup;
	CNameGroup* CurrentNameGroup;
};


MARK_ALLOCATABLE(CSkillSet);
MARK_ALLOCATABLE(CEquipmentGroup);
MARK_ALLOCATABLE(CEquipmentSet);*/
MARK_ALLOCATABLE(TreasureSubTable);
MARK_ALLOCATABLE(TreasureSubTable::Category);
MARK_ALLOCATABLE(TreasureTable);
MARK_ALLOCATABLE(TreasureCategory);
MARK_ALLOCATABLE(TreasureCategory::Item);
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

struct DataBuffer
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
	using LoadProc = void(RPGStats* self, ObjectSet<STDString>* paths);

	struct VMTMappings
	{
		bool VMTsMapped{ false };
		void* ObjectVMT{ nullptr };
		Functors::BaseVMT* StatsFunctorSetVMT{ nullptr };
		std::unordered_map<StatsFunctorActionId, Functor::FunctorVMT*> FunctorVMTs;

		void Update();
	};

	static VMTMappings sVMTMappings;

	CNamedElementManager<RPGEnumeration> ModifierValueLists;
	CNamedElementManager<ModifierList> ModifierLists;
	CNamedElementManager<Object> Objects;
	CNamedElementManager<TreasureCategory> TreasureCategories;
	CNamedElementManager<TreasureTable> TreasureTables;
	ItemTypeManager ItemTypes;
	Map<FixedString, Functors*> StatsFunctors;
	uint64_t Unkn1[5];
	Map<FixedString, float>* ExtraData;
	Map<FixedString, FixedString> field_290;
	Map<FixedString, void*> field_2A8;
	Map<FixedString, void*> field_2C0;
	Map<FixedString, void*> field_2D8;
	RefMap<FixedString, void*> field_2F0;
	FixedString TreasureRarities[7];
	Array<FixedString> FixedStrings;
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
	Map<FixedString, int32_t> DataBufferIndices;
	Array<DataBuffer*> DataBuffers;
	Map<FixedString, BloodType*> BloodTypes;
	bool field_3E0;
	bool field_3E1;
	bool IsLoading;
	bool field_3E3;
	bool Loaded;
	uint32_t FastLock;
	Array<STDString> Conditions;

	Modifier * GetModifierInfo(FixedString const& modifierListName, FixedString const& modifierName);
	ModifierList * GetTypeInfo(Object * object);

	bool ObjectExists(FixedString const& statsId, FixedString const& type);
	std::optional<Object*> CreateObject(FixedString const& name, FixedString const& type);
	std::optional<Object*> CreateObject(FixedString const& name, int32_t modifierListIndex);
	Functors* ConstructFunctorSet(FixedString const& propertyName);
	Functor* ConstructFunctor(StatsFunctorActionId action);
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
	std::optional<STDString*> GetConditions(int conditionsId);
	int GetOrCreateConditions(STDString const& conditions);

	std::optional<int> EnumLabelToIndex(FixedString const& enumName, char const* enumLabel);
	FixedString EnumIndexToLabel(FixedString const& enumName, int index);
};

Object * StatFindObject(char const * name);
Object * StatFindObject(int index);

END_NS()
