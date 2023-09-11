#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Misc.h>

namespace bg3se
{
	struct BoostConditionComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::BoostCondition;

		int ConditionFlags;
		uint8_t field_1C;
	};

	struct BoostsContainerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::BoostsContainer;

		RefMap<BoostType, Array<EntityHandle>> Boosts;
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

		int32_t AC;
	};

	struct AbilityBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityBoost;

		AbilityId Ability;
		int32_t Value;
		int8_t SomeFlag; // Unused?
	};

	struct RollBonusBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::RollBonusBoost;

		RollTypeId RollType;
		LuaExpressionBase Amount;
		AbilityId Ability;
		SkillId Skill;
	};

	struct AdvantageBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AdvantageBoost;

		AdvantageTypeId AdvantageType;
		AdvantageBoostType Type;
		AbilityId Ability;
		SkillId Skill;
		Array<Guid> Tags;
	};

	struct ActionResourceValueBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceValueBoost;

		Guid ResourceUUID;
		int Amount2;
		double Amount;
		DiceSizeId DiceSize;
	};

	struct CriticalHitBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CriticalHitBoost;

		CriticalHitBoostFlags Flags;
		int Value;
	};

	struct AbilityFailedSavingThrowBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityFailedSavingThrowBoost;

		AbilityId Ability;
	};

	struct ResistanceBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ResistanceBoost;

	    DamageType DamageType;
		ResistanceBoostFlags ResistanceFlags;
	    bool IsResistantToAll;
	};

	struct WeaponDamageResistanceBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponDamageResistanceBoost;

		Array<DamageType> DamageTypes;
	};

	struct ProficiencyBonusOverrideBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBonusOverrideBoost;

		int Value;
	};

	struct JumpMaxDistanceMultiplierBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::JumpMaxDistanceMultiplierBoost;

	    float Amount;
	};

	struct HalveWeaponDamageBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::HalveWeaponDamageBoost;

	    AbilityId Ability;
	};

	struct UnlockSpellBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::UnlockSpellBoost;

	    FixedString SpellId; // Param 1
	    AbilityId Ability; // Param 5
	    Guid SomeUUID; // Param 2
		SpellChildSelectionType SpellChildSelection; // Param 3
		SpellCooldownType CooldownType; // Param 4
	};

	struct SourceAdvantageBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SourceAdvantageBoost;

		SourceAdvantageType Type;
	};

	struct ProficiencyBonusBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBonusBoost;

		ProficiencyBonusBoostType Type;
		AbilityId Ability;
		SkillId Skill;
	};

	struct ProficiencyBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBoost;

		AbilityId Ability;
		SkillId Skill;
	};

	struct IncreaseMaxHPBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::IncreaseMaxHPBoost;

		LuaExpressionBase HP;
	};

	struct ActionResourceBlockBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceBlockBoost;

	    Guid ResourceUUID;
	    int IntParam;
	};

	struct StatusImmunityBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::StatusImmunityBoost;

		FixedString StatusID;
		Array<Guid> UnknownUUIDs;
	};

	struct UseBoostsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::UseBoosts;

		Array<BoostParameters> Boosts;
	};

	struct TemporaryHPBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::TemporaryHPBoost;

		LuaExpressionBase HP;
	};

	struct WeightBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeightBoost;

	    int Amount;
	};

	struct FactionOverrideBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::FactionOverrideBoost;

	    FixedString Faction;
	};

	struct ActionResourceMultiplierBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceMultiplierBoost;

	    Guid ResourceUUID;
	    int IntParam2;
	    int IntParam;
		DiceSizeId DiceSize;
	};

	struct InitiativeBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::InitiativeBoost;

	    int Amount;
	};

	struct DarkvisionRangeBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeBoost;

	    float Range;
	};

	struct DarkvisionRangeMinBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeMinBoost;

		float Range;
	};

	struct DarkvisionRangeOverrideBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeOverrideBoost;

		float Range;
	};

	struct AddTagBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AddTagBoost;

		Guid TagUUID;
	};

	struct IgnoreDamageThresholdMinBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::IgnoreDamageThresholdMinBoost;

		DamageType DamageType;
		bool All;
		int Amount;
	};

	struct SkillBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SkillBoost;

		SkillId Skill;
		int Amount;
	};

	struct WeaponDamageBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponDamageBoost;

		DamageType DamageType;
	    int DamageMin;
	    int DamageMax;
	    bool field_24;
	};

	struct NullifyAbilityBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::NullifyAbilityBoost;

	    AbilityId Ability;
	};

	struct RerollBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::RerollBoost;

		RollTypeId RollType;
	    int8_t field_19;
	    bool field_1A;
	};

	struct DownedStatusBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DownedStatusBoost;

	    FixedString StatusId;
	};

	struct WeaponEnchantmentBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponEnchantmentBoost;

	    int Value;
	};

	struct GuaranteedChanceRollOutcomeBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::GuaranteedChanceRollOutcomeBoost;

	    bool field_18;
	};

	struct AttributeBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AttributeBoost;

	    AttributeFlags AttributeFlags;
	};

	struct GameplayLightBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::GameplayLightBoost;

	    int field_18;
	    uint8_t field_1C;
	    int field_20;
	};

	struct DualWieldingBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DualWieldingBoost;

	    bool field_18;
	};

	struct SavantBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SavantBoost;

	    SpellSchoolId SpellSchool;
	};

	struct MinimumRollResultBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::MinimumRollResultBoost;

	    RollTypeId RollType;
	    int8_t Result;
	};

	struct CharacterWeaponDamageBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CharacterWeaponDamageBoost;

	    int MinDamage;
		int MaxDamage;
	    DamageType DamageType;
	};

	struct ProjectileDeflectBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProjectileDeflectBoost;

	    ProjectileTypeIds ProjectileTypes;
	};

	struct AbilityOverrideMinimumBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityOverrideMinimumBoost;

		AbilityId Ability;
		int Amount;
	};

	struct ACOverrideMinimumBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ACOverrideMinimumBoost;

	    int Amount;
	    bool field_1C;
	};

	struct FallDamageMultiplierBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::FallDamageMultiplierBoost;

	    float Amount;
	};
}
