#pragma once

BEGIN_NS(lua)

inline bool do_get(lua_State * L, int index, Overload<bool>)
{
    luaL_checktype(L, index, LUA_TBOOLEAN);
    return lua_toboolean(L, index) == 1;
}

inline NetId do_get(lua_State* L, int index, Overload<NetId>)
{
    auto v = luaL_checkinteger(L, index);
    return NetId((uint64_t)v);
}

inline UserId do_get(lua_State* L, int index, Overload<UserId>)
{
    auto v = luaL_checkinteger(L, index);
    return UserId((int32_t)v);
}

inline char const* do_get(lua_State* L, int index, Overload<char const*>)
{
    return luaL_checkstring(L, index);
}

FixedString do_get(lua_State* L, int index, Overload<FixedString>);
FixedStringNoRef do_get(lua_State* L, int index, Overload<FixedStringNoRef>);

inline STDString do_get(lua_State* L, int index, Overload<STDString>)
{
    size_t len;
    auto str = luaL_checklstring(L, index, &len);
    return STDString(str, (uint32_t)len);
}

inline StringView do_get(lua_State* L, int index, Overload<StringView>)
{
    size_t len;
    auto str = luaL_checklstring(L, index, &len);
    return StringView(str, str + len);
}

inline LSStringView do_get(lua_State* L, int index, Overload<LSStringView>)
{
    luaL_error(L, "Cannot unserialize Lua strings into a StringView");
    return {};
}

inline ScratchBuffer do_get(lua_State* L, int index, Overload<ScratchBuffer>)
{
    size_t len;
    auto str = luaL_checklstring(L, index, &len);
    return ScratchBuffer(std::span<char const>(str, str + len));
}

inline ScratchString do_get(lua_State* L, int index, Overload<ScratchString>)
{
    size_t len;
    auto str = luaL_checklstring(L, index, &len);
    return ScratchString(std::span<char const>(str, str + len));
}

inline Noesis::String do_get(lua_State* L, int index, Overload<Noesis::String>)
{
    size_t len;
    auto str = luaL_checklstring(L, index, &len);
    return Noesis::String(str, (uint32_t)len);
}

inline Noesis::Symbol do_get(lua_State* L, int index, Overload<Noesis::Symbol>)
{
    auto str = luaL_checkstring(L, index);
    return Noesis::Symbol{ Noesis::SymbolManager::FindString(str) };
}

inline STDWString do_get(lua_State* L, int index, Overload<STDWString>)
{
    size_t len;
    auto str = luaL_checklstring(L, index, &len);
    return FromUTF8(StringView(str, len));
}

inline Guid do_get(lua_State* L, int index, Overload<Guid>)
{
    size_t len;
    auto str = luaL_checklstring(L, index, &len);
    auto guid = Guid::ParseGuidString(StringView(str, len));
    if (!guid) {
        luaL_error(L, "Param %d: not a valid GUID value: '%s'", index, str);
    }

    return *guid;
}

ComponentHandle do_get(lua_State* L, int index, Overload<ComponentHandle>);
EntityHandle do_get(lua_State* L, int index, Overload<EntityHandle>);
ecs::EntityRef do_get(lua_State* L, int index, Overload<ecs::EntityRef>);
TypeInformationRef do_get(lua_State* L, int index, Overload<TypeInformationRef>);
ImguiHandle do_get(lua_State* L, int index, Overload<ImguiHandle>);
extui::Renderable* do_get(lua_State* L, int index, Overload<extui::Renderable*>);

inline Path do_get(lua_State* L, int index, Overload<Path>)
{
    return Path(luaL_checkstring(L, index));
}

template <class T>
inline OverrideableProperty<T> do_get(lua_State* L, int index, Overload<OverrideableProperty<T>>)
{
    return OverrideableProperty<T>{get<T>(L, index), true};
}

template <class T>
inline typename std::enable_if_t<std::is_integral_v<T>, T> do_get(lua_State * L, int index, Overload<T>)
{
    return (T)luaL_checkinteger(L, index);
}

template <class T>
inline typename std::enable_if_t<std::is_floating_point_v<T>, T> do_get(lua_State* L, int index, Overload<T>)
{
    return (T)luaL_checknumber(L, index);
}

template <class T>
typename std::enable_if_t<std::is_enum_v<T>, T> do_get(lua_State * L, int index, Overload<T>)
{
    if constexpr (IsIntegralAlias<T>) {
        return (T)do_get(L, index, Overload<std::underlying_type_t<T>>{});
    } else if constexpr (IsBitfieldV<T>) {
        return (T)get_bitfield_value(L, index, BitfieldID<T>::ID);
    } else {
        return (T)get_enum_value(L, index, EnumID<T>::ID);
    }
}

Ref do_get(lua_State* L, int index, Overload<Ref>);
RegistryEntry do_get(lua_State* L, int index, Overload<RegistryEntry>);
PersistentRef do_get(lua_State* L, int index, Overload<PersistentRef>);
PersistentRegistryEntry do_get(lua_State* L, int index, Overload<PersistentRegistryEntry>);

template <class T>
LuaDelegate<T> do_get(lua_State* L, int index, Overload<LuaDelegate<T>>)
{
    if (lua_type(L, index) == LUA_TNIL) {
        return LuaDelegate<T>();
    }

    return LuaDelegate<T>(L, index);
}

template <class TValue>
TValue checked_getfield(lua_State* L, char const* k, int index = -1)
{
    lua_getfield(L, index, k);
    TValue val = do_get(L, -1, Overload<TValue>{});
    lua_pop(L, 1);
    return val;
}

template <class TValue>
TValue gettable(lua_State* L, lua_Integer k, int index = -2)
{
    lua_rawgeti(L, index, k);
    TValue val = do_get(L, -1, Overload<TValue>{});
    lua_pop(L, 1);
    return val;
}

template <class TValue>
TValue gettable(lua_State* L, char const* k, int index = -2)
{
    push(L, k);
    lua_rawget(L, index);
    TValue val = do_get(L, -1, Overload<TValue>{});
    lua_pop(L, 1);
    return val;
}

inline glm::ivec2 do_get(lua_State* L, int index, Overload<glm::ivec2>)
{    
    auto i = lua_absindex(L, index);
    glm::ivec2 val;    
    luaL_checktype(L, index, LUA_TTABLE);
    val.x = gettable<int32_t>(L, 1, i);
    val.y = gettable<int32_t>(L, 2, i);
    return val;    
}

inline glm::ivec4 do_get(lua_State* L, int index, Overload<glm::ivec4>)
{    
    auto i = lua_absindex(L, index);
    glm::ivec4 val;    
    luaL_checktype(L, index, LUA_TTABLE);
    val.x = gettable<int32_t>(L, 1, i);
    val.y = gettable<int32_t>(L, 2, i);
    val.z = gettable<int32_t>(L, 3, i);
    val.w = gettable<int32_t>(L, 4, i);
    return val;    
}

inline glm::i16vec2 do_get(lua_State* L, int index, Overload<glm::i16vec2>)
{    
    auto i = lua_absindex(L, index);
    glm::i16vec2 val;
    luaL_checktype(L, index, LUA_TTABLE);
    val.x = gettable<int16_t>(L, 1, i);
    val.y = gettable<int16_t>(L, 2, i);
    return val;    
}

glm::vec2 do_get(lua_State* L, int index, Overload<glm::vec2>);
glm::vec3 do_get(lua_State* L, int index, Overload<glm::vec3>);
glm::vec4 do_get(lua_State* L, int index, Overload<glm::vec4>);
glm::aligned_highp_vec4 do_get(lua_State* L, int index, Overload<glm::aligned_highp_vec4>);
glm::quat do_get(lua_State* L, int index, Overload<glm::quat>);
glm::mat3 do_get(lua_State* L, int index, Overload<glm::mat3>);
glm::mat3x4 do_get(lua_State* L, int index, Overload<glm::mat3x4>);
glm::mat4x3 do_get(lua_State* L, int index, Overload<glm::mat4x3>);
glm::mat4 do_get(lua_State* L, int index, Overload<glm::mat4>);
MathParam do_get(lua_State* L, int index, Overload<MathParam>);
EntityHelper do_get(lua_State* L, int index, Overload<EntityHelper>);
stats::ConditionId do_get(lua_State* L, int index, Overload<stats::ConditionId>);
// Jank, but we can't put it anywhere else for now :(
LuaSoundObjectId do_get(lua_State* L, int index, Overload<LuaSoundObjectId>);

inline Version do_get(lua_State* L, int index, Overload<Version>)
{
    auto i = lua_absindex(L, index);
    luaL_checktype(L, i, LUA_TTABLE);
    uint32_t major = gettable<uint32_t>(L, 1, i), 
        minor = gettable<uint32_t>(L, 2, i),
        revision = gettable<uint32_t>(L, 3, i),
        build = gettable<uint32_t>(L, 4, i);
    return Version(major, minor, revision, build);
}

inline AnyRef do_get(lua_State* L, int index, Overload<AnyRef>)
{
    auto i = lua_absindex(L, index);
    auto type = lua_type(L, i);
    if (type == LUA_TNONE) {
        luaL_error(L, "any value expected, got %s", GetDebugName(L, i));
    }

    return AnyRef(i);
}

inline FunctionRef do_get(lua_State* L, int index, Overload<FunctionRef>)
{
    auto i = lua_absindex(L, index);
    luaL_checktype(L, i, LUA_TFUNCTION);
    return FunctionRef(i);
}

inline AnyUserdataRef do_get(lua_State* L, int index, Overload<AnyUserdataRef>)
{
    auto i = lua_absindex(L, index);
    auto type = lua_type(L, i);
    if (type != LUA_TLIGHTCPPOBJECT && type != LUA_TCPPOBJECT) {
        luaL_error(L, "lightcppobject expected, got %s", GetDebugName(L, i));
    }

    return AnyUserdataRef(i);
}

template <class T>
inline typename std::optional<T> do_get(lua_State* L, int index, Overload<std::optional<T>>)
{
    if (lua_type(L, index) == LUA_TNIL) {
        return {};
    } else {
        return do_get(L, index, Overload<T>{});
    }
}

template <class T>
typename std::enable_if_t<!IsByVal<T> && !std::is_pointer_v<T>, T> do_get(lua_State* L, int index, Overload<T>);

template <class T>
typename std::enable_if_t<!IsByVal<T> && std::is_pointer_v<T>, T> do_get(lua_State* L, int index, Overload<T>);

// Overload helper for fetching a parameter for a Lua -> C++ function call
template <class T>
inline T get_param(lua_State* L, int i, Overload<T>)
{
    return do_get(L, i, Overload<T>{});
}

template <class T>
inline std::optional<T> get_param(lua_State* L, int i, Overload<std::optional<T>>)
{
    if (lua_gettop(L) < i || lua_isnil(L, i)) {
        return {};
    } else {
        return do_get(L, i, Overload<T>{});
    }
}

template <class T>
inline T get(lua_State* L, int index)
{
    return do_get(L, index, Overload<T>{});
}

template <class T>
inline T get(lua_State* L, AnyRef ref)
{
    return do_get(L, ref.Index, Overload<T>{});
}

template <class T>
T try_gettable(lua_State* L, char const* k, int index, T defaultValue)
{
    push(L, k);
    lua_rawget(L, index);
    auto val = get<std::optional<T>>(L, -1);
    lua_pop(L, 1);
    return val ? *val : defaultValue;
}

template <class T>
std::optional<T> try_gettable(lua_State* L, char const* k, int index)
{
    push(L, k);
    lua_rawget(L, index);
    auto val = get<std::optional<T>>(L, -1);
    lua_pop(L, 1);
    return val;
}

END_NS()
