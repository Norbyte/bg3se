#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

struct ActionOriginator
{
	Guid ActionGuid;
	FixedString PassiveId;
	FixedString StatusId;
	FixedString InterruptId;
	bool CanApplyConcentration;
};

struct RollDefinition
{
	uint8_t field_0;
	uint8_t field_1;
	int32_t field_4;
	uint8_t field_8;
};

struct ResourceRollDefinition
{
	Guid field_0;
	uint8_t field_10;
};

struct StatsExpressionParam
{
	struct SubType1
	{
		inline ~SubType1()
		{
			if (Type == StatsExpressionParamType2::STDString) {
				String.~basic_string();
			}
		}

		inline SubType1()
			: Type(StatsExpressionParamType2::StatsExpressionVariableData), StatsExpressionVariableData(0)
		{}

		inline SubType1(SubType1 const& o)
		{
			Type = o.Type;
			if (Type == StatsExpressionParamType2::STDString) {
				String = o.String;
			} else {
				StatsExpressionVariableData = o.StatsExpressionVariableData;
			}
		}

		inline SubType1& operator = (SubType1 const& o)
		{
			Type = o.Type;
			if (Type == StatsExpressionParamType2::STDString) {
				String = o.String;
			} else {
				StatsExpressionVariableData = o.StatsExpressionVariableData;
			}

			return *this;
		}

		union {
			uint8_t StatsExpressionVariableData;
			AbilityId Ability;
			SkillId Skill;
			uint8_t StatusGroup;
			STDString String;
		};
		StatsExpressionParamType2 Type;
	};
		
	struct Param
	{
		inline ~Param()
		{
			if (Type == StatsExpressionParamType::Type1) {
				Type1.~SubType1();
			}
		}

		inline Param()
			: Type(StatsExpressionParamType::StatsExpressionType), StatsExpressionType(0)
		{}

		inline Param(Param const& o)
		{
			Type = o.Type;
			if (Type == StatsExpressionParamType::Type1) {
				Type1 = o.Type1;
			} else {
				ResourceRoll = o.ResourceRoll;
			}
		}

		inline Param& operator = (Param const& o)
		{
			Type = o.Type;
			if (Type == StatsExpressionParamType::Type1) {
				Type1 = o.Type1;
			} else {
				ResourceRoll = o.ResourceRoll;
			}

			return *this;
		}

		union {
			uint8_t StatsExpressionType;
			SubType1 Type1;
			uint8_t StatsExpressionVariableDataType;
			uint8_t StatsExpressionVariableDataModifier;
			RollDefinition Roll;
			ResourceRollDefinition ResourceRoll;
			uint8_t StatsContextType;
			int32_t Int;
			bool Bool;
		};
		StatsExpressionParamType Type;
	};

	Array<Param> ExpressionParams;
	STDString Code;
};

struct StatsExpressionParamEx : public StatsExpressionParam
{
	int FastLock;
};

struct LuaExpressionManager : public ProtectedGameObject<LuaExpressionManager>
{
	void* VMT;
	RefMap<Guid, StatsExpressionParamEx*> Expressions;
	char field_18;
};

struct DamageSums
{
	int TotalDamageDone;
	int TotalHealDone;
	int8_t DamagePercentage;
	int8_t field_9;
	int8_t field_A;
};


struct DamagePair
{
	int32_t Amount;
	DamageType DamageType;
};

struct DiceValues
{
	DiceSizeId DiceValue;
	uint8_t AmountOfDices;
	int DiceAdditionalValue;
	uint8_t field_8;
};

struct StatsRollRoll
{
	// ??? not sure
	// __int64 field_0;
	// int field_8;
	DiceValues Roll;
	stats::RollType RollType;
	bool Advantage;
	bool Disadvantage;
	Array<int16_t> field_10;
};

struct StatsRollResult
{
	int Total;
	int DiceTotal;
	int DiscardedDiceTotal;
	bool Critical;
	uint32_t field_10;
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
	HitWith HitWith;
	AbilityId AttackRollAbility;
	AbilityId SaveAbility;
	EntityHandle Inflicter;
	EntityHandle InflicterOwner;
	EntityHandle Throwing;
	int StoryActionId;
	Array<ConditionRoll> ConditionRolls;
	ResultMetadata Results;
	SpellAttackType SpellAttackType;
	FixedString SpellId;
	Guid field_140;
	FixedString field_150;
	EntityHandle field_158;
	uint8_t field_160;
	SpellSchoolId SpellSchool;
	int SpellLevel;
	int SpellPowerLevel;
	int TotalHealDone;
	uint8_t HitDescFlags;
	uint8_t field_171;
	int field_174;
	int field_178;
	int field_17C;
	int field_180;
	int field_184;
	float field_188;
	int field_18C;
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
