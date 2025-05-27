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
    uint8_t Flags{ 0 };
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
    [[bg3::legacy(Cause)]] FixedString CauseSpell;
    Guid SpellCastGuid;
    DamageFlags DamageEffectFlags;
    uint8_t SpellAttackType;
    [[bg3::legacy(field_20)]] int32_t DamageDone;
    Array<DamagePair> DamageRolls;
    [[bg3::legacy(field_38)]] EntityHandle ThrownObject;
    SpellId Spell;
    [[bg3::legacy(field_68)]] AbilityId AbilitySavingThrow;
    uint8_t HitDescFlags;
};

struct SpellRollEvent
{
    Guid RollUuid;
    [[bg3::legacy(field_10)]] FixedString CauseSpell;
    Guid SpellCastGuid;
    ConditionRollType ConditionRollType;
    RollData Roll;
    int Difficulty;
    [[bg3::legacy(field_48)]] std::optional<int> CriticalRollThreshold;
    AbilityId Ability;
    SpellAttackType SpellAttackType;
    [[bg3::legacy(field_52)]] bool WasHit;
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
    [[bg3::legacy(field_0)]] Guid DamageFunctorId;
    [[bg3::legacy(Cause)]] FixedString CauseSpell;
    Guid SpellCastGuid;
    DamageType DamageType;
    DamageFlags DamageEffectFlags;
    AbilityId Ability;
    SpellAttackType SpellAttackType;
    Dependency Dependency1;
    Dependency Dependency2;
    SpellId Spell;
    uint32_t field_90;
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
    [[bg3::legacy(field_D0)]] Array<Guid> Targets;
    std::optional<glm::vec3> SourcePos;
    std::optional<glm::vec3> TargetPos;
    [[bg3::legacy(field_100)]] bool SwapSourceTarget{ false };

    // FIXME - needs adjustment depending on variant
    inline bool operator == (InterruptEvent const& o) const
    {
        return Source == o.Source
            && Target == o.Target;
    }
};

struct UndecidedEvent
{
    InterruptEvent Event;
    bool HasReplacement;
    InterruptEvent Replacement;
    HashMap<EntityHandle, HashSet<EntityHandle>> UndecidedInterrupts;
};

struct UndecidedEventWithId
{
    glm::vec3 field_0;
    float field_C;
    UndecidedEvent Event;
};

struct DelayedTargetHitInterruptEvent
{
    Guid field_0;
    uint8_t field_10;
    uint32_t field_14;
    HitDesc Hit;
    AttackDesc Attack;
    [[bg3::legacy(field_1E8)]] EntityHandle Target;
    [[bg3::legacy(field_1F0)]] EntityHandle Source;
    [[bg3::legacy(field_1F8)]] std::optional<glm::vec3> TargetPos;
    [[bg3::legacy(field_208)]] std::optional<glm::vec3> SourcePos;
    SpellId Spell;
};


struct PausedAnimationEvent
{
    FixedString TextKey;
    bool PauseFinishedAnimations;
};

struct PrecalculatedInterruptEvent : public InterruptEvent
{
    [[bg3::legacy(field_F8)]] HashMap<EntityHandle, HashSet<EntityHandle>> AutoInterrupts;
    [[bg3::legacy(field_138)]] HashMap<EntityHandle, HashSet<EntityHandle>> PendingInterrupts;
};

struct ResolvedData
{
    __int64 field_0;
    Array<RerollValue> RerollValues;
};

struct InterruptUsageEntry
{
    uint8_t field_0;
    ConditionRolls ConditionRolls;
    RollAdjustments RollAdjustments;
    uint8_t field_70;
};

struct ExecutedDependency
{
    uint32_t field_0;
};

struct InterruptUsage
{
    Array<stats::ActionResourceCost> Costs;
    HashMap<EntityHandle, HashMap<InterruptEvent, InterruptUsageEntry>> Usage;
};

struct PrecalculatedAnimationInterruptData
{
    Array<InterruptEvent> Events;
    Array<PrecalculatedInterruptEvent> PrecalculatedInterrupts;
    uint8_t field_20;
    HashMap<Guid, ResolvedData> Resolved;
    HashMap<EntityHandle, HashMap<InterruptEvent, InterruptUsageEntry>> Usage;
    HashMap<Guid, ExecutedDependency> ExecutedDependencies;
};

struct AnimationInterruptData : public PrecalculatedAnimationInterruptData
{
    HashMap<InterruptEvent, HashMap<EntityHandle, HashSet<EntityHandle>>> Interruptors;
    int32_t field_128;
    float field_12C;
    HashMap<Guid, RollAdjustments> RollAdjustments;
    HashMap<DamageFunctorKey, DamageRollAdjustments> DamageRollAdjustments;
    HashMap<EntityHandle, InterruptUsage> InterruptUsage;
    HashMap<Guid, ExecutedDependency> ExecutedDependencies2;
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
