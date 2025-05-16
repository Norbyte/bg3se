#pragma once

BEGIN_NS(lua)

class SystemMapMetatable : public LightCppValueMetatable<SystemMapMetatable>,
    public Indexable, public Stringifiable
{
public:
    static constexpr MetatableTag MetaTag = MetatableTag::SystemMap;

    inline static void Make(lua_State* L)
    {
        lua_push_cppvalue(L, MetaTag, 0, 0);
    }

    static int Index(lua_State* L, CppObjectMetadata& self);
    static int ToString(lua_State* L, CppObjectMetadata& self);
    static char const* GetTypeName(lua_State* L, CppObjectMetadata& self);
};


END_NS()
