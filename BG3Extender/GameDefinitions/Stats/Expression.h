#pragma once

BEGIN_SE()

struct StatsExpressionInternal
{
    using Variant2 = std::variant<StatsExpressionVariableData, AbilityId, SkillId, stats::StatusGroup, STDString>;
    using Param = std::variant<StatsExpressionType, Variant2, StatsExpressionVariableDataType, StatsExpressionVariableDataModifier, RollDefinition, ResourceRollDefinition, StatsContextType, int32_t, bool>;

    Array<Param> Params;
    STDString Code;

    static Guid Hash(StringView code);
};

struct StatsExpressionPooled : public StatsExpressionInternal
{
    [[bg3::hidden]] std::atomic<int32_t> RefCount;

    // Lua property helpers
    //# P_GETTER_SETTER(Params, GetParams, SetParams)
    //# P_GETTER_SETTER(Code, GetCode, SetCode)
    //# P_GETTER(RefCount, GetRefCount)
    Array<Param>* GetParams() const;
    void SetParams(lua_State* L, Array<Param> params);

    StringView GetCode() const;
    void SetCode(lua_State* L, StringView code);

    int32_t GetRefCount() const;
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

MARK_BY_VALUE_TYPE(StatsExpressionRef);

struct StatsExpressionWithMetadata : public StatsExpressionInternal
{
    BoostType BoostType;
    BoostSource Cause;
    TranslatedString BoostSourceName;
    bool CriticalHit;
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
