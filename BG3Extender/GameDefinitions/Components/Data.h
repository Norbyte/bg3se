#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/RootTemplates.h>
#include <GameDefinitions/Progression.h>

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
	LegacyRefMap<AIBoundType, BoundData> AIBounds;
	[[bg3::hidden]] AiGrid* AiGrid;
	[[bg3::hidden]] void* Parent;
	HashSet<AiTilePos> GridPoints;
	glm::vec3 Translate;
	glm::quat RotationQuat;
	float Scale;
	[[bg3::readonly]] int16_t MetaDataIndex;
	[[bg3::readonly]] uint16_t CollidingRefCount;
	glm::vec3 TranslateOverride;
	BoundBaseFlags Flags;
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

	LegacyMap<FixedString, int> Stats;
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
	[[bg3::legacy(field_20)]] bool IsInvulnerable;
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

	[[bg3::legacy(field_0)]] Array<int32_t> AbilityDC;
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

	HashSet<SkillId> Expertise;
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

	HashMap<FixedString, Guid> PersonalStatusImmunities;
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

	LegacyRefMap<AbilityId, Array<RollDefinition>> Rolls;
	LegacyRefMap<AbilityId, Array<RollDefinition>> Rolls2;
	float WeaponRange;
	float DamageRange;
	stats::Functors* WeaponFunctors;
	uint32_t WeaponProperties;
	uint8_t WeaponGroup;
	AbilityId Ability;
	[[bg3::legacy(field_38)]] Array<StatsExpressionWithMetadata> DamageValues;
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

	[[bg3::legacy(field_0)]] ProficiencyGroupFlags Flags;
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

	[[bg3::legacy(Owner_M)]] EntityHandle Owner;
	[[bg3::legacy(field_8)]] EntityHandle Summoner;
	Guid field_10;
	EntityHandle field_20;
	FixedString field_28;
};

struct SummonContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(SummonContainer, "eoc::summon::ContainerComponent")

	[[bg3::legacy(field_18)]] HashMap<FixedString, Array<EntityHandle>> ByTag;
	HashSet<EntityHandle> Characters;
	HashSet<EntityHandle> Items;
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

struct OriginTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(OriginTag, "eoc::OriginTagComponent")

	Array<Guid> Tags;
};

struct OriginPassivesComponent : public BaseComponent
{
	DEFINE_COMPONENT(OriginPassives, "eoc::OriginPassivesComponent")

	Array<stats::PassivePrototype*> Passives;
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

	Array<stats::PassivePrototype*> field_18;
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

	CanDoActionsFlags Flags;
};

struct ActionUseConditionsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ActionUseConditions, "eoc::action::ActionUseConditionsComponent")

	Array<int32_t> Conditions;
};


struct CanMoveComponent : public BaseComponent
{
	DEFINE_COMPONENT(CanMove, "eoc::CanMoveComponent")

	CanMoveFlags Flags;
	uint16_t field_4;
	uint8_t field_6;
};

struct CanSenseComponent : public BaseComponent
{
	DEFINE_COMPONENT(CanSense, "eoc::CanSenseComponent")

	uint16_t Flags;
};

struct ConcentrationTarget
{
	[[bg3::legacy(field_0)]] EntityHandle Target;
	[[bg3::legacy(field_8)]] EntityHandle Concentration;
	[[bg3::legacy(field_10)]] ComponentHandle Status;
	[[bg3::legacy(field_18)]] int16_t SurfaceIndex;
	bool field_1A;
};

struct ConcentrationComponent : public BaseComponent
{
	DEFINE_COMPONENT(Concentration, "eoc::concentration::ConcentrationComponent")

	[[bg3::legacy(field_0)]] EntityHandle Caster;
	[[bg3::legacy(field_8)]] Array<ConcentrationTarget> Targets;
	SpellId SpellId;
};

struct DarknessComponent : public BaseComponent
{
	DEFINE_COMPONENT(Darkness, "eoc::DarknessComponent")

	uint8_t Sneaking;
	uint8_t Obscurity;
	[[bg3::legacy(field_2)]] bool SneakingConesVisibility;
	[[bg3::legacy(field_3)]] bool CharacterSneaking;
	[[bg3::legacy(field_4)]] bool ActuallySneaking;
	[[bg3::legacy(field_8)]] float GracePeriod;
	[[bg3::legacy(field_C)]] float GraceFrames;
};

struct DualWieldingComponent : public BaseComponent
{
	DEFINE_COMPONENT(DualWielding, "eoc::DualWieldingComponent")

	[[bg3::legacy(Melee)]] bool MeleeToggledOn;
	bool MeleeUI;
	[[bg3::legacy(field_1A)]] bool HasMeleeBoosts;
	[[bg3::legacy(Ranged)]] bool RangedToggledOn;
	bool RangedUI;
	[[bg3::legacy(field_1D)]] bool HasRangedBoosts;
	[[bg3::legacy(DisableDualWielding)]] bool ToggledOn;
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

	HashSet<Guid> field_0;
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

	HashMap<EntityHandle, int> Ratings;
	HashSet<Guid> field_70;
};


struct AttitudeIdentifier
{
	EntityHandle field_0;
	uint8_t field_8;
	Guid field_10;
	uint8_t field_20;

	inline bool operator == (AttitudeIdentifier const& o) const
	{
		return field_0 == o.field_0
			&& field_8 == o.field_8
			&& field_10 == o.field_10
			&& field_20 == o.field_20;
	}
};

struct AttitudesToPlayersComponent : public BaseComponent
{
	DEFINE_COMPONENT(AttitudesToPlayers, "eoc::attitude::AttitudesToPlayersComponent")

	HashMap<AttitudeIdentifier, int> Attitudes;
};

template <>
inline uint64_t HashMapHash<AttitudeIdentifier>(AttitudeIdentifier const& v)
{
	return HashMulti(v.field_0, v.field_8, v.field_10, v.field_20);
}

END_SE()

BEGIN_NS(eoc::background)

struct GoalRecord
{
	Guid Goal;
	Guid Entity;
	FixedString CategoryId;
	uint64_t field_28;
};

struct GoalsComponent : public BaseComponent
{
	DEFINE_COMPONENT(BackgroundGoals, "eoc::background::GoalsComponent")

	HashMap<Guid, Array<GoalRecord>> Goals;
};

END_NS()

BEGIN_NS(eoc::hotbar)

struct Slot
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
	Array<Slot> Elements;
	uint8_t Width;
	uint32_t Height;
	STDString field_20;
};

struct ContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(HotbarContainer, "eoc::hotbar::ContainerComponent")

	HashMap<FixedString, Array<Bar>> Containers;
	FixedString ActiveContainer;
};

struct HotbarCurrentDecksComponent : public BaseComponent
{
	DEFINE_COMPONENT(HotbarDecks, "eoc::hotbar::CurrentDecksComponent")

	HashMap<FixedString, int32_t> Decks;
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
