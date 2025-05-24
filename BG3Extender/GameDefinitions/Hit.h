#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Stats/Expression.h>

BEGIN_SE()

struct AttackDesc
{
    int TotalDamageDone{ 0 };
    int TotalHealDone{ 0 };
    [[bg3::legacy(DamagePercentage)]] uint8_t InitialHPPercentage{ 0 };
    uint8_t field_9{ 0 };
    Array<DamagePair> DamageList;
};

struct RerollCondition
{
    uint8_t RollValue;
    bool KeepNew;
};

struct RerollValue
{
    uint8_t RollValue;
    uint8_t RerollType;
};

struct Roll
{
    RollDefinition Roll;
    stats::RollType RollType;
    bool Advantage;
    bool Disadvantage;
    Array<RerollCondition> RerollConditions;
};

struct StatsRollResult
{
    int Total;
    int NaturalRoll;
    int DiscardedDiceTotal;
    RollCritical Critical;
    std::optional<uint32_t> CriticalThreshold;
    Array<RerollValue> RollsCount;
};

struct ResolvedRollBonus
{
    DiceSizeId DiceSize;
    uint8_t NumDice;
    int ResolvedRollBonus;
    [[bg3::legacy(Description)]] TranslatedString SourceName;
};

struct FixedRollBonus
{
    [[bg3::legacy(field_0)]] int RollBonus;
    [[bg3::legacy(Description)]] TranslatedString SourceName;
};

struct StatsRollMetadata
{
    int ProficiencyBonus;
    int RollBonus;
    int HighGroundBonus;
    int LowGroundPenalty;
    [[bg3::legacy(field_10)]] int BaseUnarmedDamage;
    HashMap<AbilityId, int32_t> AbilityBoosts;
    HashMap<SkillId, int32_t> SkillBonuses;
    bool AutoSkillCheckFail;
    bool AutoAbilityCheckFail;
    bool AutoAbilitySavingThrowFail;
    bool HasCustomMetadata;
    bool IsCritical;
    Array<ResolvedRollBonus> ResolvedRollBonuses;
    [[bg3::legacy(ResolvedUnknowns)]] Array<FixedRollBonus> FixedRollBonuses;
};

struct StatsRoll
{
    Roll Roll;
    StatsRollResult Result;
    StatsRollMetadata Metadata;
};

struct StatsExpressionResolved
{
    [[bg3::hidden]]
    StatsExpressionParamEx* CachedStatExpression;
    STDString StatExpression;
    Array<int32_t> IntParams;
    Array<StatsRoll> RollParams;
    Array<DamageType> DamageTypeParams;
    int IntIndex;
    int RollIndex;
    int DamageTypeIndex;
};

struct ConditionRoll
{
    uint8_t DataType;
    //# P_BITMASK(RollType)
    ConditionRollType RollType;
    std::variant<StatsRoll, StatsExpressionResolved> Roll;
    int Difficulty;
    Guid RollUuid;
    bool SwappedSourceAndTarget;
    AbilityId Ability;
    SkillId Skill;
};


struct ConditionRolls
{
    Array<ConditionRoll> Rolls;
};

struct DamageModifierMetadata
{
    uint8_t MetadataType;
    int Value;
    DamageType DamageType;
    uint8_t SourceType;
    [[bg3::legacy(Argument)]] std::variant<int32_t, RollDefinition, StatsExpressionResolved> Source;
    [[bg3::legacy(Description)]] TranslatedString SourceName;
    [[bg3::legacy(Description2)]] FixedString SourceId;
};

struct DamageResistance
{
    [[bg3::legacy(Flags)]] uint8_t Type;
    DamageModifierMetadata Meta;
};

struct StatsDamage
{
    LegacyRefMap<DamageType, Array<StatsRoll>> DamageRolls;
    Array<DamageModifierMetadata> Modifiers;
    StatsExpressionResolved ConditionRoll;
    Array<DamageModifierMetadata> Modifiers2;
    Array<DamageResistance> Resistances;
    int AdditionalDamage{ 0 };
    int TotalDamage{ 0 };
    int FinalDamage{ 0 };
    LegacyRefMap<DamageType, int32_t> TotalDamagePerType;
    LegacyRefMap<DamageType, int32_t> FinalDamagePerType;
    [[bg3::legacy(field_D0)]] uint32_t Multiplier{ 1 };
    [[bg3::legacy(field_D4)]] uint32_t BaseValue{ 1 };
    [[bg3::legacy(field_D8)]] uint32_t SecondaryValue{ 1 };
    [[bg3::legacy(field_DC)]] uint8_t DamageMultiplierType;
};

struct HitDamageOverride
{
    uint8_t DamageType;
    int OriginalValue;
    int OverriddenValue;
};

struct HitDesc
{
    int TotalDamageDone{ 0 };
    stats::DeathType DeathType{ stats::DeathType::None };
    DamageType DamageType{ DamageType::None };
    CauseType CauseType{ CauseType::None };
    glm::vec3 ImpactPosition;
    glm::vec3 ImpactDirection;
    float ImpactForce{ 0.0f };
    int ArmorAbsorption{ 0 };
    int LifeSteal{ 0 };
    // TODO - need to remap DamageFlags
    uint32_t EffectFlags{ 0 };
    EntityHandle Inflicter;
    EntityHandle InflicterOwner;
    EntityHandle Throwing;
    int StoryActionId{ -1 };
    HitWith HitWith{ HitWith::None };
    AbilityId AttackRollAbility{ AbilityId::None };
    AbilityId SaveAbility{ AbilityId::None };
    [[bg3::legacy(field_4F)]] uint8_t SpellAttackType{ 0 };
    Array<ConditionRoll> ConditionRolls;
    [[bg3::legacy(Results)]] StatsDamage Damage;
    Guid SpellCastGuid;
    FixedString SpellId;
    [[bg3::legacy(field_150)]] FixedString StatusId;
    [[bg3::legacy(field_158)]] EntityHandle StatusEntity;
    uint8_t field_160{ 0 };
    SpellSchoolId SpellSchool{ SpellSchoolId::None };
    [[bg3::legacy(HitDescFlags)]] uint8_t HealingTypes{ 1 };
    AttackFlags AttackFlags{ 0 };
    int SpellLevel{ 0 };
    int SpellPowerLevel{ 0 };
    int TotalHealDone{ 0 };
    [[bg3::legacy(field_174)]] float RedirectedDamage{ 0.0f };
    int OriginalDamageValue{ 0 };
    [[bg3::legacy(field_178)]] float FallHeight{ 0.0f };
    [[bg3::legacy(field_17C)]] float FallDamageMultiplier{ 0.0f };
    [[bg3::legacy(field_180)]] float FallMaxDamage{ 0.0f };
    [[bg3::legacy(field_184)]] float FallWeight{ 0.0f };
    uint8_t field_188{ 0 };
    Array<HitDamageOverride> OverriddenDamage;
    Array<DamagePair> DamageList;
};

struct HitResultData
{
    Array<int32_t> field_0;
    Array<int32_t> field_10;
    Array<uint8_t> field_20;
    Array<int32_t> field_30;
};

struct SpellFunctorResult
{
    HitDesc Hit;
    AttackDesc Attack;
    uint8_t field_1D0;
};

struct HitResult
{
    HitDesc Hit;
    AttackDesc Attack;
    HitResultData Results;
    uint32_t NumConditionRolls;
};

END_SE()
