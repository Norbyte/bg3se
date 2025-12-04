#include <GameDefinitions/Stats/Expression.h>

BEGIN_SE()

void StatsExpressionInternal::Parse(StringView code)
{
    // Build a temp expression object to parse the expression code
    auto manager = GetStaticSymbols().eoc__DynamicStatsExpressionManager;
    Guid hash;
    MurmurHash3_x64_128(code.data(), (int)code.size(), 0, &hash);
    auto expr = (*manager)->CreateStatsExpression(hash, code);

    // Copy over the parsed tree
    Code = expr->Code;
    Params = expr->Params;
}

END_SE()
