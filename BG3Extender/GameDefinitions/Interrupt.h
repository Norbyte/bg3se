#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Hit.h>

BEGIN_NS(interrupt)

struct Dependency
{
	Guid field_0;
	uint8_t field_10;
};

struct DamageFunctorKey
{
	Guid field_0;
	EntityHandle field_10;

	inline bool operator == (DamageFunctorKey const& o) const
	{
		return field_0 == o.field_0
			&& field_10 == o.field_10;
	}
};

struct RollAdjustmentMetadata
{
	int Adjustment;
	TranslatedString Source;
};

struct TotalRollAdjustments
{
	int field_0;
	int field_4;
	Array<RollAdjustmentMetadata> Metadata;
};

struct RollAdjustments
{
	int BaseAdjustment;
	[[bg3::legacy(ResolvedRolls)]] Array<FixedRollBonus> FixedRollBonuses;
	std::optional<int> FixedAdjustment;
	uint8_t RerollType;
	Array<RerollCondition> RerollConditions;
	Array<RerollValue> RerollValues;
	std::optional<int> ResolvedRoll;
	uint8_t UseMaxRoll;
};

struct DamageRollAdjustments
{
	// StatsFunctorAdjustRollMode
	MultiHashMap<uint8_t, MultiHashMap<DamageType, TotalRollAdjustments>> TotalAdjustments;
	MultiHashMap<uint8_t, MultiHashMap<DamageType, int32_t>> field_40;
	Array<RerollCondition> RerollConditions;
	MultiHashSet<DamageType> DamageTypes;
	MultiHashSet<DamageType> DamageTypes2;
};


struct AppliedChange
{
	uint8_t Flags;
	Array<RollAdjustments> Adjustments;
	MultiHashMap<DamageFunctorKey, DamageRollAdjustments> DamageRollAdjustments;
};

struct RollData
{
	uint8_t DiceValue;
	uint8_t AmountOfDices;
	int DiceAdditionalValue;
	int Total;
	int NaturalRoll;
	uint8_t ResultCritical;
	uint8_t RerollType;
	int InitialReroll_M;
};


struct SpellCastEvent
{
	FixedString field_0;
	Guid field_8;
	SpellId field_18;
};

struct CastHitEvent
{
	FixedString field_0;
	Guid field_8;
	DamageFlags DamageEffectFlags;
	uint8_t SpellAttackType;
	int field_20;
	Array<DamagePair> DamageRolls;
	EntityHandle field_38;
	SpellId Spell;
	uint8_t field_68;
	uint8_t HitDescFlags;
};

struct SpellRollEvent
{
	Guid RollUuid;
	FixedString field_10;
	Guid field_18;
	uint8_t ConditionRollType;
	RollData Roll;
	int Difficulty;
	std::optional<int> field_48;
	AbilityId Ability;
	SpellAttackType SpellAttackType;
	bool field_52;
	Guid field_58;
	SpellId Spell;
};

struct ConditionalRollEvent
{
	Guid RollUuid;
	Guid field_10;
	__int64 field_20;
	EntityHandle field_28;
	FixedString field_30;
	uint8_t ConditionRollType;
	RollData Roll;
	int field_50;
	AbilityId Ability;
	bool field_55;
};

struct EnterAttackRangeEvent
{
	FixedString field_0;
	glm::vec3 field_4;
};

struct LeaveAttackRangeEvent
{
	FixedString field_0;
	glm::vec3 field_4;
};

struct SpellPreDamageEvent
{
	Guid field_0;
	FixedString field_10;
	Guid field_18;
	STDString field_28;
	DamageType DamageType;
	DamageFlags DamageEffectFlags;
	AbilityId Ability;
	SpellAttackType SpellAttackType;
	Dependency Dependency1;
	Dependency Dependency2;
	SpellId Spell;
};

struct PlaceholderSpellRollEvent
{
	Guid field_0;
	Guid field_10;
	FixedString field_20;
	Guid field_28;
	Dependency Dependency;
	stats::PropertyContext StatsFunctorContext;
	FixedString field_58;
	int field_5C;
};

struct ConditionResultEvent
{
	Guid field_0;
	FixedString field_10;
	Guid field_18;
	Guid field_28;
	int field_38;
	stats::PropertyContext StatsFunctorContext;
	char field_48;
	DamageFlags DamageEffectFlags;
};

struct StatusAppliedEvent
{
	FixedString field_0;
};

struct DeathEvent
{
	uint8_t field_0;
};

struct DebugEvent
{
	uint8_t field_0;
};


struct ActionEntry
{
	EntityHandle field_0;
	EntityHandle field_8;
};

struct InterruptEvent
{
	std::variant<SpellCastEvent, CastHitEvent, SpellRollEvent, ConditionalRollEvent, EnterAttackRangeEvent, LeaveAttackRangeEvent, SpellPreDamageEvent, PlaceholderSpellRollEvent, ConditionResultEvent, StatusAppliedEvent, DeathEvent, DebugEvent> Variant;
	EntityHandle Source;
	EntityHandle SourceProxy;
	EntityHandle Target;
	EntityHandle TargetProxy;
	Array<ActionEntry> field_D0;
	std::optional<glm::vec3> SourcePos;
	std::optional<glm::vec3> TargetPos;
	uint8_t field_100;

	// FIXME - needs adjustment depending on variant
	inline bool operator == (InterruptEvent const& o) const
	{
		return Source == o.Source
			&& Target == o.Target;
	}
};

END_NS()

BEGIN_SE()

template <>
inline uint64_t MultiHashMapHash<interrupt::InterruptEvent>(interrupt::InterruptEvent const& v)
{
	// FIXME - needs adjustment depending on variant
	return HashMulti(v.Source, v.Target);
}

template <>
inline uint64_t MultiHashMapHash<interrupt::DamageFunctorKey>(interrupt::DamageFunctorKey const& v)
{
	return HashMulti(v.field_0, v.field_10);
}

END_SE()
