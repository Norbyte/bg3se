
/// <lua_module>Table</lua_module>
BEGIN_NS(lua::table)

UserReturn Find(lua_State* L)
{
    return FindValue(L, 1, 2);
}

void RegisterTableLib()
{
    DECLARE_MODULE(Table, Both)
    BEGIN_MODULE()
    MODULE_FUNCTION(Find)
    END_MODULE()
}

END_NS()
