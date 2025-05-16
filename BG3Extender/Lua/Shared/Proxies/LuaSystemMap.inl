#include <Lua/Shared/Proxies/LuaSystemMap.h>

BEGIN_NS(lua)

#define T(cls) case cls::SystemType: \
        MakeObjectRef(L, ecs->GetSystem<cls>()); \
        break;

int SystemMapMetatable::Index(lua_State* L, CppObjectMetadata& self)
{
    auto type = get<ExtSystemType>(L, 2);
    auto ecs = State::FromLua(L)->GetEntitySystemHelpers();

    switch (type) {
#include <GameDefinitions/Components/ExposedSystemTypes.inl>

    default:
        luaL_error(L, "System not supported");
        break;
    }

    return 1;
}

#undef T

int SystemMapMetatable::ToString(lua_State* L, CppObjectMetadata& self)
{
    push(L, "SystemMap");
    return 1;
}


char const* SystemMapMetatable::GetTypeName(lua_State* L, CppObjectMetadata& self)
{
    return "SystemMap";
}

END_NS()
