#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
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
		uint8_t RollType;
		bool Advantage;
		bool Disadvantage;
	};

	struct StatsRollResult
	{
		int Total;
		int DiceTotal;
		bool Critical;
	};

	struct StatsRollBoosts
	{
		int ProficiencyBonus;
		int RollBonus;
		int ThrownDamage;
		VirtualMultiHashMap<AbilityId, int32_t> AbilityBoosts;
		VirtualMultiHashMap<SkillId, int32_t> SkillBonuses;
	};

	struct StatsRollBase
	{
		StatsRollRoll Roll;
		StatsRollResult Result;
		StatsRollBoosts Boosts;
		bool AutoSkillCheckFail;
		bool AutoAbilityCheckFail;
		bool AutoAbilitySavingThrowFail;
		bool HasCustomMetadata;
		bool IsCritical;
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
		Array<void*> RollParams;
		Array<DamageType> DamageTypeParams;
		int IntIndex;
		int RollIndex;
		int DamageTypeIndex;
	};


	struct ConditionRoll
	{
		uint8_t DataType;
		uint8_t RollType;
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
		int TotalDamagePerceived;
		DeathType DeathType;
		DamageType DamageType;
		CauseType CauseType;
		glm::vec3 ImpactPosition;
		glm::vec3 ImpactDirection;
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
		uint8_t SpellSchool;
		int SpellLevel;
		int SpellPowerLevel;
		int TotalHealDone;
		bool InstantKill;
		bool FirstAttack;
		ObjectSet<DamagePair> DamageList;
	};

	struct NewHit : public Hit
	{
		DamageSums DamageSums;
	};
}
