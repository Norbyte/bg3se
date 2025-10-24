#pragma once

BEGIN_NS(lua)

#define TYPECHECK(ty, luaty) \
    inline bool do_typecheck(lua_State * L, int index, Overload<ty>) \
    { \
        return lua_type(L, index) == luaty; \
    }

TYPECHECK(bool, LUA_TBOOLEAN)

inline bool do_typecheck(lua_State* L, int index, Overload<NetId>)
{
    return lua_isinteger(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<UserId>)
{
    return lua_isinteger(L, index);
}

TYPECHECK(char const*, LUA_TSTRING)
TYPECHECK(FixedString, LUA_TSTRING)
TYPECHECK(FixedStringNoRef, LUA_TSTRING)
TYPECHECK(STDString, LUA_TSTRING)
TYPECHECK(StringView, LUA_TSTRING)
TYPECHECK(LSStringView, LUA_TSTRING)
TYPECHECK(ScratchBuffer, LUA_TSTRING)
TYPECHECK(ScratchString, LUA_TSTRING)
TYPECHECK(Noesis::String, LUA_TSTRING)
TYPECHECK(Noesis::Symbol, LUA_TSTRING)
TYPECHECK(STDWString, LUA_TSTRING)
TYPECHECK(Path, LUA_TSTRING)

inline bool do_typecheck(lua_State* L, int index, Overload<Guid>)
{
    // TODO - check if string is guid-formed?
    return lua_type(L, index) == LUA_TSTRING;
}

inline bool do_typecheck(lua_State* L, int index, Overload<ComponentHandle>)
{
    return lua_isinteger(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<EntityHandle>)
{
    return lua_type(L, index) == LUA_TNIL 
        || lua_typecheck_cppvalue(L, index, MetatableTag::Entity);
}

inline bool do_typecheck(lua_State* L, int index, Overload<ecs::EntityRef>)
{
    return lua_type(L, index) == LUA_TNIL
        || lua_typecheck_cppvalue(L, index, MetatableTag::Entity);
}

inline bool do_typecheck(lua_State* L, int index, Overload<EntityHelper>)
{
    return lua_type(L, index) == LUA_TNIL
        || lua_typecheck_cppvalue(L, index, MetatableTag::Entity);
}

inline bool do_typecheck(lua_State* L, int index, Overload<ImguiHandle>)
{
    return lua_type(L, index) == LUA_TNIL
        || lua_typecheck_cppvalue(L, index, MetatableTag::ImguiObject);
}

inline bool do_typecheck(lua_State* L, int index, Overload<extui::Renderable*>)
{
    return lua_type(L, index) == LUA_TNIL
        || lua_typecheck_cppvalue(L, index, MetatableTag::ImguiObject);
}

template <class T>
inline bool do_typecheck(lua_State* L, int index, Overload<OverrideableProperty<T>>)
{
    return do_typecheck<T>(L, index);
}

template <class T>
inline typename std::enable_if_t<std::is_integral_v<T>, bool> do_typecheck(lua_State * L, int index, Overload<T>)
{
    return lua_isinteger(L, index);
}

template <class T>
inline typename std::enable_if_t<std::is_floating_point_v<T>, bool> do_typecheck(lua_State* L, int index, Overload<T>)
{
    return lua_isnumber(L, index);
}

template <class T>
inline typename std::enable_if_t<std::is_enum_v<T>, bool> do_typecheck(lua_State * L, int index, Overload<T>)
{
    if constexpr (IsIntegralAlias<T>) {
        return do_typecheck(L, index, Overload<std::underlying_type_t<T>>{});
    } else if constexpr (IsBitfieldV<T>) {
        auto propMap = BitfieldRegistry::Get().BitfieldsById[BitfieldID<T>::ID]->RegistryIndex;
        return lua_typecheck_cppvalue(L, index, MetatableTag::BitfieldValue, propMap);
    } else {
        auto propMap = EnumRegistry::Get().EnumsById[EnumID<T>::ID]->RegistryIndex;
        return lua_typecheck_cppvalue(L, index, MetatableTag::EnumValue, propMap);
    }
}

inline bool do_typecheck(lua_State* L, int index, Overload<Ref>)
{
    return true;
}

inline bool do_typecheck(lua_State* L, int index, Overload<RegistryEntry>)
{
    return true;
}

inline bool do_typecheck(lua_State* L, int index, Overload<PersistentRegistryEntry>)
{
    return true;
}

template <class TValue>
bool do_typecheck_table(lua_State* L, lua_Integer k, int index = -2)
{
    lua_rawgeti(L, index, k);
    bool ok = do_typecheck(L, -1, Overload<TValue>{});
    lua_pop(L, 1);
    return ok;
}

template <class T>
inline bool do_typecheck(lua_State* L, int index, Overload<LuaDelegate<T>>)
{
    return lua_type(L, index) == LUA_TNIL
        || lua_type(L, index) == LUA_TFUNCTION;
}

template <class TElement, unsigned TSize>
inline bool do_typecheck_array(lua_State* L, int index)
{    
    auto idx = lua_absindex(L, index);
    if (!lua_typecheck_array_n(L, idx, TSize)) return false;

    for (unsigned i = 1; i < TSize; i++) {
        if (!do_typecheck_table<TElement>(L, i, idx)) return false;
    }

    return true;
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::ivec2>)
{    
    return do_typecheck_array<int32_t, 2>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::i16vec2>)
{
    return do_typecheck_array<int32_t, 2>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::ivec4>)
{
    return do_typecheck_array<int32_t, 4>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::vec2>)
{
    return do_typecheck_array<float, 2>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::vec3>)
{
    return do_typecheck_array<float, 3>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::vec4>)
{
    return do_typecheck_array<float, 4>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::aligned_highp_vec4>)
{
    return do_typecheck_array<float, 4>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::quat>)
{
    return do_typecheck_array<float, 4>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::mat3>)
{
    return do_typecheck_array<float, 9>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::mat3x4>)
{
    return do_typecheck_array<float, 12>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::mat4x3>)
{
    return do_typecheck_array<float, 12>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<glm::mat4>)
{
    return do_typecheck_array<float, 16>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<MathParam>)
{
    return lua_type(L, index) == LUA_TNUMBER
        || do_typecheck_array<float, 3>(L, index)
        || do_typecheck_array<float, 4>(L, index)
        || do_typecheck_array<float, 9>(L, index)
        || do_typecheck_array<float, 16>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<Version>)
{
    return do_typecheck_array<int32_t, 4>(L, index);
}

inline bool do_typecheck(lua_State* L, int index, Overload<AnyRef>)
{
    return lua_type(L, index) != LUA_TNONE;
}

TYPECHECK(FunctionRef, LUA_TFUNCTION)
TYPECHECK(AnyUserdataRef, LUA_TLIGHTCPPOBJECT)

template <class T>
inline bool do_typecheck(lua_State* L, int index, Overload<std::optional<T>>)
{
    if (lua_type(L, index) == LUA_TNIL) {
        return true;
    } else {
        return do_typecheck(L, index, Overload<T>{});
    }
}

#undef TYPECHECK

END_NS()
