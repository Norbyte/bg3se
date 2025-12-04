#pragma once

BEGIN_SE()

struct StatsExpressionInternal
{
    using Variant2 = std::variant<StatsExpressionVariableData, AbilityId, SkillId, stats::StatusGroup, STDString>;
    using Param = std::variant<StatsExpressionType, Variant2, StatsExpressionVariableDataType, StatsExpressionVariableDataModifier, RollDefinition, ResourceRollDefinition, StatsContextType, int32_t, bool>;

    Array<Param> Params;
    STDString Code;

    // Lua call helpers
    //# P_FUN(Parse, StatsExpressionInternal::Parse)
    void Parse(StringView code);
};

struct StatsExpressionPooled : public StatsExpressionInternal
{
    [[bg3::hidden]] std::atomic<int32_t> RefCount;
};

class [[bg3::hidden]] StatsExpressionRef
{
public:
    inline StatsExpressionRef()
        : expression_{ nullptr }
    {}
    
    explicit inline StatsExpressionRef(StatsExpressionPooled* expr)
        : expression_{ expr }
    {
        if (expression_) {
            expression_->RefCount++;
        }
    }
    
    inline ~StatsExpressionRef()
    {
        if (expression_ && --expression_->RefCount == 0) {
            GameDelete(expression_);
        }
    }
    
    inline StatsExpressionRef(StatsExpressionRef const& expr)
        : expression_{ expr.expression_ }
    {
        if (expression_) {
            expression_->RefCount++;
        }
    }
    
    inline StatsExpressionRef& operator = (StatsExpressionRef const& expr)
    {
        expression_ = expr.expression_;
        if (expression_) {
            expression_->RefCount++;
        }
        return *this;
    }

    inline StatsExpressionPooled* operator -> () const
    {
        return expression_;
    }

    inline StatsExpressionPooled* operator * () const
    {
        return expression_;
    }

    inline StatsExpressionPooled* Get() const
    {
        return expression_;
    }
    
private:
    StatsExpressionPooled* expression_;
};

BY_VAL(StatsExpressionRef);

struct StatsExpressionWithMetadata : public StatsExpressionInternal
{
    uint8_t field_28;
    BoostSource Cause;
    TranslatedString field_48;
    uint8_t field_58;
};

struct StaticStatsExpressionManager : public ProtectedGameObject<StaticStatsExpressionManager>
{
    [[bg3::hidden]] void* VMT;
    LegacyRefMap<Guid, StatsExpressionInternal*> Expressions;
    uint8_t field_18;
};

struct DynamicStatsExpressionManager : public ProtectedGameObject<DynamicStatsExpressionManager>
{
    virtual ~DynamicStatsExpressionManager() = 0;
    virtual StatsExpressionRef CreateStatsExpression(Guid const& id, StringView const& expr) = 0;
};

END_SE()
