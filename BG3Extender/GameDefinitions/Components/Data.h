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
	DEFINE_COMPONENT(Armor, "eoc::ArmorComponent")

	int ArmorType;
	int ArmorClass;
	int AbilityModifierCap;
	uint8_t ArmorClassAbility;
	uint8_t Shield;
};

struct ArmorSetStateComponent : public BaseComponent
{
	DEFINE_COMPONENT(ArmorSetState, "eoc::armor_set::StateComponent")

	uint8_t State;
};

struct BaseHpComponent : public BaseComponent
{
	DEFINE_COMPONENT(BaseHp, "eoc::BaseHpComponent")

	int Vitality;
	int VitalityBoost;
};

struct Bound
{
	EntityHandle Entity;
	RefMap<AIBoundType, AIBound> AIBounds;
	[[bg3::hidden]] void* AiGrid;
	[[bg3::hidden]] void* Parent;
	MultiHashSet<uint32_t> GridPoints;
	glm::vec3 Translate;
	glm::quat RotationQuat;
	float Scale;
	int16_t MetaDataIndex;
	uint16_t CollidingRefCount;
	glm::vec3 TranslateOverride;
	uint8_t Flags;
	uint8_t field_89;
	uint16_t BoundFlags;
	FixedString OwnerPlatform;
	uint64_t field_90;
};

struct BoundComponent : public BaseProxyComponent
{
	DEFINE_COMPONENT(Bound, "eoc::BoundComponent")

	Bound Bound;
};

struct CustomStatsComponent : public BaseComponent
{
	DEFINE_COMPONENT(CustomStats, "eoc::CustomStatsComponent")

	Map<FixedString, int> Stats;
};

struct DataComponent : public BaseComponent
{
	DEFINE_COMPONENT(Data, "eoc::DataComponent")

	int32_t Weight;
	FixedString StatsId;
	uint32_t StepsType;
};

struct DetachedComponent : public BaseComponent
{
	DEFINE_COMPONENT(Detached, "eoc::DetachedComponent")

	uint32_t Flags;
};


struct ExperienceComponent : public BaseComponent
{
	DEFINE_COMPONENT(Experience, "eoc::exp::ExperienceComponent")

	int CurrentLevelExperience;
	int NextLevelExperience;
	int TotalExperience;
	uint8_t field_28;
};

struct HealthComponent : public BaseComponent
{
	DEFINE_COMPONENT(Health, "eoc::HealthComponent")

	int Hp;
	int MaxHp;
	int TemporaryHp;
	int MaxTemporaryHp;
	Guid field_10;
	uint8_t field_20;
};

struct ResistancesComponent : public BaseComponent
{
	DEFINE_COMPONENT(Resistances, "eoc::ResistancesComponent")

	std::array<ResistanceBoostFlags, 14> Resistances;
	uint8_t field_E;
	int AC;
	std::array<uint16_t, 14> PerDamageTypeHealthThresholds;
	std::array<uint32_t, 14> PerDamageTypeHealthThresholds2;
};

struct DifficultyCheckComponent : public BaseComponent
{
	DEFINE_COMPONENT(DifficultyCheck, "eoc::DifficultyCheckComponent")

	Array<int32_t> field_0;
	Array<int32_t> field_10;
	Array<AbilityId> Abilities;
	Array<uint32_t> field_30;
	int field_40;
	int field_44;
};

struct AttributeFlagsComponent : public BaseComponent
{
	DEFINE_COMPONENT(AttributeFlags, "eoc::AttributeFlagsComponent")

	uint32_t AttributeFlags;
};

struct BodyTypeComponent : public BaseComponent
{
	DEFINE_COMPONENT(BodyType, "eoc::BodyTypeComponent")

	uint8_t BodyType;
	uint8_t BodyType2;
};

struct ObjectSizeComponent : public BaseComponent
{
	DEFINE_COMPONENT(ObjectSize, "eoc::ObjectSizeComponent")

	uint8_t Size;
	[[bg3::legacy(field_1)]] uint8_t SoundSize;
};

struct BaseStatsComponent : public BaseComponent
{
	DEFINE_COMPONENT(BaseStats, "eoc::BaseStatsComponent")

	std::array<int, 7> BaseAbilities;
};

struct ExpertiseComponent : public BaseComponent
{
	DEFINE_COMPONENT(Expertise, "eoc::expertise::ExpertiseComponent")

	MultiHashSet<SkillId> Expertise;
};

struct StatsComponent : public BaseComponent
{
	DEFINE_COMPONENT(Stats, "eoc::StatsComponent")

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
	DEFINE_COMPONENT(StatusImmunities, "eoc::StatusImmunitiesComponent")

	MultiHashMap<FixedString, Guid> PersonalStatusImmunities;
};

struct ValueComponent : public BaseComponent
{
	DEFINE_COMPONENT(Value, "eoc::ValueComponent")

	int Value;
	uint8_t Rarity;
	bool Unique;
};

struct WeaponComponent : public BaseComponent
{
	DEFINE_COMPONENT(Weapon, "eoc::WeaponComponent")

	RefMap<AbilityId, Array<RollDefinition>> Rolls;
	RefMap<AbilityId, Array<RollDefinition>> Rolls2;
	float WeaponRange;
	float DamageRange;
	[[bg3::hidden]]
	void* WeaponFunctors;
	uint32_t WeaponProperties;
	uint8_t WeaponGroup;
	AbilityId Ability;
	Array<StatsExpressionWithMetadata> field_38;
	DiceSizeId DamageDice;
	DiceSizeId VersatileDamageDice;
};

struct BackgroundComponent : public BaseComponent
{
	DEFINE_COMPONENT(Background, "eoc::BackgroundComponent")

	Guid Background;
};
	
struct GodComponent : public BaseComponent
{
	DEFINE_COMPONENT(God, "eoc::god::GodComponent")

	Guid God;
	Guid field_10;
	uint8_t field_20;
	int field_24;
};
	
struct ProficiencyComponent : public BaseComponent
{
	DEFINE_COMPONENT(Proficiency, "eoc::stats::proficiency::ProficiencyComponent")

	[[bg3::legacy(field_0)]] ProficiencyGroupFlags Flags;
};

struct ProficiencyGroupComponent : public BaseComponent
{
	DEFINE_COMPONENT(ProficiencyGroup, "eoc::stats::proficiency::ProficiencyGroupComponent")

	__int64 field_0;
};


struct LevelUpUpgrades
{
	struct Selector
	{
		[[bg3::legacy(field_0)]] uint8_t Type;
		Guid Class;
		Guid Subclass;
		[[bg3::legacy(field_28)]] int Level;
		[[bg3::legacy(field_2C)]] bool Multiclass;
		int field_30;
	};

	struct FeatSelector : public Selector
	{
		Guid Feat;
		Array<uint8_t> Array_b8;
		STDString FeatName;
		int field_80;
	};

	struct SkillSelector : public Selector
	{
		Guid Skill;
		[[bg3::legacy(Array_b8)]] Array<SkillId> Proficiencies;
		STDString field_60;
		int field_80;
	};
		
	struct SkillExpertiseSelector : public Selector
	{
		[[bg3::legacy(field_38)]] Guid Skill;
		uint8_t field_48;
		[[bg3::legacy(Array_b8)]] Array<SkillId> Expertise;
		STDString field_60;
		int field_80;
	};
		
	struct AbilityBonusSelector : public Selector
	{
		Guid AbilityBonus;
		[[bg3::legacy(Array_b8)]] Array<AbilityId> Bonuses;
		[[bg3::legacy(Array_i32)]] Array<uint32_t> BonusAmounts;
		[[bg3::legacy(field_60)]] STDString BonusType;
		int field_80;
	};
		
	struct SpellSelector : public Selector
	{
		struct StringPair
		{
			FixedString From;
			FixedString To;
		};

		Guid SpellList;
		Array<FixedString> Spells;
		[[bg3::legacy(Array_FS2)]] Array<StringPair> ReplaceSpells;
		[[bg3::legacy(field_78)]] STDString SelectorId;
	};
		
	struct PassiveSelector : public Selector
	{
		struct StringPair
		{
			FixedString From;
			FixedString To;
		};

		[[bg3::legacy(SpellList)]] Guid PassiveList;
		[[bg3::legacy(Spells)]] Array<FixedString> Passives;
		[[bg3::legacy(Array_FS2)]] Array<StringPair> ReplacePassives;
		[[bg3::legacy(field_78)]] STDString SelectorId;
		int field_80;
	};
		
	struct EquipmentSelector : public Selector
	{
		[[bg3::legacy(field_38)]] Guid EquipmentList;
		[[bg3::legacy(Array_FS)]] Array<FixedString> Equipment;
		[[bg3::legacy(field_60)]] STDString SelectorId;
		int field_80;
	};

	Array<FeatSelector> Feats;
	Array<AbilityBonusSelector> AbilityBonuses;
	Array<SkillSelector> Skills;
	[[bg3::legacy(Unknowns2), bg3::legacy(SkillBonuses)]] Array<SkillExpertiseSelector> SkillExpertise;
	Array<SpellSelector> Spells;
	[[bg3::legacy(Spells2)]] Array<PassiveSelector> Passives;
	[[bg3::legacy(Unknowns4)]] Array<EquipmentSelector> Equipment;
};

struct LevelUpData
{
	Guid Class;
	Guid SubClass;
	Guid Feat;
	Guid AccessorySet;
	std::array<int, 7> Abilities;
	LevelUpUpgrades Upgrades;
	[[bg3::legacy(field_B0)]] Array<SpellMetaId> Spells;
};

struct FloatingComponent : public BaseComponent
{
	DEFINE_COMPONENT(Floating, "eoc::FloatingComponent")

	int field_18;
	int field_1C;
};

struct VoiceComponent : public BaseComponent
{
	DEFINE_COMPONENT(Voice, "eoc::VoiceComponent")

	Guid Voice;
};

struct CharacterCreationStatsComponent : public BaseComponent
{
	DEFINE_COMPONENT(CharacterCreationStats, "eoc::CharacterCreationStatsComponent")

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
	DEFINE_COMPONENT(Disarmable, "eoc::DisarmableComponent")

	Guid field_0;
	uint8_t field_10;
	uint8_t field_11;
};

struct ShortRestComponent : public BaseComponent
{
	DEFINE_COMPONENT(ShortRest, "eoc::rest::ShortRestComponent")

	uint8_t StateId;
};

struct IsSummonComponent : public BaseComponent
{
	DEFINE_COMPONENT(IsSummon, "eoc::summon::IsSummonComponent")

	EntityHandle Owner_M;
	EntityHandle field_8;
	Guid field_10;
	EntityHandle field_20;
	FixedString field_28;
};

struct SummonContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(SummonContainer, "eoc::summon::ContainerComponent")

	[[bg3::legacy(field_18)]] MultiHashMap<FixedString, Array<EntityHandle>> ByTag;
	MultiHashSet<EntityHandle> Characters;
	MultiHashSet<EntityHandle> Items;
};

struct StealthComponent : public BaseComponent
{
	DEFINE_COMPONENT(Stealth, "eoc::StealthComponent")

	bool SeekHiddenFlag;
	glm::vec3 Position;
	float SeekHiddenTimeout;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
};

DEFINE_TAG_COMPONENT(eoc, ClientControlComponent, ClientControl)
DEFINE_TAG_COMPONENT(ls, IsGlobalComponent, IsGlobal)
DEFINE_TAG_COMPONENT(ls, SavegameComponent, Savegame)

struct DisabledEquipmentComponent : public BaseComponent
{
	DEFINE_COMPONENT(DisabledEquipment, "eoc::DisabledEquipmentComponent")

	bool ShapeshiftFlag;
};

struct LootingStateComponent : public BaseComponent
{
	DEFINE_COMPONENT(LootingState, "eoc::LootingStateComponent")

	EntityHandle Looter_M;
	uint8_t State;
	int field_24;
};

struct LootComponent : public BaseComponent
{
	DEFINE_COMPONENT(Loot, "eoc::LootComponent")

	uint8_t Flags;
	uint8_t InventoryType;
};

struct LockComponent : public BaseComponent
{
	DEFINE_COMPONENT(Lock, "eoc::lock::LockComponent")

	FixedString Key_M;
	int LockDC;
	Guid field_8;
	Array<Guid> field_18;
};

struct KeyComponent : public BaseComponent
{
	DEFINE_COMPONENT(Key, "eoc::lock::KeyComponent")

	FixedString Key;
};

struct SummonLifetimeComponent : public BaseComponent
{
	DEFINE_COMPONENT(SummonLifetime, "eoc::summon::LifetimeComponent")

	std::variant<uint8_t, float> Lifetime;
};

struct HotbarContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(HotbarContainer, "eoc::hotbar::ContainerComponent")

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
	DEFINE_COMPONENT(HotbarDecks, "eoc::hotbar::CurrentDecksComponent")

	MultiHashMap<FixedString, int32_t> Decks;
};

struct OriginTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(OriginTag, "eoc::OriginTagComponent")

	Array<Guid> Tags;
};

struct PassiveInfo
{
	uint32_t field_0;
	uint32_t Passive;
};

struct OriginPassivesComponent : public BaseComponent
{
	DEFINE_COMPONENT(OriginPassives, "eoc::OriginPassivesComponent")

	Array<PassiveInfo> field_18;
};

struct ClassTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(ClassTag, "eoc::ClassTagComponent")

	Array<Guid> Tags;
};

struct BackgroundTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(BackgroundTag, "eoc::BackgroundTagComponent")

	Array<Guid> Tags;
};

struct OriginAppearanceTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(OriginAppearanceTag, "eoc::OriginAppearanceTagComponent")

	Array<Guid> Tags;
};

struct VoiceTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(VoiceTag, "eoc::VoiceTagComponent")

	Array<Guid> Tags;
};

struct GodTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(GodTag, "eoc::god::TagComponent")

	Array<Guid> Tags;
};

struct BackgroundPassivesComponent : public BaseComponent
{
	DEFINE_COMPONENT(BackgroundPassives, "eoc::BackgroundPassivesComponent")

	Array<PassiveInfo> field_18;
};

struct GlobalShortRestDisabledComponent : public BaseComponent
{
	DEFINE_COMPONENT(GlobalShortRestDisabled, "eoc::GlobalShortRestDisabledComponent")
};

struct GlobalLongRestDisabledComponent : public BaseComponent
{
	DEFINE_COMPONENT(GlobalLongRestDisabled, "eoc::GlobalLongRestDisabledComponent")
};

struct StoryShortRestDisabledComponent : public BaseComponent
{
	DEFINE_COMPONENT(StoryShortRestDisabled, "eoc::StoryShortRestDisabledComponent")
};

struct FleeCapabilityComponent : public BaseComponent
{
	DEFINE_COMPONENT(FleeCapability, "eoc::FleeCapabilityComponent")

	FleeErrorFlags Flags;
	[[bg3::legacy(field_1C)]] float FleeDistance;
	[[bg3::legacy(field_20)]] float CurrentFleeDistance;
};

struct CanDoRestComponent : public BaseComponent
{
	DEFINE_COMPONENT(CanDoRest, "eoc::CanDoRestComponent")

	RestFlags Flags;
	RestErrorFlags RestErrorFlags1;
	RestErrorFlags RestErrorFlags2;
};

struct CanModifyHealthComponent : public BaseComponent
{
	DEFINE_COMPONENT(CanModifyHealth, "eoc::CanModifyHealthComponent")
		
	uint16_t Flags;
};

struct AvailableLevelComponent : public BaseComponent
{
	DEFINE_COMPONENT(AvailableLevel, "eoc::exp::AvailableLevelComponent")

	int Level;
};

struct CanBeLootedComponent : public BaseComponent
{
	DEFINE_COMPONENT(CanBeLooted, "eoc::CanBeLootedComponent")

	uint16_t Flags;
};

struct CanBeDisarmedComponent : public BaseComponent
{
	DEFINE_COMPONENT(CanBeDisarmed, "eoc::CanBeDisarmedComponent")

	uint16_t Flags;
};

DEFINE_TAG_COMPONENT(eoc, CanTriggerRandomCastsComponent, CanTriggerRandomCasts)

struct CanDoActionsComponent : public BaseComponent
{
	DEFINE_COMPONENT(CanDoActions, "eoc::CanDoActionsComponent")

	uint16_t Flags;
};

struct ActionUseConditionsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ActionUseConditions, "eoc::action::ActionUseConditionsComponent")

	Array<int32_t> Conditions;
};


struct CanMoveComponent : public BaseComponent
{
	DEFINE_COMPONENT(CanMove, "eoc::CanMoveComponent")

	uint16_t Flags;
	uint16_t field_4;
	uint8_t field_6;
};

struct CanSenseComponent : public BaseComponent
{
	DEFINE_COMPONENT(CanSense, "eoc::CanSenseComponent")

	uint16_t Flags;
};

struct ConcentrationComponent : public BaseComponent
{
	DEFINE_COMPONENT(Concentration, "eoc::concentration::ConcentrationComponent")

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
	DEFINE_COMPONENT(Darkness, "eoc::DarknessComponent")

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
	DEFINE_COMPONENT(DualWielding, "eoc::DualWieldingComponent")

	bool Melee;
	bool RangedUI;
	uint8_t field_1A;
	bool Ranged;
	bool MeleeUI;
	uint8_t field_1D;
	bool DisableDualWielding;
};

DEFINE_TAG_COMPONENT(eoc, GravityDisabledComponent, GravityDisabled)
DEFINE_TAG_COMPONENT(eoc::improvised_weapon, CanBeWieldedComponent, CanBeWielded)
DEFINE_TAG_COMPONENT(eoc::tag, AvatarComponent, Avatar)
DEFINE_TAG_COMPONENT(eoc::tag, HasExclamationDialogComponent, HasExclamationDialog)
DEFINE_TAG_COMPONENT(eoc::tag, TraderComponent, Trader)
DEFINE_TAG_COMPONENT(eoc::ambush, AmbushingComponent, Ambushing)

struct InteractionFilterComponent : public BaseComponent
{
	DEFINE_COMPONENT(InteractionFilter, "eoc::InteractionFilterComponent")

	MultiHashSet<Guid> field_0;
	uint8_t field_30;
	uint8_t field_31;
};

struct InvisibilityComponent : public BaseComponent
{
	DEFINE_COMPONENT(Invisibility, "eoc::InvisibilityComponent")

	uint8_t field_0;
	glm::vec3 field_4;
	uint8_t field_10;
};

DEFINE_TAG_COMPONENT(eoc, IsInTurnBasedModeComponent, IsInTurnBasedMode)

struct ItemBoostsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ItemBoosts, "eoc::ItemBoostsComponent")

	Array<EntityHandle> Boosts;
};

struct EquipableComponent : public BaseComponent
{
	DEFINE_COMPONENT(Equipable, "eoc::EquipableComponent")

	[[bg3::legacy(field_18)]] Guid EquipmentTypeID;
	ItemSlot Slot;
};

struct RaceComponent : public BaseComponent
{
	DEFINE_COMPONENT(Race, "eoc::RaceComponent")

	Guid Race;
};

struct CanTravelComponent : public BaseComponent
{
	DEFINE_COMPONENT(CanTravel, "eoc::CanTravelComponent")

	TravelFlags Flags;
	uint16_t field_2;
	TravelErrorFlags ErrorFlags;
};

struct MovementComponent : public BaseComponent
{
	DEFINE_COMPONENT(Movement, "eoc::MovementComponent")

	glm::vec3 Direction;
	float Acceleration;
	float Speed;
	float Speed2;
};

struct ObjectInteractionComponent : public BaseComponent
{
	DEFINE_COMPONENT(ObjectInteraction, "eoc::ObjectInteractionComponent")

	Array<EntityHandle> Interactions;
};

struct CanDeflectProjectilesComponent : public BaseComponent
{
	DEFINE_COMPONENT(CanDeflectProjectiles, "eoc::CanDeflectProjectilesComponent")

	uint16_t Flags;
};

struct ActiveSkeletonSlotsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ActiveSkeletonSlots, "ls::ActiveSkeletonSlotsComponent")

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
	DEFINE_COMPONENT(ApprovalRatings, "eoc::approval::RatingsComponent")

	MultiHashMap<EntityHandle, int> Ratings;
	MultiHashSet<Guid> field_70;
};


struct AttitudesToPlayersComponent : public BaseComponent
{
	DEFINE_COMPONENT(AttitudesToPlayers, "eoc::attitude::AttitudesToPlayersComponent")

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
	DEFINE_COMPONENT(BackgroundGoals, "eoc::background::GoalsComponent")

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
	DEFINE_COMPONENT(CalendarDaysPassed, "eoc::calendar::DaysPassedComponent")

	int Days;
};

struct StartingDateComponent : public BaseComponent
{
	DEFINE_COMPONENT(CalendarStartingDate, "eoc::calendar::StartingDateComponent")

	int Day;
	int Year;
};

END_NS()

BEGIN_NS(eoc::ftb)

struct ParticipantComponent : public BaseComponent
{
	DEFINE_COMPONENT(FTBParticipant, "eoc::ftb::ParticipantComponent")

	EntityHandle field_18;
};

struct ZoneBlockReasonComponent : public BaseComponent
{
	DEFINE_COMPONENT(FTBZoneBlockReason, "eoc::ftb::ZoneBlockReasonComponent")

	uint8_t Reason;
};

DEFINE_TAG_COMPONENT(eoc::ftb, RespectComponent, FTBRespect)

END_NS()

BEGIN_NS(heal)

DEFINE_TAG_COMPONENT(eoc::heal, BlockComponent, HealBlock)

END_NS()
