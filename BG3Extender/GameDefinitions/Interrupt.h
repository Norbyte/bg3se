#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Hit.h>
#include <GameDefinitions/Stats/Common.h>

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
	HashMap<uint8_t, HashMap<DamageType, TotalRollAdjustments>> TotalAdjustments;
	HashMap<uint8_t, HashMap<DamageType, int32_t>> field_40;
	Array<RerollCondition> RerollConditions;
	HashSet<DamageType> DamageTypes;
	HashSet<DamageType> DamageTypes2;
};


struct AppliedChange
{
	uint8_t Flags;
	Array<RollAdjustments> Adjustments;
	HashMap<DamageFunctorKey, DamageRollAdjustments> DamageRollAdjustments;
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
	FixedString Cause;
	Guid SpellCastGuid;
	SpellId Spell;
};

struct CastHitEvent
{
	FixedString Cause;
	Guid SpellCastGuid;
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
	Guid SpellCastGuid;
	ConditionRollType ConditionRollType;
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
	ConditionRollType ConditionRollType;
	RollData Roll;
	int field_50;
	AbilityId Ability;
	bool field_55;
};

struct EnterAttackRangeEvent
{
	FixedString Event;
	glm::vec3 Position;
};

struct LeaveAttackRangeEvent
{
	FixedString Event;
	glm::vec3 Position;
};

struct SpellPreDamageEvent
{
	Guid field_0;
	FixedString Cause;
	Guid SpellCastGuid;
	DamageType DamageType;
	DamageFlags DamageEffectFlags;
	AbilityId Ability;
	SpellAttackType SpellAttackType;
	Dependency Dependency1;
	Dependency Dependency2;
	SpellId Spell;
	uint8_t field_90;
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
	EntityHandle Observer;
	EntityHandle Interrupt;
};

struct InterruptEvent
{
	std::variant<SpellCastEvent, CastHitEvent, SpellRollEvent, ConditionalRollEvent, EnterAttackRangeEvent, LeaveAttackRangeEvent, SpellPreDamageEvent, PlaceholderSpellRollEvent, ConditionResultEvent, StatusAppliedEvent, DeathEvent, DebugEvent> Event;
	EntityHandle Source;
	EntityHandle SourceProxy;
	EntityHandle Target;
	EntityHandle TargetProxy;
	Array<Guid> field_D0;
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

struct UndecidedEventWithId
{
	glm::vec3 field_0;
	float field_C;
	InterruptEvent Event;
	__int64 field_108;
	InterruptEvent Event2;
	HashMap<EntityHandle, HashSet<EntityHandle>> field_208;
};

struct DelayedTargetHitInterruptEvent
{
	__int64 field_0;
	__int64 field_8;
	__int64 field_10;
	HitDesc Hit;
	AttackDesc Attack;
	__int64 field_1E8;
	__int64 field_1F0;
	__int64 field_1F8;
	__int64 field_200;
	__int64 field_208;
	__int64 field_210;
	SpellId Spell;
};


struct PausedAnimationEvent
{
	FixedString field_0;
	int field_4;
};

struct PrecalculatedInterruptEvent : public InterruptEvent
{
	HashMap<EntityHandle, HashSet<EntityHandle>> field_F8;
	HashMap<EntityHandle, HashSet<EntityHandle>> field_138;
};

struct ResolvedData
{
	__int64 field_0;
	Array<RerollValue> RerollValues;
};

struct InterruptUsageEntry
{
	__int64 field_0;
	ConditionRolls ConditionRolls;
	__int64 field_18;
	Array<RollAdjustmentMetadata> RollAdjustmentMetadata;
	__int64 field_30;
	__int64 field_38;
	Array<RerollCondition> RerollConditions;
	Array<RerollValue> RerollValues;
	__int64 field_60;
	__int64 field_68;
	__int64 field_70;
};

struct ExecutedDependency
{
	// UNKNOWN
};

struct InterruptUsage
{
	Array<stats::ActionResourceCost> Costs;
	HashMap<EntityHandle, HashMap<InterruptEvent, InterruptUsageEntry>> Usage;
};

struct AnimationInterruptData
{
	Array<InterruptEvent> Events;
	Array<PrecalculatedInterruptEvent> PrecalculatedInterrupts;
	__int64 field_20;
	HashMap<Guid, ResolvedData> Resolved;
	HashMap<EntityHandle, HashMap<InterruptEvent, InterruptUsageEntry>> Usage;
	[[bg3::hidden]] HashMap<Guid, ExecutedDependency> ExecutedDependencies;
	HashMap<InterruptEvent, HashMap<EntityHandle, EntityHandle>> field_E8;
	int32_t field_128;
	float field_12C;
	HashMap<Guid, RollAdjustments> RollAdjustments;
	HashMap<DamageFunctorKey, DamageRollAdjustments> DamageRollAdjustments;
	HashMap<EntityHandle, InterruptUsage> InterruptUsage;
	[[bg3::hidden]] HashMap<Guid, ExecutedDependency> ExecutedDependencies2;
};

END_NS()

BEGIN_SE()

template <>
inline uint64_t HashMapHash<interrupt::InterruptEvent>(interrupt::InterruptEvent const& v)
{
	// FIXME - needs adjustment depending on variant
	return HashMulti(v.Source, v.Target);
}

template <>
inline uint64_t HashMapHash<interrupt::DamageFunctorKey>(interrupt::DamageFunctorKey const& v)
{
	return HashMulti(v.field_0, v.field_10);
}

END_SE()
