#include <GameDefinitions/Stats/Expression.h>

BEGIN_SE()

Guid StatsExpressionInternal::Hash(StringView code)
{
    Guid hash;
    MurmurHash3_x64_128(code.data(), (int)code.size(), 0, &hash);
    return hash;
}

Array<StatsExpressionInternal::Param>* StatsExpressionPooled::GetParams() const
{
    // Non-typesafe temp jank since the Lua serializer doesn't understand
    // how to serialize pointer-to-const yet
    return const_cast<Array<StatsExpressionInternal::Param>*>(&Params);
}

void StatsExpressionPooled::SetParams(lua_State* L, Array<StatsExpressionInternal::Param> params)
{
    if (RefCount > 1) {
        WARN("Changing 'Params' field of stats expression ('%s') that is refernced %d times; "
            "this can cause unintended changes in other places where the same expression is used. "
            "Consider reassigning the stats expression instead.", Code.c_str(), RefCount.load());
    }

    Params = params;
}

StringView StatsExpressionPooled::GetCode() const
{
    return Code;
}

void StatsExpressionPooled::SetCode(lua_State* L, StringView code)
{
    if (RefCount > 1 && code != Code) {
        WARN("Changing 'Code' field of stats expression ('%s') that is refernced %d times; "
            "this can cause unintended changes in other places where the same expression is used. "
            "Consider reassigning the stats expression instead.", Code.c_str(), RefCount.load());
    }

    Code = code;
}

int32_t StatsExpressionPooled::GetRefCount() const
{
    return RefCount.load();
}

END_SE()
