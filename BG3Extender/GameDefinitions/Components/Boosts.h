#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct BoostConditionComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::BoostCondition;

		int ConditionFlags;
		char field_1C;
	};

	struct BoostsContainerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::BoostsContainer;

		RefMap<uint8_t, Array<ObjectHandle>> Boosts;
	};

	struct BoostParameters
	{
		FixedString field_0;
		STDString field_8;
		STDString field_28;
	};

	struct BoostInfoComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::BoostInfo;

		BoostType Type;
		ObjectHandle field_20;
		uint32_t Flags_M;
		FixedString Passive;
		ObjectHandle Cause_M;
		int64_t field_38;
		ObjectHandle OwnerEntity;
		BoostParameters Params;
		UUID field_90;
	};

	struct ArmorClassBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ArmorClassBoost;

		int32_t AC;
	};

	struct AbilityBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityBoost;

		AbilityId Ability;
		int32_t Value;
		int8_t SomeFlag; // Unused?
	};

	struct RollBonusBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::RollBonusBoost;

		RollTypeId RollType;
		LuaExpressionBase Amount;
		AbilityId Ability;
		SkillId Skill;
	};

	struct AdvantageBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AdvantageBoost;

		AdvantageTypeId AdvantageType;
		AdvantageBoostType Type;
		AbilityId Ability;
		SkillId Skill;
		Array<UUID> Tags;
	};

	struct ActionResourceValueBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceValueBoost;

		UUID ResourceUUID;
		int Amount2;
		double Amount;
		DiceSizeId DiceSize;
	};

	struct CriticalHitBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CriticalHitBoost;

		CriticalHitBoostFlags Flags;
		int Value;
	};

	struct AbilityFailedSavingThrowBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityFailedSavingThrowBoost;

		AbilityId Ability;
	};

	struct ResistanceBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ResistanceBoost;

	    DamageType DamageType;
		ResistanceBoostFlags ResistanceFlags;
	    bool IsResistantToAll;
	};

	struct WeaponDamageResistanceBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponDamageResistanceBoost;

		Array<DamageType> DamageTypes;
	};

	struct ProficiencyBonusOverrideBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBonusOverrideBoost;

		int Value;
	};

	struct JumpMaxDistanceMultiplierBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::JumpMaxDistanceMultiplierBoost;

	    float Amount;
	};

	struct HalveWeaponDamageBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::HalveWeaponDamageBoost;

	    AbilityId Ability;
	};

	struct UnlockSpellBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::UnlockSpellBoost;

	    FixedString SpellId; // Param 1
	    AbilityId Ability; // Param 5
	    UUID SomeUUID; // Param 2
		SpellChildSelectionType SpellChildSelection; // Param 3
	    SpellUnlockDurationType UnlockDuration; // Param 4
	};

	struct SourceAdvantageBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SourceAdvantageBoost;

		SourceAdvantageType Type;
	};

	struct ProficiencyBonusBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBonusBoost;

		ProficiencyBonusBoostType Type;
		AbilityId Ability;
		SkillId Skill;
	};

	struct ProficiencyBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBoost;

		AbilityId Ability;
		SkillId Skill;
	};

	struct IncreaseMaxHPBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::IncreaseMaxHPBoost;

		LuaExpressionBase HP;
	};

	struct ActionResourceBlockBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceBlockBoost;

	    UUID ResourceUUID;
	    int IntParam;
	};

	struct StatusImmunityBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::StatusImmunityBoost;

		FixedString StatusID;
		Array<UUID> UnknownUUIDs;
	};

	struct UseBoosts : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::UseBoosts;

		Array<BoostParameters> Boosts;
	};

	struct TemporaryHPBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::TemporaryHPBoost;

		LuaExpressionBase HP;
	};

	struct WeightBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeightBoost;

	    int Amount;
	};

	struct FactionOverrideBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::FactionOverrideBoost;

	    FixedString Faction;
	};

	struct ActionResourceMultiplierBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceMultiplierBoost;

	    UUID ResourceUUID;
	    int IntParam2;
	    int IntParam;
		DiceSizeId DiceSize;
	};

	struct InitiativeBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::InitiativeBoost;

	    int Amount;
	};

	struct DarkvisionRangeBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeBoost;

	    float Range;
	};

	struct DarkvisionRangeMinBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeMinBoost;

		float Range;
	};

	struct DarkvisionRangeOverrideBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeOverrideBoost;

		float Range;
	};

	struct AddTagBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AddTagBoost;

		UUID TagUUID;
	};

	struct IgnoreDamageThresholdMinBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::IgnoreDamageThresholdMinBoost;

		DamageType DamageType;
		bool All;
		int Amount;
	};

	struct SkillBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SkillBoost;

		SkillId Skill;
		int Amount;
	};

	struct WeaponDamageBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponDamageBoost;

		DamageType DamageType;
	    int DamageMin;
	    int DamageMax;
	    bool field_24;
	};

	struct NullifyAbilityBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::NullifyAbilityBoost;

	    AbilityId Ability;
	};

	struct RerollBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::RerollBoost;

		RollTypeId RollType;
	    int8_t field_19;
	    bool field_1A;
	};

	struct DownedStatusBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DownedStatusBoost;

	    FixedString StatusId;
	};

	struct WeaponEnchantmentBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponEnchantmentBoost;

	    int Value;
	};

	struct GuaranteedChanceRollOutcomeBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::GuaranteedChanceRollOutcomeBoost;

	    bool field_18;
	};

	struct AttributeBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AttributeBoost;

	    AttributeFlags AttributeFlags;
	};

	struct GameplayLightBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::GameplayLightBoost;

	    int field_18;
	    char field_1C;
	    int field_20;
	};

	struct DualWieldingBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DualWieldingBoost;

	    bool field_18;
	};

	struct SavantBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SavantBoost;

	    SpellSchoolId SpellSchool;
	};

	struct MinimumRollResultBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::MinimumRollResultBoost;

	    RollTypeId RollType;
	    int8_t Result;
	};

	struct CharacterWeaponDamageBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CharacterWeaponDamageBoost;

	    int MinDamage;
		int MaxDamage;
	    DamageType DamageType;
	};

	struct ProjectileDeflectBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProjectileDeflectBoost;

	    ProjectileTypeIds ProjectileTypes;
	};

	struct AbilityOverrideMinimumBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityOverrideMinimumBoost;

		AbilityId Ability;
		int Amount;
	};

	struct ACOverrideMinimumBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ACOverrideMinimumBoost;

	    int Amount;
	    bool field_1C;
	};

	struct FallDamageMultiplierBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::FallDamageMultiplierBoost;

	    float Amount;
	};
}
