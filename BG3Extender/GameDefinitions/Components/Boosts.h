#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>

namespace dse
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

	struct SomeBoostParams
	{
		struct UNK
		{
			UUID a, b;
		};

		Array<UNK> Array_32b;
		STDString field_18;
	};

	struct ArmorClassBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ArmorClassBoost;

		int32_t AC;
	};

	struct AbilityBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityBoost;

		uint8_t Ability;
		int32_t Value;
		int8_t SomeFlag;
	};

	struct RollBonusBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::RollBonusBoost;

		char field_18;
		SomeBoostParams field_20;
		char field_58;
		char field_59;
	};

	struct AdvantageBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AdvantageBoost;

		int field_18;
		char field_1C;
		char field_1D;
		char field_1E;
		Array<UUID> arr_16b;
	};

	struct ActionResourceValueBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceValueBoost;

		UUID GUID_M;
		int field_28;
		__int64 field_30;
		char field_38;
	};

	struct CriticalHitBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CriticalHitBoost;

		char field_18;
		int field_1C;
	};

	struct AbilityFailedSavingThrowBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityFailedSavingThrowBoost;

		char field_18;
	};

	struct ResistanceBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ResistanceBoost;

	    char field_18;
	    int field_1C;
	    char field_20;
	};

	struct WeaponDamageResistanceBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponDamageResistanceBoost;

		Array<uint8_t> arr_1b;
	};

	struct ProficiencyBonusOverrideBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBonusOverrideBoost;

	    int field_18;
	};

	struct JumpMaxDistanceMultiplierBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::JumpMaxDistanceMultiplierBoost;

	    int field_18;
	};

	struct HalveWeaponDamageBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::HalveWeaponDamageBoost;

	    char field_18;
	};

	struct UnlockSpellBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::UnlockSpellBoost;

	    FixedString field_18;
	    char field_1C;
	    UUID GUID_M;
	    char field_30;
	    char field_31;
	};

	struct SourceAdvantageBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SourceAdvantageBoost;

		char field_18;
		__int64 field_20;
	};

	struct ProficiencyBonusBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBonusBoost;

	    int field_18;
	    char field_1C;
	    char field_1D;
	};

	struct ProficiencyBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProficiencyBoost;

	  int field_18;
	};

	struct IncreaseMaxHPBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::IncreaseMaxHPBoost;

		SomeBoostParams field_18;
	};

	struct ActionResourceBlockBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceBlockBoost;

	    UUID GUID_M;
	    int field_28;
	};

	struct StatusImmunityBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::StatusImmunityBoost;

		int arr_16b;
		Array<UUID> field_20;
	};

	struct UseBoosts : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::UseBoosts;

		Array<int32_t> field_18;
	};

	struct TemporaryHPBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::TemporaryHPBoost;

		SomeBoostParams field_18;
	};

	struct WeightBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeightBoost;

	    int field_18;
	};

	struct FactionOverrideBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::FactionOverrideBoost;

	    FixedString field_18;
	};

	struct ActionResourceMultiplierBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceMultiplierBoost;

	    UUID GUID_M;
	    int field_28;
	    int field_2C;
	    char field_30;
	};

	struct InitiativeBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::InitiativeBoost;

	    int field_18;
	};

	struct DarkvisionRangeBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeBoost;

	    int field_18;
	};

	struct DarkvisionRangeMinBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeMinBoost;

	    int field_18;
	};

	struct DarkvisionRangeOverrideBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DarkvisionRangeOverrideBoost;

	    int field_18;
	};

	struct AddTagBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AddTagBoost;

		UUID field_18;
	};

	struct IgnoreDamageThresholdMinBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::IgnoreDamageThresholdMinBoost;

		char field_18;
		char field_19;
		int field_1C;
	};

	struct SkillBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SkillBoost;

		char field_18;
		int field_1C;
	};

	struct WeaponDamageBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponDamageBoost;

	    char field_18;
	    int field_1C;
	    int field_20;
	    char field_24;
	};

	struct NullifyAbilityBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::NullifyAbilityBoost;

	    char field_18;
	};

	struct RerollBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::RerollBoost;

	    char field_18;
	    char field_19;
	    char field_1A;
	};

	struct DownedStatusBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::DownedStatusBoost;

	    FixedString field_18;
	};

	struct WeaponEnchantmentBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::WeaponEnchantmentBoost;

	    int field_18;
	};

	struct GuaranteedChanceRollOutcomeBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::GuaranteedChanceRollOutcomeBoost;

	    char field_18;
	};

	struct AttributeBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AttributeBoost;

	    int field_18;
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

	    char field_18;
	};

	struct SavantBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SavantBoost;

	    char field_18;
	};

	struct MinimumRollResultBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::MinimumRollResultBoost;

	    char field_18;
	    char field_19;
	};

	struct CharacterWeaponDamageBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CharacterWeaponDamageBoost;

	    __int64 field_18;
	    char field_20;
	};

	struct ProjectileDeflectBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ProjectileDeflectBoost;

	    char field_18;
	};

	struct AbilityOverrideMinimumBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::AbilityOverrideMinimumBoost;

		char field_18;
		int field_1C;
	};

	struct ACOverrideMinimumBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ACOverrideMinimumBoost;

	    int field_18;
	    char field_1C;
	};

	struct FallDamageMultiplierBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::FallDamageMultiplierBoost;

	    int field_18;
	};
}
