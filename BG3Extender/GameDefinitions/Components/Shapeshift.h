#pragma once

BEGIN_NS(esv::shapeshift)

struct HealthReservationComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ShapeshiftHealthReservation;
	static constexpr auto EngineClass = "esv::shapeshift::HealthReservationComponent";

	MultiHashMap<Guid, int32_t> Reservations;
};

struct IntOverride
{
	int Value;
	uint32_t Flags;
};

struct FloatOverride
{
	float Value;
	uint32_t Flags;
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
	std::optional<FixedString> field_7C;
	std::optional<FloatOverride> Weight;
	std::optional<uint32_t> StepsType;
	std::optional<Guid> Race;
	std::optional<uint8_t> Gender;
	std::optional<uint8_t> field_B2;
	uint8_t field_B4;
	uint8_t field_B5;
	Array<Guid> field_B8;
	Array<Guid> field_C8;
	std::optional<uint8_t> field_D8;
	std::optional<uint16_t> field_DA;
	std::optional<uint16_t> field_DE;
	std::optional<uint16_t> field_E2;
	std::optional<uint16_t> field_E6;
	std::optional<STDString> field_F0;
	std::optional<RootTemplateOverride> RootTemplate;
	std::optional<TranslatedString> DisplayName;
	std::optional<TranslatedString> Title;
	std::optional<FixedString> SpellSet;
	std::optional<IntOverride> field_14C;
	std::optional<EntityHandle> field_158;
	uint8_t field_168;
	uint8_t field_169;
	uint8_t field_16A;
	MultiHashMap<EntityHandle, Array<FixedString>> field_170;
	Array<FixedString> field_1B0;
	uint8_t field_1C0;
	std::optional<Guid> AiHint;
	std::optional<FixedString> CombatArchetype;
	std::optional<int32_t> Armor;
	std::optional<int32_t> ArmorType;
	std::optional<AbilityId> UnarmedAttackAbility;
	std::optional<AbilityId> UnarmedRangedAttackAbility;
	uint8_t field_1FC;
	uint8_t field_1FD;
	std::optional<bool> CoverAmount;
	std::optional<bool> CanShootThrough;
	std::optional<uint8_t> ShootThroughType;
	std::optional<bool> WalkThrough;
	[[bg3::hidden]] Array<void*> field_208;
	bool CustomFlags;
	uint32_t Flags;
	bool CustomDefaultBoosts;
	FixedString DefaultBoosts;
	bool CustomPersonalStatusImmunities;
	FixedString PersonalStatusImmunities;
	bool CustomPassives;
	FixedString Passives;
	bool CustomResistances;
	std::array<std::array<ResistanceBoostFlags, 7>, 2> Resistances;
	bool CustomDifficultyStatuses;
	FixedString DifficultyStatuses;
	bool CustomProficiencyGroup;
	ProficiencyGroupFlags ProficiencyGroup;
	uint8_t field_258;
	uint8_t field_259;
	uint8_t field_25A;
	std::optional<AbilityId> SpellCastingAbility;
	uint64_t field_260;
	uint64_t field_268;
	uint8_t field_270;
	__declspec(align(8)) uint8_t field_278;
};

struct StatesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerShapeshiftStates;
	static constexpr auto EngineClass = "esv::shapeshift::StatesComponent";

	int field_0;
	Array<State> States;
};

END_NS()
