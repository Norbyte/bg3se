#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct DamageSums : public HasObjectProxy
	{
		int TotalDamageDone;
		int TotalHealDone;
		int8_t DamagePercentage;
	};


	struct DamagePair : public HasObjectProxy
	{
		int32_t Amount;
		DamageType DamageType;
	};

	struct DiceValues : public HasObjectProxy
	{
		int8_t DiceValue;
		uint8_t AmountOfDices;
		int DiceAdditionalValue;
	};

	struct DamageModifierMetadata : public HasObjectProxy
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

	struct StatsRollRoll : public HasObjectProxy
	{
		DiceValues Roll;
		RollTypeId RollType;
		bool Advantage;
		bool Disadvantage;
	};

	struct StatsRollResult : public HasObjectProxy
	{
		int Total;
		int DiceTotal;
		int DiscardedDiceTotal;
		bool Critical;
	};

	struct ResolvedRollBonus : public HasObjectProxy
{
		uint8_t DiceSetType;
		uint8_t NumDice;
		int ResolvedRollBonus;
	};

	struct StatsRollMetadata : public HasObjectProxy
	{
		int ProficiencyBonus;
		int RollBonus;
		VirtualMultiHashMap<AbilityId, int32_t> AbilityBoosts;
		VirtualMultiHashMap<SkillId, int32_t> SkillBonuses;
		bool AutoSkillCheckFail;
		bool AutoAbilityCheckFail;
		bool AutoAbilitySavingThrowFail;
		bool HasCustomMetadata;
		bool IsCritical;
		Array<ResolvedRollBonus> ResolvedRollBonuses;
	};

	struct StatsRollBase : public HasObjectProxy
	{
		StatsRollRoll Roll;
		StatsRollResult Result;
		StatsRollMetadata Metadata;
	};


	struct StatsRollType0 : StatsRollBase
	{
		uint8_t Type;
	};

	struct StatsRollType1 : public HasObjectProxy
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


	struct ConditionRoll : public HasObjectProxy
	{
		uint8_t DataType;
		RollTypeId RollType; // Maybe?
		StatsRollType0 Roll;
		int Difficulty;
	};


	struct DamageResistance : public HasObjectProxy
	{
		int ResistanceType;
		DamageType DamageType;
		int Damage;
	};

	struct Hit : public HasObjectProxy
	{
		struct Subobj : public HasObjectProxy
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
