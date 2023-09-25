#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct DamageSums
	{
		int TotalDamageDone;
		int TotalHealDone;
		int8_t DamagePercentage;
	};


	struct DamagePair
	{
		int32_t Amount;
		DamageType DamageType;
	};

	struct DiceValues
	{
		int8_t DiceValue;
		uint8_t AmountOfDices;
		int DiceAdditionalValue;
		uint8_t field_8;
	};

	struct DamageModifierMetadata
	{

		uint8_t MetadataType;
		int Value;
		DamageType DamageType;
		uint8_t SourceType;
		union {
			DiceValues Dice;
			int32_t Source;
		};
	};

	struct StatsRollRoll
	{
		DiceValues Roll;
		stats::RollType RollType;
		bool Advantage;
		bool Disadvantage;
	};

	struct StatsRollResult
	{
		int Total;
		int DiceTotal;
		int DiscardedDiceTotal;
		bool Critical;
	};

	struct ResolvedRollBonus
{
		uint8_t DiceSetType;
		uint8_t NumDice;
		int ResolvedRollBonus;
	};

	struct StatsRollMetadata
	{
		int ProficiencyBonus;
		int RollBonus;
		MultiHashMap<AbilityId, int32_t> AbilityBoosts;
		MultiHashMap<SkillId, int32_t> SkillBonuses;
		bool AutoSkillCheckFail;
		bool AutoAbilityCheckFail;
		bool AutoAbilitySavingThrowFail;
		bool HasCustomMetadata;
		bool IsCritical;
		Array<ResolvedRollBonus> ResolvedRollBonuses;
	};

	struct StatsRollBase
	{
		StatsRollRoll Roll;
		StatsRollResult Result;
		StatsRollMetadata Metadata;
	};


	struct StatsRollType0 : StatsRollBase
	{
		uint8_t Type;
	};

	struct StatsRollType1
	{
		void* CachedStatExpression;
		STDString StatExpression;
		Array<int32_t> IntParams;
		Array<StatsRollType0> RollParams;
		Array<DamageType> DamageTypeParams;
		int IntIndex;
		int RollIndex;
		int DamageTypeIndex;
	};


	struct ConditionRoll
	{
		uint8_t DataType;
		stats::RollType RollType; // Maybe?
		StatsRollType0 Roll;
		int Difficulty;
	};


	struct DamageResistance
	{
		int ResistanceType;
		DamageType DamageType;
		int Damage;
	};

	struct Hit
	{
		struct Subobj
		{
			RefMap<uint8_t, ObjectSet<StatsRollType0>> DamageRolls;
			ObjectSet<DamageModifierMetadata> ModifierMetadata;
			StatsRollType1 ConditionRoll;
			ObjectSet<DamageModifierMetadata> ModifierMetadata2;
			ObjectSet<DamageResistance> Resistances;
			int AdditionalDamage;
			int TotalDamage;
			int FinalDamage;
			RefMap<DamageType, int32_t> TotalDamagePerType;
			RefMap<DamageType, int32_t> FinalDamagePerType;
		};

		int TotalDamageDone;
		stats::DeathType DeathType;
		DamageType DamageType;
		CauseType CauseType;
		glm::vec3 ImpactPosition;
		glm::vec3 ImpactDirection;
		float ImpactForce;
		int ArmorAbsorption;
		int LifeSteal;
		DamageFlags EffectFlags;
		HitWith HitWith;
		AbilityId AttackRollAbility;
		AbilityId SaveAbility;
		EntityHandle Inflicter;
		EntityHandle InflicterOwner;
		EntityHandle Throwing;
		int StoryActionId;
		ObjectSet<ConditionRoll> ConditionRolls;
		Subobj SomeSubobj;
		uint8_t SpellAttackType;
		FixedString SpellId;
		int32_t field_180;
		EntityHandle field_188;
		uint8_t field_190;
		uint8_t SpellSchool;
		int SpellLevel;
		int SpellPowerLevel;
		int TotalHealDone;
		uint8_t HitDescFlags;
		ObjectSet<DamagePair> DamageList;
	};

	struct NewHit : public Hit
	{
		DamageSums DamageSums;
	};
}
