#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/RootTemplates.h>

BEGIN_SE()

struct GenericPropertyTag
{
	uint8_t Type;
	Guid Entity;
};

struct ArmorComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Armor;
	static constexpr auto EngineClass = "eoc::ArmorComponent";

	int ArmorType;
	int ArmorClass;
	int AbilityModifierCap;
	uint8_t ArmorClassAbility;
	uint8_t Shield;
};

struct ArmorSetStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ArmorSetState;
	static constexpr auto EngineClass = "eoc::armor_set::StateComponent";

	uint8_t State;
};

struct BaseHpComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::BaseHp;
	static constexpr auto EngineClass = "eoc::BaseHpComponent";

	int Vitality;
	int VitalityBoost;
};

struct Bound
{
	EntityHandle Entity;
	RefMap<uint16_t, AIBound> AIBounds;
	[[bg3::hidden]] void* SomeContainer;
	[[bg3::hidden]] void* GameObject;
	MultiHashSet<uint32_t> field_28;
	glm::vec3 Translate;
	glm::quat RotationQuat;
	float Scale;
	int16_t field_78;
	uint16_t SomeRefCount;
	glm::vec3 TranslateOverride;
	uint8_t Flags;
	uint8_t field_89;
	uint16_t Flags2;
	FixedString field_8C;
};

struct BoundComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Bound;
	static constexpr auto EngineClass = "eoc::BoundComponent";

	Bound* Bound;
};

struct CustomStatsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CustomStats;
	static constexpr auto EngineClass = "eoc::CustomStatsComponent";

	Map<FixedString, int> Stats;
};

struct DataComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Data;
	static constexpr auto EngineClass = "eoc::DataComponent";

	int32_t Weight;
	FixedString StatsId;
	uint32_t StepsType;
};

struct DetachedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Detached;
	static constexpr auto EngineClass = "eoc::DetachedComponent";

	uint32_t Flags;
};


struct ExperienceComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Experience;
	static constexpr auto EngineClass = "eoc::exp::ExperienceComponent";

	int CurrentLevelExperience;
	int NextLevelExperience;
	int TotalExperience;
	int SomeExperience;
	uint8_t field_28;
};

struct HealthComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Health;
	static constexpr auto EngineClass = "eoc::HealthComponent";

	int Hp;
	int MaxHp;
	int TemporaryHp;
	int MaxTemporaryHp;
	Guid field_10;
	uint8_t field_20;
};

struct ResistancesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Resistances;
	static constexpr auto EngineClass = "eoc::ResistancesComponent";

	std::array<ResistanceBoostFlags, 14> Resistances;
	uint8_t field_E;
	int AC;
	std::array<uint16_t, 14> PerDamageTypeHealthThresholds;
	std::array<uint32_t, 14> PerDamageTypeHealthThresholds2;
};

struct DifficultyCheckComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DifficultyCheck;
	static constexpr auto EngineClass = "eoc::DifficultyCheckComponent";

	int SpellDC;
	int field_4;
	int field_8;
};

struct AttributeFlagsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::AttributeFlags;
	static constexpr auto EngineClass = "eoc::AttributeFlagsComponent";

	uint32_t AttributeFlags;
};

struct BodyTypeComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::BodyType;
	static constexpr auto EngineClass = "eoc::BodyTypeComponent";

	uint8_t BodyType;
	uint8_t BodyType2;
};

struct ObjectSizeComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ObjectSize;
	static constexpr auto EngineClass = "eoc::ObjectSizeComponent";

	uint8_t Size;
	[[bg3::legacy(field_1)]] uint8_t SoundSize;
};

struct BaseStatsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::BaseStats;
	static constexpr auto EngineClass = "eoc::BaseStatsComponent";

	std::array<int, 7> BaseAbilities;
};

struct ExpertiseComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Expertise;
	static constexpr auto EngineClass = "eoc::expertise::ExpertiseComponent";

	MultiHashSet<SkillId> Expertise;
};

struct StatsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Stats;
	static constexpr auto EngineClass = "eoc::StatsComponent";

	int InitiativeBonus;
	std::array<int, 7> Abilities;
	std::array<int, 7> AbilityModifiers;
	std::array<int, 18> Skills;
	int ProficiencyBonus;
	AbilityId SpellCastingAbility;
	int field_8C;
	int field_90;
	int ArmorType;
	int ArmorType2;
	AbilityId UnarmedAttackAbility;
	AbilityId RangedAttackAbility;
};

struct StatusImmunitiesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::StatusImmunities;
	static constexpr auto EngineClass = "eoc::StatusImmunitiesComponent";

	MultiHashMap<FixedString, Guid> PersonalStatusImmunities;
};

struct ValueComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Value;
	static constexpr auto EngineClass = "eoc::ValueComponent";

	int Value;
	uint8_t Rarity;
	bool Unique;
};

struct WeaponComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Weapon;
	static constexpr auto EngineClass = "eoc::WeaponComponent";

	struct Element1 : public StatsExpressionParam
	{
		uint8_t field_28;
		BoostCause Cause;
		TranslatedString field_48;
		uint8_t field_58;
	};


	RefMap<AbilityId, Array<DiceValues>> Rolls;
	RefMap<AbilityId, Array<DiceValues>> Rolls2;
	float WeaponRange;
	float DamageRange;
	[[bg3::hidden]]
	void* WeaponFunctors;
	uint32_t WeaponProperties;
	uint8_t WeaponGroup;
	AbilityId Ability;
	Array<Element1> field_38;
	DiceSizeId DamageDice;
	DiceSizeId VersatileDamageDice;
};

struct BackgroundComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Background;
	static constexpr auto EngineClass = "eoc::BackgroundComponent";

	Guid Background;
};
	
struct GodComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::God;
	static constexpr auto EngineClass = "eoc::god::GodComponent";

	Guid God;
};
	
struct ProficiencyComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Proficiency;
	static constexpr auto EngineClass = "eoc::stats::proficiency::ProficiencyComponent";

	__int64 field_0;
};
	
struct ProficiencyGroupComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyGroup;
	static constexpr auto EngineClass = "eoc::stats::proficiency::ProficiencyGroupComponent";

	__int64 field_0;
};


struct LevelUpUpgrades
{
	struct Reference
	{
		[[bg3::legacy(field_0)]] uint8_t Type;
		Guid Class;
		Guid Subclass;
		[[bg3::legacy(field_28)]] int Level;
		[[bg3::legacy(field_2C)]] bool Multiclass;
	};

	struct FeatData : public LevelUpUpgrades::Reference
	{
		int field_30;
		Guid Feat;
		Array<uint8_t> Array_b8;
		STDString FeatName;
		int field_80;
	};

	struct SkillData : public LevelUpUpgrades::Reference
	{
		int field_30;
		Guid Skill;
		[[bg3::legacy(Array_b8)]] Array<SkillId> Proficiencies;
		STDString field_60;
		int field_80;
	};
		
	struct SkillBonusData : public LevelUpUpgrades::Reference
	{
		int field_30;
		[[bg3::legacy(field_38)]] Guid Skill;
		uint8_t field_48;
		[[bg3::legacy(Array_b8)]] Array<SkillId> Expertise;
		STDString field_60;
		int field_80;
	};
		
	struct AbilityBonusData : public LevelUpUpgrades::Reference
	{
		int field_30;
		Guid AbilityBonus;
		[[bg3::legacy(Array_b8)]] Array<AbilityId> Bonuses;
		[[bg3::legacy(Array_i32)]] Array<uint32_t> BonusAmounts;
		[[bg3::legacy(field_60)]] STDString BonusType;
		int field_80;
	};
		
	struct SpellData : public LevelUpUpgrades::Reference
	{
		struct StringPair
		{
			FixedString From;
			FixedString To;
		};

		int field_30;
		Guid SpellList;
		Array<FixedString> Spells;
		[[bg3::legacy(Array_FS2)]] Array<StringPair> ReplaceSpells;
		[[bg3::legacy(field_78)]] STDString SelectorId;
	};
		
	struct PassiveData : public LevelUpUpgrades::Reference
	{
		struct StringPair
		{
			FixedString From;
			FixedString To;
		};

		int field_30;
		[[bg3::legacy(SpellList)]] Guid PassiveList;
		[[bg3::legacy(Spells)]] Array<FixedString> Passives;
		[[bg3::legacy(Array_FS2)]] Array<StringPair> ReplacePassives;
		[[bg3::legacy(field_78)]] STDString SelectorId;
		int field_80;
	};
		
	struct EquipmentData : public LevelUpUpgrades::Reference
	{
		int field_30;
		[[bg3::legacy(field_38)]] Guid EquipmentList;
		[[bg3::legacy(Array_FS)]] Array<FixedString> Equipment;
		[[bg3::legacy(field_60)]] STDString SelectorId;
		int field_80;
	};

	Array<FeatData> Feats;
	Array<AbilityBonusData> AbilityBonuses;
	Array<SkillData> Skills;
	[[bg3::legacy(Unknowns2)]] Array<SkillBonusData> SkillBonuses;
	Array<SpellData> Spells;
	[[bg3::legacy(Spells2)]] Array<PassiveData> Passives;
	[[bg3::legacy(Unknowns4)]] Array<EquipmentData> Equipment;
};

struct LevelUpData
{
	Guid Class;
	Guid SubClass;
	Guid Feat;
	Guid AccessorySet;
	std::array<int, 7> Abilities;
	LevelUpUpgrades Upgrades;
	[[bg3::legacy(field_B0)]] Array<SpellIdBase> Spells;
};

struct FloatingComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Floating;
	static constexpr auto EngineClass = "eoc::FloatingComponent";

	int field_18;
	int field_1C;
};

struct VoiceComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Voice;
	static constexpr auto EngineClass = "eoc::VoiceComponent";

	Guid Voice;
};

struct CharacterCreationStatsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CharacterCreationStats;
	static constexpr auto EngineClass = "eoc::CharacterCreationStatsComponent";

	Guid Race;
	Guid SubRace;
	uint8_t BodyType;
	uint8_t BodyShape;
	STDString Name;
	std::array<int32_t, 7> Abilities;
	uint8_t field_5C;
};

struct DisarmableComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Disarmable;
	static constexpr auto EngineClass = "eoc::DisarmableComponent";

	Guid field_0;
	uint8_t field_10;
	uint8_t field_11;
};

struct ShortRestComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ShortRest;
	static constexpr auto EngineClass = "eoc::rest::ShortRestComponent";

	uint8_t StateId;
};

struct IsSummonComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::IsSummon;
	static constexpr auto EngineClass = "eoc::summon::IsSummonComponent";

	EntityHandle Owner_M;
};

struct SummonContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SummonContainer;
	static constexpr auto EngineClass = "eoc::summon::ContainerComponent";

	[[bg3::legacy(field_18)]] MultiHashMap<FixedString, Array<EntityHandle>> ByTag;
	MultiHashSet<EntityHandle> Characters;
	MultiHashSet<EntityHandle> Items;
};

struct StealthComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Stealth;
	static constexpr auto EngineClass = "eoc::StealthComponent";

	bool SeekHiddenFlag;
	glm::vec3 Position;
	float SeekHiddenTimeout;
	int field_2C;
};

DEFINE_TAG_COMPONENT(eoc, ClientControlComponent, ClientControl)
DEFINE_TAG_COMPONENT(ls, IsGlobalComponent, IsGlobal)
DEFINE_TAG_COMPONENT(ls, SavegameComponent, Savegame)

struct DisabledEquipmentComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DisabledEquipment;
	static constexpr auto EngineClass = "eoc::DisabledEquipmentComponent";

	bool ShapeshiftFlag;
};

struct LootingStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::LootingState;
	static constexpr auto EngineClass = "eoc::LootingStateComponent";

	EntityHandle Looter_M;
	uint8_t State;
	int field_24;
};

struct LootComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Loot;
	static constexpr auto EngineClass = "eoc::LootComponent";

	uint8_t Flags;
	uint8_t InventoryType;
};

struct LockComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Lock;
	static constexpr auto EngineClass = "eoc::lock::LockComponent";

	FixedString Key_M;
	int LockDC;
	Guid field_8;
	Array<Guid> field_18;
};

struct KeyComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Key;
	static constexpr auto EngineClass = "eoc::lock::KeyComponent";

	FixedString Key;
};

struct SummonLifetimeComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SummonLifetime;
	static constexpr auto EngineClass = "eoc::summon::LifetimeComponent";

	std::variant<uint8_t, float> Lifetime;
};

struct HotbarContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::HotbarContainer;
	static constexpr auto EngineClass = "eoc::hotbar::ContainerComponent";

	struct Element
	{
		EntityHandle Item;
		SpellId SpellId;
		FixedString Passive;
		uint32_t Slot;
		bool IsNew;
	};

	struct Bar
	{
		uint8_t Index;
		uint8_t field_1;
		Array<Element> Elements;
		uint8_t Width;
		uint32_t Height;
		STDString field_20;
	};

	MultiHashMap<FixedString, Array<Bar>> Containers;
	FixedString ActiveContainer;
};

struct HotbarCurrentDecksComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::HotbarDecks;
	static constexpr auto EngineClass = "eoc::hotbar::CurrentDecksComponent";

	MultiHashMap<FixedString, int32_t> Decks;
};

struct OriginTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::OriginTag;
	static constexpr auto EngineClass = "eoc::OriginTagComponent";

	Array<Guid> Tags;
};

struct PassiveInfo
{
	uint32_t field_0;
	uint32_t Passive;
};

struct OriginPassivesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::OriginPassives;
	static constexpr auto EngineClass = "eoc::OriginPassivesComponent";

	Array<PassiveInfo> field_18;
};

struct ClassTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ClassTag;
	static constexpr auto EngineClass = "eoc::ClassTagComponent";

	Array<Guid> Tags;
};

struct BackgroundTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::BackgroundTag;
	static constexpr auto EngineClass = "eoc::BackgroundTagComponent";

	Array<Guid> Tags;
};

struct OriginAppearanceTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::OriginAppearanceTag;
	static constexpr auto EngineClass = "eoc::OriginAppearanceTagComponent";

	Array<Guid> Tags;
};

struct VoiceTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::VoiceTag;
	static constexpr auto EngineClass = "eoc::VoiceTagComponent";

	Array<Guid> Tags;
};

struct GodTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::GodTag;
	static constexpr auto EngineClass = "eoc::god::TagComponent";

	Array<Guid> Tags;
};

struct BackgroundPassivesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::BackgroundPassives;
	static constexpr auto EngineClass = "eoc::BackgroundPassivesComponent";

	Array<PassiveInfo> field_18;
};

struct GlobalShortRestDisabledComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::GlobalShortRestDisabled;
	static constexpr auto EngineClass = "eoc::GlobalShortRestDisabledComponent";
};

struct GlobalLongRestDisabledComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::GlobalLongRestDisabled;
	static constexpr auto EngineClass = "eoc::GlobalLongRestDisabledComponent";
};

struct StoryShortRestDisabledComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::StoryShortRestDisabled;
	static constexpr auto EngineClass = "eoc::StoryShortRestDisabledComponent";
};

struct FleeCapabilityComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::FleeCapability;
	static constexpr auto EngineClass = "eoc::FleeCapabilityComponent";

	FleeErrorFlags Flags;
	[[bg3::legacy(field_1C)]] float FleeDistance;
	[[bg3::legacy(field_20)]] float CurrentFleeDistance;
};

struct CanDoRestComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanDoRest;
	static constexpr auto EngineClass = "eoc::CanDoRestComponent";

	RestFlags Flags;
	RestErrorFlags RestErrorFlags1;
	RestErrorFlags RestErrorFlags2;
};

struct CanModifyHealthComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanModifyHealth;
	static constexpr auto EngineClass = "eoc::CanModifyHealthComponent";
		
	uint16_t Flags;
};

struct AvailableLevelComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::AvailableLevel;
	static constexpr auto EngineClass = "eoc::exp::AvailableLevelComponent";

	int Level;
};

struct CanBeLootedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanBeLooted;
	static constexpr auto EngineClass = "eoc::CanBeLootedComponent";

	uint16_t Flags;
};

struct CanBeDisarmedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanBeDisarmed;
	static constexpr auto EngineClass = "eoc::CanBeDisarmedComponent";

	uint16_t Flags;
};

DEFINE_TAG_COMPONENT(eoc, CanTriggerRandomCastsComponent, CanTriggerRandomCasts)

struct CanDoActionsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanDoActions;
	static constexpr auto EngineClass = "eoc::CanDoActionsComponent";

	uint16_t Flags;
};

struct ActionUseConditionsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ActionUseConditions;
	static constexpr auto EngineClass = "eoc::action::ActionUseConditionsComponent";

	Array<int32_t> Conditions;
};


struct CanMoveComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanMove;
	static constexpr auto EngineClass = "eoc::CanMoveComponent";

	uint16_t Flags;
};

struct CanSenseComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanSense;
	static constexpr auto EngineClass = "eoc::CanSenseComponent";

	uint16_t Flags;
};

struct ConcentrationComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Concentration;
	static constexpr auto EngineClass = "eoc::concentration::ConcentrationComponent";

	struct TargetInfo
	{
		[[bg3::legacy(field_0)]] EntityHandle Target;
		[[bg3::legacy(field_8)]] EntityHandle Concentration;
		[[bg3::legacy(field_10)]] EntityHandle Status;
		int16_t field_18;
		bool field_1A;
	};

	[[bg3::legacy(field_0)]] EntityHandle Caster;
	[[bg3::legacy(field_8)]] Array<TargetInfo> Targets;
	SpellId SpellId;
};

struct DarknessComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Darkness;
	static constexpr auto EngineClass = "eoc::DarknessComponent";

	uint8_t Sneaking;
	uint8_t Obscurity;
	uint8_t field_2;
	uint8_t field_3;
	uint8_t field_4;
	float field_8;
	float field_C;
};

struct DualWieldingComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DualWielding;
	static constexpr auto EngineClass = "eoc::DualWieldingComponent";

	bool Melee;
	bool RangedUI;
	uint8_t field_1A;
	bool Ranged;
	bool MeleeUI;
	uint8_t field_1D;
	bool DisableDualWielding;
};

DEFINE_TAG_COMPONENT(eoc, GravityDisabledComponent, GravityDisabled)
DEFINE_TAG_COMPONENT(eoc, GravityDisabledUntilMovedComponent, GravityDisabledUntilMoved)
DEFINE_TAG_COMPONENT(eoc::improvised_weapon, CanBeWieldedComponent, CanBeWielded)
DEFINE_TAG_COMPONENT(eoc::tag, AvatarComponent, Avatar)
DEFINE_TAG_COMPONENT(eoc::tag, HasExclamationDialogComponent, HasExclamationDialog)
DEFINE_TAG_COMPONENT(eoc::tag, TraderComponent, Trader)
DEFINE_TAG_COMPONENT(eoc::ambush, AmbushingComponent, Ambushing)

struct InteractionFilterComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InteractionFilter;
	static constexpr auto EngineClass = "eoc::InteractionFilterComponent";

	MultiHashSet<Guid> field_0;
	uint8_t field_30;
	uint8_t field_31;
};

struct InvisibilityComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Invisibility;
	static constexpr auto EngineClass = "eoc::InvisibilityComponent";

	uint8_t field_0;
	glm::vec3 field_4;
	uint8_t field_10;
};

DEFINE_TAG_COMPONENT(eoc, IsInTurnBasedModeComponent, IsInTurnBasedMode)

struct ItemBoostsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ItemBoosts;
	static constexpr auto EngineClass = "eoc::ItemBoostsComponent";

	Array<EntityHandle> Boosts;
};

struct EquipableComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Equipable;
	static constexpr auto EngineClass = "eoc::EquipableComponent";

	[[bg3::legacy(field_18)]] Guid EquipmentTypeID;
	ItemSlot Slot;
};

struct RaceComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Race;
	static constexpr auto EngineClass = "eoc::RaceComponent";

	Guid Race;
};

struct CanTravelComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanTravel;
	static constexpr auto EngineClass = "eoc::CanTravelComponent";

	TravelFlags Flags;
	uint16_t field_2;
	TravelErrorFlags ErrorFlags;
};

struct MovementComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Movement;
	static constexpr auto EngineClass = "eoc::MovementComponent";

	glm::vec3 Direction;
	float Acceleration;
	float Speed;
	float Speed2;
};

struct ObjectInteractionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ObjectInteraction;
	static constexpr auto EngineClass = "eoc::ObjectInteractionComponent";

	Array<EntityHandle> Interactions;
};

struct CanDeflectProjectilesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanDeflectProjectiles;
	static constexpr auto EngineClass = "eoc::CanDeflectProjectilesComponent";

	uint16_t Flags;
};

struct ActiveSkeletonSlotsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ActiveSkeletonSlots;
	static constexpr auto EngineClass = "ls::ActiveSkeletonSlotsComponent";

	Array<FixedString> Slots;
};

struct SomeSharedServerClientObjId
{
	__int64 field_0;
	int field_8;
};


struct SomeSharedServerClientObjId2 : public SomeSharedServerClientObjId
{
	__int64 field_10;
	int field_18;
};

struct ApprovalRatingsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ApprovalRatings;
	static constexpr auto EngineClass = "eoc::approval::RatingsComponent";

	MultiHashMap<EntityHandle, int> Ratings;
	MultiHashSet<Guid> field_70;
};


struct AttitudesToPlayersComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::AttitudesToPlayers;
	static constexpr auto EngineClass = "eoc::attitude::AttitudesToPlayersComponent";

	struct Key
	{
		EntityHandle field_0;
		uint8_t field_8;
		Guid field_10;
		uint8_t field_20;

		inline bool operator == (Key const& o) const
		{
			return field_0 == o.field_0
				&& field_8 == o.field_8
				&& field_10 == o.field_10
				&& field_20 == o.field_20;
		}
	};

	MultiHashMap<Key, int> Attitudes;
};

template <>
inline uint64_t MultiHashMapHash<AttitudesToPlayersComponent::Key>(AttitudesToPlayersComponent::Key const& v)
{
	return HashMulti(v.field_0, v.field_8, v.field_10, v.field_20);
}

END_SE()

BEGIN_NS(eoc::background)

struct GoalsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::BackgroundGoals;
	static constexpr auto EngineClass = "eoc::background::GoalsComponent";

	struct Goal
	{
		Guid Goal;
		Guid Entity;
		FixedString CategoryId;
		uint64_t field_28;
	};

	MultiHashMap<Guid, Array<Goal>> Goals;
};

END_NS()

BEGIN_NS(eoc::calendar)

struct DaysPassedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CalendarDaysPassed;
	static constexpr auto EngineClass = "eoc::calendar::DaysPassedComponent";

	int Days;
};

struct StartingDateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CalendarStartingDate;
	static constexpr auto EngineClass = "eoc::calendar::StartingDateComponent";

	int Day;
	int Year;
};

END_NS()

BEGIN_NS(eoc::ftb)

struct ParticipantComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::FTBParticipant;
	static constexpr auto EngineClass = "eoc::ftb::ParticipantComponent";

	EntityHandle field_18;
};

struct ZoneBlockReasonComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::FTBZoneBlockReason;
	static constexpr auto EngineClass = "eoc::ftb::ZoneBlockReasonComponent";

	uint8_t Reason;
};

DEFINE_TAG_COMPONENT(eoc::ftb, RespectComponent, FTBRespect)

END_NS()

BEGIN_NS(heal)

DEFINE_TAG_COMPONENT(eoc::heal, BlockComponent, HealBlock)

END_NS()
