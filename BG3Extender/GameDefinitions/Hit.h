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
    uint8_t RollValue{ 0 };
    bool KeepNew{ false };
};

struct RerollValue
{
    uint8_t RollValue{ 0 };
    uint8_t RerollType{ 0 };
};

struct Roll
{
    RollDefinition Roll;
    stats::RollType RollType{ stats::RollType::None };
    bool Advantage{ false };
    bool Disadvantage{ false };
    Array<RerollCondition> RerollConditions;
};

struct StatsRollResult
{
    int Total{ 0 };
    int NaturalRoll{ 0 };
    int DiscardedDiceTotal{ 0 };
    RollCritical Critical{ RollCritical::None };
    std::optional<uint32_t> CriticalThreshold;
    Array<RerollValue> RollsCount;
};

struct ResolvedRollBonus
{
    DiceSizeId DiceSize{ DiceSizeId::Default };
    uint8_t NumDice{ 0 };
    [[bg3::legacy(ResolvedRollBonus)]] int Bonus{ 0 };
    [[bg3::legacy(Description)]] TranslatedString SourceName;
};

struct FixedRollBonus
{
    [[bg3::legacy(field_0)]] int RollBonus{ 0 };
    [[bg3::legacy(Description)]] TranslatedString SourceName;
};

struct StatsRollMetadata
{
    int ProficiencyBonus{ 0 };
    int RollBonus{ 0 };
    int HighGroundBonus{ 0 };
    int LowGroundPenalty{ 0 };
    [[bg3::legacy(field_10)]] int BaseUnarmedDamage{ 0 };
    HashMap<AbilityId, int32_t> AbilityBoosts;
    HashMap<SkillId, int32_t> SkillBonuses;
    bool AutoSkillCheckFail{ false };
    bool AutoAbilityCheckFail{ false };
    bool AutoAbilitySavingThrowFail{ false };
    bool HasCustomMetadata{ false };
    bool IsCritical{ false };
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
    [[bg3::hidden]] StatsExpressionParamEx* CachedStatExpression{ nullptr };
    STDString StatExpression;
    Array<int32_t> IntParams;
    Array<StatsRoll> RollParams;
    Array<DamageType> DamageTypeParams;
    int IntIndex{ 0 };
    int RollIndex{ 0 };
    int DamageTypeIndex{ 0 };
};

struct ConditionRoll
{
    uint8_t DataType{ 0 };
    //# P_BITMASK(RollType)
    ConditionRollType RollType{ ConditionRollType::AbilityCheckRoll };
    std::variant<StatsRoll, StatsExpressionResolved> Roll;
    int Difficulty{ 0 };
    Guid RollUuid;
    bool SwappedSourceAndTarget{ false };
    AbilityId Ability{ AbilityId::None };
    SkillId Skill{ SkillId::Sentinel };
};


struct ConditionRolls
{
    Array<ConditionRoll> Rolls;
};

struct DamageModifierMetadata
{
    uint8_t MetadataType{ 0 };
    int Value{ 0 };
    DamageType DamageType{ DamageType::None };
    uint8_t SourceType{ 0 };
    [[bg3::legacy(Argument)]] std::variant<int32_t, RollDefinition, StatsExpressionResolved> Source;
    [[bg3::legacy(Description)]] TranslatedString SourceName;
    [[bg3::legacy(Description2)]] FixedString SourceId;
};

struct DamageResistance
{
    [[bg3::legacy(Flags)]] uint8_t Type{ 0 };
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
    DamageType DamageType{ DamageType::None };
    int OriginalValue{ 0 };
    int OverriddenValue{ 0 };
};

struct HitDesc
{
    int TotalDamageDone{ 0 };
    stats::DeathType DeathType{ stats::DeathType::None };
    DamageType DamageType{ DamageType::None };
    CauseType CauseType{ CauseType::None };
    glm::vec3 ImpactPosition{ .0f };
    glm::vec3 ImpactDirection{ .0f };
    float ImpactForce{ 0.0f };
    int ArmorAbsorption{ 0 };
    int LifeSteal{ 0 };
    DamageFlags EffectFlags{ 0 };
    EntityHandle Inflicter;
    EntityHandle InflicterOwner;
    EntityHandle Throwing;
    int StoryActionId{ 0 };
    HitWith HitWith{ HitWith::None };
    AbilityId AttackRollAbility{ AbilityId::None };
    AbilityId SaveAbility{ AbilityId::None };
    [[bg3::legacy(field_4F)]] SpellAttackType SpellAttackType{ 0 };
    Array<ConditionRoll> ConditionRolls;
    [[bg3::legacy(Results)]] StatsDamage Damage;
    Guid SpellCastGuid;
    FixedString SpellId;
    [[bg3::legacy(field_150)]] FixedString StatusId;
    [[bg3::legacy(field_158)]] EntityHandle StatusEntity;
    [[bg3::legacy(field_160)]] SurfaceType SurfaceType{ 0 };
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
    uint8_t field_1D0{ 0 };
};

struct HitResult
{
    HitDesc Hit;
    AttackDesc Attack;
    HitResultData Results;
    uint32_t NumConditionRolls{ 0 };
};

END_SE()
