BEGIN_NS(lua::json)

struct StringifyContext
{
    bool StringifyInternalTypes{ false };
    bool IterateUserdata{ false };
    bool Beautify{ true };
    bool AvoidRecursion{ false };
    bool Binary{ false };
    uint32_t MaxDepth{ 64 };
    int32_t LimitDepth{ -1 };
    int32_t LimitArrayElements{ -1 };
    std::unordered_set<void*> SeenUserdata;
};

STDString Stringify(lua_State * L, StringifyContext& ctx, int index);
bool Parse(lua_State* L, StringView json, bool binary = false);

END_NS()
