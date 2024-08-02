#pragma once

BEGIN_NS(esv::shapeshift)

struct HealthReservationComponent : public BaseComponent
{
	DEFINE_COMPONENT(ShapeshiftHealthReservation, "esv::shapeshift::HealthReservationComponent")

	HashMap<Guid, int32_t> Reservations;
};

struct IntOverride
{
	int Value;
	ShapeshiftChangeType Type;
};

struct FloatOverride
{
	float Value;
	ShapeshiftChangeType Type;
};

struct RootTemplateOverride
{
	FixedString RootTemplate;
	uint8_t field_114;
};

struct State
{
	Guid field_0;
	std::array<std::optional<IntOverride>, 7> Abilities;
	std::optional<IntOverride> Vitality;
	std::optional<IntOverride> VitalityBoost;
	std::optional<FixedString> HotBar;
	std::optional<FloatOverride> Weight;
	std::optional<uint32_t> StepsType;
	std::optional<Guid> Race;
	std::optional<uint8_t> Gender;
	std::optional<bool> UseShapeshiftIdentity;
	uint8_t field_B4;
	bool KillEntityAtZeroHP;
	Array<Guid> TagsAdd;
	Array<Guid> TagsRemove;
	std::optional<bool> RemoveOldTags;
	std::optional<uint16_t> ActionCapabilities;
	std::optional<uint16_t> InteractionsCapabilities;
	std::optional<uint16_t> AwarenessCapabilities;
	std::optional<uint16_t> SpeakingCapabilities;
	std::optional<STDString> ChangeScript;
	std::optional<RootTemplateOverride> RootTemplate;
	std::optional<TranslatedString> DisplayName;
	std::optional<TranslatedString> Title;
	std::optional<FixedString> Icon;
	std::optional<FloatOverride> Scale;
	std::optional<EntityHandle> field_158;
	bool ApplyVisualInCC;
	bool MuteEquipmentSound;
	bool OverrideSpeaker;
	HashMap<ShapeshiftSpellSource, Array<FixedString>> SpellsAdd;
	Array<FixedString> SpellsRemove;
	bool RemovePrevSpells;
	std::optional<Guid> AiHint;
	std::optional<FixedString> CombatArchetype;
	std::optional<int32_t> Armor;
	std::optional<int32_t> ArmorType;
	std::optional<AbilityId> UnarmedAttackAbility;
	std::optional<AbilityId> UnarmedRangedAttackAbility;
	uint8_t field_1FC;
	uint8_t field_1FD;
	std::optional<bool> CanSeeThrough;
	std::optional<bool> CanShootThrough;
	std::optional<uint8_t> ShootThroughType;
	std::optional<bool> WalkThrough;
	Array<SurfacePathInfluence> SurfacePathInfluences;
	uint8_t FlagsInheritanceType;
	uint32_t Flags;
	uint8_t DefaultBoostsInheritanceType;
	FixedString DefaultBoosts;
	uint8_t PersonalStatusImmunitiesInheritanceType;
	FixedString PersonalStatusImmunities;
	uint8_t PassivesInheritanceType;
	FixedString Passives;
	uint8_t ResistancesInheritanceType;
	std::array<std::array<ResistanceBoostFlags, 7>, 2> Resistances;
	uint8_t DifficultyStatusesInheritanceType;
	FixedString DifficultyStatuses;
	uint8_t ProficiencyGroupsInheritanceType;
	ProficiencyGroupFlags ProficiencyGroup;
	uint8_t BloodSurfaceTypeInheritanceType;
	uint8_t HitBloodTypeInheritanceType;
	uint8_t DeathActionsInheritanceType;
	std::optional<AbilityId> SpellCastingAbility;
	std::optional<Guid> DummyClass;
	bool BlockLevelUp;
};

struct StatesComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerShapeshiftStates, "esv::shapeshift::StatesComponent")

	int field_0;
	Array<State> States;
};

END_NS()
