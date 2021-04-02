#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Stats/Common.h>
#include <GameDefinitions/Stats/Character.h>
#include <GameDefinitions/Stats/Item.h>
#include <GameDefinitions/Stats/Functors.h>

namespace bg3se
{
	struct IGameObject;
	struct SpellPrototype;

	/*
	typedef void (* eoc__SpellPrototypeManager__Init)(void * self);
	*/

	struct CDivinityStats : public ProtectedGameObject<CDivinityStats>
	{
		void * VMT;
		bool SomeFlag;
		CDivinityStatsList<CDivinityStats_Item> * ItemList;
		CDivinityStatsList<CDivinityStats_Character> * CharacterList;
		char SomeBuf0x50b[80];
		CDivinityStats_Item * DefaultWeaponStats;
		ObjectSet<FixedString> FSSetUnkn;
		uint64_t Unkn;
	};

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

	struct CRPGStats_Modifier : public ProtectedGameObject<CRPGStats_Modifier>
	{
		int32_t RPGEnumerationIndex;
		int32_t LevelMapIndex;
		int32_t UnknownZero;
		FixedString Name;
	};

	struct ModifierList
	{
		CNamedElementManager<CRPGStats_Modifier> Attributes;
		FixedString Name;

		CRPGStats_Modifier * GetAttributeInfo(FixedString const& name, int * attributeIndex) const;
	};

	struct CRPGStats_ExtraData : public ProtectedGameObject<CRPGStats_ExtraData>
	{
		Map<FixedString, float> Properties;
	};
	/*
	struct CDivinityStats_Condition
	{
		void * ScriptCheckBlock;
		FixedString Name;
	};

	struct CRPGStats_Conditions_Manager
	{
		Map<FixedString, CDivinityStats_Condition *> Conditions;
		ObjectSet<STDString> Variables;
	};
	*/
	struct CRPGStats_ItemType_Manager : public CNamedElementManager<uint64_t>
	{
		uint64_t Unknown;
	};

	struct SpellPrototype : public ProtectedGameObject<SpellPrototype>
	{
		/*using FormatDescriptionParamProc = void(SkillPrototype* skillPrototype, CDivinityStats_Character* tgtCharStats,
			eoc::Text* eocText, int paramIndex, bool isFromItem, float xmm9_4_0, FixedString* paramText,
			ObjectSet<STDString>* stdStringSet);

		using GetSkillDamageProc = void(SkillPrototype* self, struct DamagePairList* damageList,
			CRPGStats_ObjectInstance* attackerStats, bool isFromItem, bool stealthed, float* attackerPosition,
			float* targetPosition, DeathType* pDeathType, int level, bool noRandomization);*/

		void* VMT;
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

		CRPGStats_Object* GetStats() const;
	};

	struct SpellIdWithPrototype
	{
		SomeGuidId SpellId;
		SpellPrototype* SpellProto;
	};

	struct SpellPrototypeManager : public ProtectedGameObject<SpellPrototypeManager>
	{
		void* VMT;
		VirtualMultiHashMap<FixedString, SpellPrototype*> Spells;
		Array<FixedString> SpellNames;
		bool Initialized;

		/*void SyncSpellStat(CRPGStats_Object* object);
		void SyncSpellStat(CRPGStats_Object* object, SpellPrototype* proto);*/
	};


	struct StatusPrototype : public ProtectedGameObject<StatusPrototype>
	{
		/*using FormatDescriptionParamProc = void (StatusPrototype *prototype, CRPGStats_ObjectInstance* owner,
			CRPGStats_ObjectInstance* statusSource, float multiplier, eoc::Text * text, int paramIndex,
			FixedString * param, ObjectSet<STDString> * paramSet);*/

		void* VMT{ nullptr };
		int StatsObjectIndex{ -1 };
		StatusType StatusId;
		FixedString StatusName;
		int StatusPropertyFlags;
		int StatusGroups;
		DescriptionInfo Description;
		uint8_t LEDEffect;
		uint8_t TickType;
		uint8_t Flags;
		ObjectSet<SurfaceType>* AbsorbSurfaceTypes{ nullptr };
		Array<void*> Boosts;
		int16_t RemoveEvents;

		CRPGStats_Object* GetStats() const;
	};

	struct StatusPrototypeManager : public ProtectedGameObject<StatusPrototypeManager>
	{
		void* VMT;
		Map<FixedString, StatusPrototype*> Prototypes;
		Array<FixedString> PrototypeNames;
		bool Initialized;

		// void SyncStatusStat(CRPGStats_Object* object);
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

	// FIXME - most likely unused in BG3
	struct CRPGStats_DeltaModifier : Noncopyable<CRPGStats_DeltaModifier>
	{
		ObjectSet<int> BoostIndices;
		ObjectSet<int> BoostCounts;
		FixedString ModifierType;
		ItemSlot32 SlotType{ ItemSlot32::Sentinel };
		WeaponType WeaponType{ WeaponType::Sentinel };
		ArmorType ArmorType{ ArmorType::Sentinel };
		HandednessType Handedness{ HandednessType::Any };
		FixedString Name;
		FixedString BoostType;
		int MinLevel{ -1 };
		int MaxLevel{ -1 };
		int MinLevelBoosted{ -1 };
		int MaxLevelBoosted{ -1 };
		int Frequency{ 1 };
		int Frequency2{ 1 };
	};

	struct CRPGStats_DeltaModifier_List : public CNamedElementManager<CRPGStats_DeltaModifier>
	{};

	struct CRPGStats_Treasure_SubTable_Description
	{
		struct Category
		{
			int Index;
			int Frequency;
			uint16_t Frequencies[7];
			uint16_t Frequencies2[7];
			bool IsTreasureTable;
			bool IsTreasureTable2;
		};

		struct DropCount
		{
			int Chance;
			int Amount;
		};

		ObjectSet<Category*> Categories;
		ObjectSet<int> CategoryFrequencies;
		int TotalFrequency;
		ObjectSet<DropCount> DropCounts;
		ObjectSet<int> Amounts;
		int TotalCount;
		int StartLevel;
		int EndLevel;
	};


	struct CRPGStats_Treasure_Table
	{
		FixedString Name;
		int MinLevel;
		int MaxLevel;
		bool IgnoreLevelDiff;
		bool UseTreasureGroupContainers;
		bool CanMerge;
		ObjectSet<CRPGStats_Treasure_SubTable_Description*> SubTables;
	};

	struct CRPGStats_Treasure_Category
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


	struct CItemCombinationProperty
	{
		struct ItemCombinationPropertyEntry
		{
			FixedString ObjectId;
			IngredientType IngredientType;
			FixedString Result;
		};


		FixedString Name;
		FixedString PreviewIcon;
		FixedString PreviewTooltip;
		ObjectSet<ItemCombinationPropertyEntry> Entries;
	};


	struct CItemCombinationPreviewData
	{
		FixedString Name;
		FixedString Type;
		FixedString StatsId;
		FixedString Tooltip;
		FixedString Icon;
	};


	struct CItemCombinationIngredient
	{
		FixedString Object;
		IngredientType IngredientType;
		IngredientTransformType Transform;
		ItemDataRarity ItemRarity;
	};


	struct CItemCombinationResult
	{
		struct CombinationResult
		{
			FixedString Result;
			FixedString Boost;
			int ResultAmount;
		};


		ObjectSet<CombinationResult> Results;
		AbilityType Requirement;
		int ReqLevel;
		FixedString PreviewStatsId;
		FixedString PreviewIcon;
		FixedString PreviewTooltip;
		FixedString Name;
	};


	struct CItemCombination
	{
		FixedString Name;
		ObjectSet<CItemCombinationIngredient> Ingredients;
		ObjectSet<CItemCombinationResult> Results;
		RecipeCategory RecipeCategory;
		CraftingStationType CraftingStation;
		bool AutoLevel;
	};


	struct CItemCombinationManager : public CNamedElementManager<CItemCombination>
	{
		CItemCombination* CurrentItemCombo;
		CItemCombinationPreviewData* CurrentPreviewData;
		CItemCombinationProperty* CurrentComboProperties;
		RefMap<FixedString, CItemCombinationPreviewData*> PreviewData;
		RefMap<FixedString, CItemCombinationProperty*> ComboProperties;
	};


	MARK_ALLOCATABLE(CSkillSet);
	MARK_ALLOCATABLE(CEquipmentGroup);
	MARK_ALLOCATABLE(CEquipmentSet);*/
	MARK_ALLOCATABLE(CRPGStats_Treasure_SubTable_Description);
	MARK_ALLOCATABLE(CRPGStats_Treasure_SubTable_Description::Category);
	MARK_ALLOCATABLE(CRPGStats_Treasure_Table);
	MARK_ALLOCATABLE(CRPGStats_Treasure_Category);
	MARK_ALLOCATABLE(CRPGStats_Treasure_Category::Item);
	/*MARK_ALLOCATABLE(CNameGroupName);
	MARK_ALLOCATABLE(CNameGroup);
	MARK_ALLOCATABLE(CNameGroupLink);
	MARK_ALLOCATABLE(CRootGroup);
	MARK_ALLOCATABLE(CLevelGroup);
	MARK_ALLOCATABLE(CItemGroup);
	MARK_ALLOCATABLE(CItemCombinationProperty);
	MARK_ALLOCATABLE(CItemCombinationPreviewData);
	MARK_ALLOCATABLE(CItemCombinationIngredient);
	MARK_ALLOCATABLE(CItemCombinationResult);
	MARK_ALLOCATABLE(CItemCombinationResult::CombinationResult);
	MARK_ALLOCATABLE(CItemCombination);


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

	struct DataBuffer {};

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
		//typedef void (*LoadProc)(RPGStats* self);

		struct VMTMappings
		{
			bool VMTsMapped{ false };
			void* ObjectVMT{ nullptr };
			StatsFunctorSet::BaseVMT* StatsFunctorSetVMT{ nullptr };
			std::unordered_map<StatsFunctorActionId, StatsFunctorBase::FunctorVMT*> FunctorVMTs;

			void Update();
		};

		static VMTMappings sVMTMappings;

		CNamedElementManager<RPGEnumeration> ModifierValueLists;
		CNamedElementManager<ModifierList> ModifierLists;
		CNamedElementManager<CRPGStats_Object> Objects;
		CNamedElementManager<CRPGStats_LevelMap> LevelMaps;
		CNamedElementManager<CRPGStats_DeltaModifier_List> DeltaMods;
		CNamedElementManager<CRPGStats_Treasure_Category> TreasureCategories;
		CNamedElementManager<CRPGStats_Treasure_Table> TreasureTables;
		CRPGStats_ItemType_Manager ItemTypes;
		Map<FixedString, StatsFunctorSet*> StatsFunctors;
		uint64_t Unkn1[4];
		RNG Rng;
		Map<FixedString, float>* ExtraData;
		RefMap<FixedString, ObjectSet<CRPGStats_Requirement>> Requirements;
		RefMap<FixedString, ItemColor> ItemColors;
		Map<FixedString, void*> TreasureGroups[4];
		RefMap<FixedString, void*> Unknown5;
		FixedString TreasureRarities[7];
		ObjectSet<FixedString> FixedStrings;
		ObjectSet<int64_t*> Int64s;
		Array<UUID> GUIDs;
		Array<float> ConstantFloats;
		void* EquipmentSetManager;
		void* SpellSetManager;
		void* ItemProgressionManager;
		void* ItemCombinationManager;
		void* CurrentDataBuffer;
		FixedString CurrentDataBufferPath;
		Map<FixedString, int32_t> DataBufferIndices;
		ObjectSet<DataBuffer*> DataBuffers;
		Map<FixedString, BloodType*> BloodTypes;
		CDivinityStats* DivinityStats;
		CRITICAL_SECTION CriticalSection;
		void* Unkn5;
		Array<STDString> ConditionList;

		CRPGStats_Modifier * GetModifierInfo(FixedString const& modifierListName, FixedString const& modifierName);
		ModifierList * GetTypeInfo(CRPGStats_Object * object);

		bool ObjectExists(FixedString const& statsId, FixedString const& type);
		std::optional<CRPGStats_Object*> CreateObject(FixedString const& name, FixedString const& type);
		std::optional<CRPGStats_Object*> CreateObject(FixedString const& name, int32_t modifierListIndex);
		StatsFunctorSet* ConstructFunctorSet(FixedString const& propertyName);
		StatsFunctorBase* ConstructFunctor(StatsFunctorActionId action);
		/*void SyncObjectFromServer(MsgS2CSyncStat const& msg);
		void SyncWithPrototypeManager(CRPGStats_Object* object);
		void BroadcastSyncAll();*/

		std::optional<FixedString*> GetFixedString(int stringId);
		FixedString* GetOrCreateFixedString(int& stringId);
		std::optional<int64_t*> GetInt64(int int64Id);
		int64_t* GetOrCreateInt64(int& int64Id);
		std::optional<float*> GetFloat(int floatId);
		float* GetOrCreateFloat(int& floatId);
		std::optional<UUID*> GetGuid(int guidId);
		UUID* GetOrCreateGuid(int& guidId);
		std::optional<STDString*> GetConditions(int conditionsId);
		STDString* GetOrCreateConditions(int& conditionsId);

		std::optional<int> EnumLabelToIndex(FixedString const& enumName, char const* enumLabel);
		FixedString EnumIndexToLabel(FixedString const& enumName, int index);
	};

	typedef void* (*ScriptCheckBlock__Build)(STDString const& str, ObjectSet<STDString> const& variables, int offset, int length);

	CRPGStats_Object * StatFindObject(char const * name);
	CRPGStats_Object * StatFindObject(int index);

	/*
	template <class TTag>
	std::optional<int32_t> CharacterStatGetter(CDivinityStats_Character__GetStat * getter,
		WrappableFunction<TTag, CDivinityStats_Character__GetStat> & wrapper,
		CDivinityStats_Character * character, bool original, bool baseStats)
	{
		if (getter == nullptr) {
			return {};
		}

		if (original || !wrapper.IsWrapped()) {
			return getter(character, baseStats);
		} else {
			return wrapper.CallWithHooks(character, baseStats);
		}
	}

	template <class TTag>
	std::optional<int32_t> CharacterStatGetter(CDivinityStats_Character__GetStatNoBoosts * getter,
		WrappableFunction<TTag, CDivinityStats_Character__GetStatNoBoosts> & wrapper,
		CDivinityStats_Character * character, bool original, bool baseStats)
	{
		if (getter == nullptr) {
			return {};
		}

		if (original || !wrapper.IsWrapped()) {
			return getter(character);
		} else {
			return wrapper.CallWithHooks(character);
		}
	}

	template <class TTag>
	std::optional<int32_t> CharacterStatGetter(CDivinityStats_Character__GetStatWithBoosts * getter,
		WrappableFunction<TTag, CDivinityStats_Character__GetStatWithBoosts> & wrapper,
		CDivinityStats_Character * character, bool original, bool baseStats)
	{
		if (getter == nullptr) {
			return {};
		}

		if (original || !wrapper.IsWrapped()) {
			return getter(character, baseStats, false);
		} else {
			return wrapper.CallWithHooks(character, baseStats, false);
		}
	}

	template <class TTag>
	std::optional<int32_t> CharacterStatGetter(CDivinityStats_Character__GetStatWithInit * getter,
		WrappableFunction<TTag, CDivinityStats_Character__GetStatWithInit> & wrapper,
		CDivinityStats_Character * character, bool original, bool baseStats)
	{
		if (getter == nullptr) {
			return {};
		}

		if (original || !wrapper.IsWrapped()) {
			return getter(character, baseStats, 0);
		} else {
			return wrapper.CallWithHooks(character, baseStats, 0);
		}
	}

	template <class TTag>
	std::optional<int32_t> CharacterStatGetter(CDivinityStats_Character__GetStatHearing * getter,
		WrappableFunction<TTag, CDivinityStats_Character__GetStatHearing> & wrapper,
		CDivinityStats_Character * character, bool original, bool baseStats)
	{
		if (getter == nullptr) {
			return {};
		}

		if (original || !wrapper.IsWrapped()) {
			return (int)(getter(character, nullptr, baseStats) * 100.0f);
		} else {
			return (int)(wrapper.CallWithHooks(character, nullptr, baseStats) * 100.0f);
		}
	}

	struct CharacterStatsGetters
	{

		union {
			void * Ptrs[26];
			struct {
#define DEFN_GETTER(type, name) CDivinityStats_Character__Get##type * Get##name;
#include <GameDefinitions/CharacterGetters.inl>
#undef DEFN_GETTER

				CDivinityStats_Character__GetHitChance * GetHitChance;
				CDivinityStats_Character__GetTalent * GetTalent;
				CDivinityStats_Character__GetAbility * GetAbility;
			};
		};

#define DEFN_GETTER(type, name) \
	enum class name##Tag{}; \
	WrappableFunction<name##Tag, CDivinityStats_Character__Get##type> Wrapper##name;

#include <GameDefinitions/CharacterGetters.inl>
#undef DEFN_GETTER

		enum class HitChanceTag {}; \
		WrappableFunction<HitChanceTag, CDivinityStats_Character__GetHitChance> WrapperHitChance;

		bool Wrapped{ false };

		void WrapAll();
		void UnwrapAll();

		std::optional<int32_t> GetStat(CDivinityStats_Character * character, FixedString const& name, 
			bool original, bool baseValues);
	};

	namespace esv
	{
		using ExecutePropertyDataOnGroundHitProc = void(glm::vec3* position, uint64_t casterHandle, DamagePairList* damageList, CRPGStats_Object_Property_List* propertyList, DamageType damageType);
		using ExecuteCharacterSetExtraPropertiesProc = void(CRPGStats_Object_Property_List* extraProperties, uint64_t attackerHandle,
			ObjectSet<esv::Character*> const& targets, glm::vec3 const& impactOrigin, CRPGStats_Object_PropertyContext propertyContext,
			bool isFromItem, SkillPrototype* skillProto, HitDamageInfo* damageInfo, float statusStartTimer, esv::Character* refTarget, 
			bool statusFlag0x40, float a12);
		using ExecutePropertyDataOnPositionOnlyProc = void(CRPGStats_Object_Property_List* properties, uint64_t attackerHandle,
			glm::vec3 const* position, float areaRadius, CRPGStats_Object_PropertyContext propertyContext, bool isFromItem, 
			SkillPrototype* skillPrototype, HitDamageInfo* damageInfo);
	}*/
}
