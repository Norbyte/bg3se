#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Stats/Expression.h>

BEGIN_SE()

struct DamageSums
{
	int TotalDamageDone;
	int TotalHealDone;
	int8_t DamagePercentage;
	int8_t field_9;
	int8_t field_A;
};

struct StatsRollRoll
{
	DiceValues Roll;
	stats::RollType RollType;
	bool Advantage;
	bool Disadvantage;
	Array<int16_t> field_10;
};

struct StatsRollResult
{
	int Total;
	int NaturalRoll;
	int DiscardedDiceTotal;
	RollCritical Critical;
	uint32_t DiceSize;
	uint8_t field_14;
	Array<int16_t> field_18;
};

struct ResolvedRollBonus
{
	DiceSizeId DiceSize;
	uint8_t NumDice;
	int ResolvedRollBonus;
	TranslatedString Description;
};

struct ResolvedUnknown
{
	int field_0;
	TranslatedString Description;
};

struct StatsRollMetadata
{
	int ProficiencyBonus;
	int RollBonus;
	__int64 field_8;
	int field_10;
	int field_14;
	MultiHashMap<AbilityId, int32_t> AbilityBoosts;
	MultiHashMap<SkillId, int32_t> SkillBonuses;
	bool AutoSkillCheckFail;
	bool AutoAbilityCheckFail;
	bool AutoAbilitySavingThrowFail;
	bool HasCustomMetadata;
	bool IsCritical;
	Array<ResolvedRollBonus> ResolvedRollBonuses;
	Array<ResolvedUnknown> ResolvedUnknowns;
};

struct StatsRollType0
{
	StatsRollRoll Roll;
	StatsRollResult Result;
	StatsRollMetadata Metadata;
};

struct StatsRollType1
{
	[[bg3::hidden]]
	StatsExpressionParamEx* CachedStatExpression;
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
	//+ P_BITMASK(RollType)
	ConditionRollType RollType;
	std::variant<StatsRollType0, StatsRollType1> Roll;
	int Difficulty;
	Guid field_120;
	bool field_130;
	AbilityId Ability;
	SkillId Skill;
};

struct DamageModifierMetadata
{
	uint8_t MetadataType;
	int Value;
	DamageType DamageType;
	uint8_t SourceType;
	std::variant<DiceValues, int32_t, StatsRollType1> Argument;
	TranslatedString Description;
	FixedString Description2;
};

struct DamageResistance
{
	uint32_t Flags;
	DamageModifierMetadata Meta;
};

struct Hit
{
	struct ResultMetadata
	{
		RefMap<DamageType, Array<StatsRollType0>> DamageRolls;
		Array<DamageModifierMetadata> Modifiers;
		StatsRollType1 ConditionRoll;
		Array<DamageModifierMetadata> Modifiers2;
		Array<DamageResistance> Resistances;
		int AdditionalDamage;
		int TotalDamage;
		int FinalDamage;
		RefMap<DamageType, int32_t> TotalDamagePerType;
		RefMap<DamageType, int32_t> FinalDamagePerType;
		Guid field_D0;
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
	// TODO - need to remap DamageFlags
	uint32_t EffectFlags;
	EntityHandle Inflicter;
	EntityHandle InflicterOwner;
	EntityHandle Throwing;
	int StoryActionId;
	HitWith HitWith;
	AbilityId AttackRollAbility;
	AbilityId SaveAbility;
	uint8_t field_4F;
	Array<ConditionRoll> ConditionRolls;
	ResultMetadata Results;
	Guid SpellCastGuid;
	FixedString SpellId;
	FixedString field_150;
	EntityHandle field_158;
	uint8_t field_160;
	SpellSchoolId SpellSchool;
	uint8_t HitDescFlags;
	uint8_t AttackFlags;
	int SpellLevel;
	int SpellPowerLevel;
	int TotalHealDone;
	int field_174;
	int OriginalDamageValue;
	int field_178;
	int field_17C;
	int field_180;
	int field_184;
	uint8_t field_188;
	Array<glm::vec3> field_190;
	Array<DamagePair> DamageList;
};

struct HitResultData
{
	Array<int32_t> field_0;
	Array<int32_t> field_10;
	Array<uint8_t> field_20;
	Array<int32_t> field_30;
};

struct HitResult
{
	Hit Hit;
	DamageSums DamageSums;
	Array<DamagePair> DamageList;
	HitResultData Results;
	uint32_t NumConditionRolls;
};

END_SE()
