#pragma once

BEGIN_SE()

struct StatsExpressionParam
{
    using Variant2 = std::variant<StatsExpressionVariableData, AbilityId, SkillId, stats::StatusGroup, STDString>;
    using Param = std::variant<StatsExpressionType, Variant2, StatsExpressionVariableDataType, StatsExpressionVariableDataModifier, RollDefinition, ResourceRollDefinition, StatsContextType, int32_t, bool>;

    Array<Param> Params;
    STDString Code;
};

struct StatsExpressionParamEx : public StatsExpressionParam
{
    [[bg3::hidden]] int FastLock;
};

struct StatsExpressionWithMetadata : public StatsExpressionParam
{
    uint8_t field_28;
    BoostSource Cause;
    TranslatedString field_48;
    uint8_t field_58;
};

struct LuaExpressionManager : public ProtectedGameObject<LuaExpressionManager>
{
    [[bg3::hidden]] void* VMT;
    LegacyRefMap<Guid, StatsExpressionParamEx*> Expressions;
    uint8_t field_18;
};

END_SE()
