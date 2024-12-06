#pragma once

BEGIN_NS(lua)

class UserVariableHolderMetatable : public LightCppValueMetatable<UserVariableHolderMetatable>,
    public Indexable, public NewIndexable, public Lengthable, public Iterable, public Stringifiable
{
public:
    static constexpr MetatableTag MetaTag = MetatableTag::UserVariableHolder;

    inline static void Make(lua_State* L, EntityHandle const& component)
    {
        lua_push_cppvalue(L, MetaTag, 0, component.Handle);
    }

    static int Index(lua_State* L, CppObjectMetadata& self);
    static int NewIndex(lua_State* L, CppObjectMetadata& self);
    static int Length(lua_State* L, CppObjectMetadata& self);
    static int Next(lua_State* L, CppObjectMetadata& self);
    static int ToString(lua_State* L, CppObjectMetadata& self);
    static char const* GetTypeName(lua_State* L, CppObjectMetadata& self);
};

class ModVariableHolderMetatable : public LightCppValueMetatable<ModVariableHolderMetatable>,
    public Indexable, public NewIndexable, public Lengthable, public Iterable, public Stringifiable
{
public:
    static constexpr MetatableTag MetaTag = MetatableTag::ModVariableHolder;

    inline static void Make(lua_State* L, uint32_t modIndex)
    {
        lua_push_cppvalue(L, MetaTag, 0, modIndex);
    }

    static int Index(lua_State* L, CppObjectMetadata& self);
    static int NewIndex(lua_State* L, CppObjectMetadata& self);
    static int Length(lua_State* L, CppObjectMetadata& self);
    static int Next(lua_State* L, CppObjectMetadata& self);
    static int ToString(lua_State* L, CppObjectMetadata& self);
    static char const* GetTypeName(lua_State* L, CppObjectMetadata& self);
};

END_NS()
