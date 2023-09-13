#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Misc.h>

BEGIN_SE()

struct BoostConditionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::BoostCondition;
	static constexpr auto EngineClass = "eoc::BoostConditionComponent";

	int ConditionFlags;
	uint8_t field_1C;
};

struct BoostsContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::BoostsContainer;
	static constexpr auto EngineClass = "eoc::BoostsContainerComponent";

	uint64_t field_0;
	MultiHashMap<BoostType, Array<EntityHandle>> Boosts;
};

struct BoostParameters
{
	FixedString Boost;
	STDString Params;
	STDString Params2;
};

struct BoostInfoComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::BoostInfo;
	static constexpr auto EngineClass = "eoc::BoostInfoComponent";

	BoostType Type;
	ComponentHandle field_18;
	uint64_t field_20;
	uint32_t Flags_M;
	FixedString Passive;
	EntityHandle Cause_M;
	int64_t field_38;
	EntityHandle OwnerEntity;
	BoostParameters Params;
	Guid field_90;
};

struct ArmorClassBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ArmorClassBoost;
	static constexpr auto EngineClass = "eoc::ArmorClassBoostComponent";

	int32_t AC;
};

struct AbilityBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityBoost;
	static constexpr auto EngineClass = "eoc::AbilityBoostComponent";

	AbilityId Ability;
	int32_t Value;
	int8_t SomeFlag; // Unused?
};

struct RollBonusBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::RollBonusBoost;
	static constexpr auto EngineClass = "eoc::RollBonusBoostComponent";

	RollTypeId RollType;
	LuaExpressionBase Amount;
	AbilityId Ability;
	SkillId Skill;
};

struct AdvantageBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::AdvantageBoost;
	static constexpr auto EngineClass = "eoc::AdvantageBoostComponent";

	AdvantageTypeId AdvantageType;
	AdvantageBoostType Type;
	AbilityId Ability;
	SkillId Skill;
	Array<Guid> Tags;
};

struct ActionResourceValueBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceValueBoost;
	static constexpr auto EngineClass = "eoc::ActionResourceValueBoostComponent";

	Guid ResourceUUID;
	int Amount2;
	double Amount;
	DiceSizeId DiceSize;
};

struct CriticalHitBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CriticalHitBoost;
	static constexpr auto EngineClass = "eoc::CriticalHitBoostComponent";

	CriticalHitBoostFlags Flags;
	int Value;
};

struct AbilityFailedSavingThrowBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityFailedSavingThrowBoost;
	static constexpr auto EngineClass = "eoc::AbilityFailedSavingThrowBoostComponent";

	AbilityId Ability;
};

struct ResistanceBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ResistanceBoost;
	static constexpr auto EngineClass = "eoc::ResistanceBoostComponent";

	DamageType DamageType;
	ResistanceBoostFlags ResistanceFlags;
	bool IsResistantToAll;
};

struct WeaponDamageResistanceBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponDamageResistanceBoost;
	static constexpr auto EngineClass = "eoc::WeaponDamageResistanceBoostComponent";

	Array<DamageType> DamageTypes;
};

struct ProficiencyBonusOverrideBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBonusOverrideBoost;
	static constexpr auto EngineClass = "eoc::ProficiencyBonusOverrideBoostComponent";

	int Value;
};

struct JumpMaxDistanceMultiplierBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::JumpMaxDistanceMultiplierBoost;
	static constexpr auto EngineClass = "eoc::JumpMaxDistanceMultiplierBoostComponent";

	float Amount;
};

struct HalveWeaponDamageBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::HalveWeaponDamageBoost;
	static constexpr auto EngineClass = "eoc::HalveWeaponDamageBoostComponent";

	AbilityId Ability;
};

struct UnlockSpellBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::UnlockSpellBoost;
	static constexpr auto EngineClass = "eoc::UnlockSpellBoostComponent";

	FixedString SpellId; // Param 1
	AbilityId Ability; // Param 5
	Guid SomeUUID; // Param 2
	SpellChildSelectionType SpellChildSelection; // Param 3
	SpellCooldownType CooldownType; // Param 4
};

struct SourceAdvantageBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SourceAdvantageBoost;
	static constexpr auto EngineClass = "eoc::SourceAdvantageBoostComponent";

	SourceAdvantageType Type;
};

struct ProficiencyBonusBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBonusBoost;
	static constexpr auto EngineClass = "eoc::ProficiencyBonusBoostComponent";

	ProficiencyBonusBoostType Type;
	AbilityId Ability;
	SkillId Skill;
};

struct ProficiencyBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBoost;
	static constexpr auto EngineClass = "eoc::ProficiencyBoostComponent";

	AbilityId Ability;
	SkillId Skill;
};

struct IncreaseMaxHPBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::IncreaseMaxHPBoost;
	static constexpr auto EngineClass = "eoc::IncreaseMaxHPBoostComponent";

	LuaExpressionBase HP;
};

struct ActionResourceBlockBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceBlockBoost;
	static constexpr auto EngineClass = "eoc::ActionResourceBlockBoostComponent";

	Guid ResourceUUID;
	int IntParam;
};

struct StatusImmunityBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::StatusImmunityBoost;
	static constexpr auto EngineClass = "eoc::StatusImmunityBoostComponent";

	FixedString StatusID;
	Array<Guid> UnknownUUIDs;
};

struct UseBoostsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::UseBoosts;
	static constexpr auto EngineClass = "eoc::UseBoostsComponent";

	Array<BoostParameters> Boosts;
};

struct TemporaryHPBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::TemporaryHPBoost;
	static constexpr auto EngineClass = "eoc::TemporaryHPBoostComponent";

	LuaExpressionBase HP;
};

struct WeightBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::WeightBoost;
	static constexpr auto EngineClass = "eoc::WeightBoostComponent";

	int Amount;
};

struct FactionOverrideBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::FactionOverrideBoost;
	static constexpr auto EngineClass = "eoc::FactionOverrideBoostComponent";

	FixedString Faction;
};

struct ActionResourceMultiplierBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceMultiplierBoost;
	static constexpr auto EngineClass = "eoc::ActionResourceMultiplierBoostComponent";

	Guid ResourceUUID;
	int IntParam2;
	int IntParam;
	DiceSizeId DiceSize;
};

struct InitiativeBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InitiativeBoost;
	static constexpr auto EngineClass = "eoc::InitiativeBoostComponent";

	int Amount;
};

struct DarkvisionRangeBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeBoost;
	static constexpr auto EngineClass = "eoc::DarkvisionRangeBoostComponent";

	float Range;
};

struct DarkvisionRangeMinBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeMinBoost;
	static constexpr auto EngineClass = "eoc::DarkvisionRangeMinBoostComponent";

	float Range;
};

struct DarkvisionRangeOverrideBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeOverrideBoost;
	static constexpr auto EngineClass = "eoc::DarkvisionRangeOverrideBoostComponent";

	float Range;
};

struct AddTagBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::AddTagBoost;
	static constexpr auto EngineClass = "eoc::AddTagBoostComponent";

	Guid TagUUID;
};

struct IgnoreDamageThresholdMinBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::IgnoreDamageThresholdMinBoost;
	static constexpr auto EngineClass = "eoc::IgnoreDamageThresholdMinBoostComponent";

	DamageType DamageType;
	bool All;
	int Amount;
};

struct SkillBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SkillBoost;
	static constexpr auto EngineClass = "eoc::SkillBoostComponent";

	SkillId Skill;
	int Amount;
};

struct WeaponDamageBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponDamageBoost;
	static constexpr auto EngineClass = "eoc::WeaponDamageBoostComponent";

	DamageType DamageType;
	int DamageMin;
	int DamageMax;
	bool field_24;
};

struct NullifyAbilityBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::NullifyAbilityBoost;
	static constexpr auto EngineClass = "eoc::NullifyAbilityBoostComponent";

	AbilityId Ability;
};

struct RerollBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::RerollBoost;
	static constexpr auto EngineClass = "eoc::RerollBoostComponent";

	RollTypeId RollType;
	int8_t field_19;
	bool field_1A;
};

struct DownedStatusBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DownedStatusBoost;
	static constexpr auto EngineClass = "eoc::DownedStatusBoostComponent";

	FixedString StatusId;
};

struct WeaponEnchantmentBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponEnchantmentBoost;
	static constexpr auto EngineClass = "eoc::WeaponEnchantmentBoostComponent";

	int Value;
};

struct GuaranteedChanceRollOutcomeBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::GuaranteedChanceRollOutcomeBoost;
	static constexpr auto EngineClass = "eoc::GuaranteedChanceRollOutcomeBoostComponent";

	bool field_18;
};

struct AttributeBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::AttributeBoost;
	static constexpr auto EngineClass = "eoc::AttributeBoostComponent";

	AttributeFlags AttributeFlags;
};

struct GameplayLightBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::GameplayLightBoost;
	static constexpr auto EngineClass = "eoc::GameplayLightBoostComponent";

	int field_18;
	uint8_t field_1C;
	int field_20;
};

struct DualWieldingBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DualWieldingBoost;
	static constexpr auto EngineClass = "eoc::DualWieldingBoostComponent";

	bool field_18;
};

struct SavantBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SavantBoost;
	static constexpr auto EngineClass = "eoc::SavantBoostComponent";

	SpellSchoolId SpellSchool;
};

struct MinimumRollResultBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::MinimumRollResultBoost;
	static constexpr auto EngineClass = "eoc::MinimumRollResultBoostComponent";

	RollTypeId RollType;
	int8_t Result;
};

struct CharacterWeaponDamageBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CharacterWeaponDamageBoost;
	static constexpr auto EngineClass = "eoc::CharacterWeaponDamageBoostComponent";

	int MinDamage;
	int MaxDamage;
	DamageType DamageType;
};

struct ProjectileDeflectBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ProjectileDeflectBoost;
	static constexpr auto EngineClass = "eoc::ProjectileDeflectBoostComponent";

	ProjectileTypeIds ProjectileTypes;
};

struct AbilityOverrideMinimumBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityOverrideMinimumBoost;
	static constexpr auto EngineClass = "eoc::AbilityOverrideMinimumBoostComponent";

	AbilityId Ability;
	int Amount;
};

struct FallDamageMultiplierBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::FallDamageMultiplierBoost;
	static constexpr auto EngineClass = "eoc::FallDamageMultiplierBoostComponent";

	float Amount;
};

END_SE()
