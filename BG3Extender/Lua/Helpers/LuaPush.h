#pragma once

BEGIN_NS(lua)

inline void push(lua_State* L, nullptr_t v)
{
	lua_pushnil(L);
}

// Prevent implicit typecast to bool from pointer types
template <class T>
inline typename std::enable_if_t<std::is_same_v<T, bool>, void> push(lua_State* L, bool v)
{
	lua_pushboolean(L, v ? 1 : 0);
}

inline void push(lua_State* L, int16_t v)
{
	lua_pushinteger(L, (lua_Integer)v);
}

inline void push(lua_State* L, int32_t v)
{
	lua_pushinteger(L, (lua_Integer)v);
}

inline void push(lua_State* L, uint32_t v)
{
	lua_pushinteger(L, (lua_Integer)v);
}

inline void push(lua_State* L, int64_t v)
{
	lua_pushinteger(L, v);
}

inline void push(lua_State* L, uint64_t v)
{
	lua_pushinteger(L, (lua_Integer)v);
}

inline void push(lua_State* L, double v)
{
	lua_pushnumber(L, v);
}

inline void push(lua_State* L, char const* v)
{
	if (v) {
		lua_pushstring(L, v);
	}
	else {
		lua_pushnil(L);
	}
}

void push(lua_State* L, FixedString const& v);

inline void push(lua_State* L, std::string const& s)
{
	lua_pushlstring(L, s.data(), s.size());
}

inline void push(lua_State* L, STDString const& s)
{
	lua_pushlstring(L, s.data(), s.size());
}

inline void push(lua_State* L, STDWString const& s)
{
	push(L, ToUTF8(s));
}

inline void push(lua_State* L, Guid const& s)
{
	push(L, s.ToString());
}

inline void push(lua_State* L, StringView const& v)
{
	lua_pushlstring(L, v.data(), v.size());
}

inline void push(lua_State* L, WStringView const& v)
{
	push(L, ToUTF8(v));
}

void push(lua_State* L, EntityHandle const& h);
void push(lua_State* L, ComponentHandle const& h);
void push(lua_State* L, ecs::EntityRef const& h);
void push(lua_State* L, TypeInformationRef const& h);
#if defined(ENABLE_IMGUI)
void push(lua_State* L, ImguiHandle const& h);
void push(lua_State* L, extui::Renderable* o);
#endif

inline void push(lua_State* L, lua_CFunction v)
{
	lua_pushcfunction(L, v);
}

inline void push(lua_State* L, Path const& p)
{
	push(L, p.Name);
}

inline void push(lua_State* L, NetId const& v)
{
	push(L, v.Id);
}

inline void push(lua_State* L, UserId const& v)
{
	push(L, v.Id);
}

void push(lua_State* L, glm::ivec2 const& v);
void push(lua_State* L, glm::ivec4 const& v);
void push(lua_State* L, glm::vec2 const& v);
void push(lua_State* L, glm::vec3 const& v);
void push(lua_State* L, glm::vec4 const& v);
void push(lua_State* L, glm::quat const& v);
void push(lua_State* L, glm::mat3 const& m);
void push(lua_State* L, glm::mat3x4 const& m);
void push(lua_State* L, glm::mat4x3 const& m);
void push(lua_State* L, glm::mat4 const& m);
void push(lua_State* L, Ref const& v);
void push(lua_State* L, PersistentRef const& v);
void push(lua_State* L, RegistryEntry const& v);
void push(lua_State* L, PersistentRegistryEntry const& v);
void push(lua_State* L, RawComponentRef const& v);

template <class T>
void push(lua_State* L, LuaDelegate<T> const& v)
{
	if (v) {
		v.Push();
	} else {
		lua_pushnil(L);
	}
}

void assign(lua_State* L, int idx, glm::vec2 const& v);
void assign(lua_State* L, int idx, glm::vec3 const& v);
void assign(lua_State* L, int idx, glm::vec4 const& v);
void assign(lua_State* L, int idx, glm::quat const& v);
void assign(lua_State* L, int idx, glm::mat3 const& m);
void assign(lua_State* L, int idx, glm::mat3x4 const& m);
void assign(lua_State* L, int idx, glm::mat4x3 const& m);
void assign(lua_State* L, int idx, glm::mat4 const& m);

#if defined(ENABLE_UI)
inline void push(lua_State* L, Noesis::String const& s)
{
	lua_pushlstring(L, s.Str(), s.Size());
}

inline void push(lua_State* L, Noesis::Symbol const& s)
{
	push(L, s.Str());
}

inline void push(lua_State* L, Noesis::Color const& v)
{
	push(L, glm::vec4(v.r, v.g, v.b, v.a));
}

inline void push(lua_State* L, Noesis::Rect const& v)
{
	push(L, glm::vec4(v.x, v.y, v.width, v.height));
}

inline void push(lua_State* L, Noesis::Vector2 const& v)
{
	push(L, glm::vec2(v.x, v.y));
}

inline void push(lua_State* L, Noesis::Vector3 const& v)
{
	push(L, glm::vec3(v.x, v.y, v.z));
}
#endif

template <class T>
inline typename std::enable_if_t<std::is_enum_v<T>, void> push(lua_State* L, T v)
{
	if constexpr (IsBitfieldV<T>) {
		push_bitfield_value(L, static_cast<EnumUnderlyingType>(v), BitfieldID<T>::ID);
	} else {
		push_enum_value(L, static_cast<EnumUnderlyingType>(v), EnumID<T>::ID);
	}
}

template <class T>
inline void push(lua_State* L, std::optional<T> const& v)
{
	if (v) {
		push(L, *v);
	} else {
		lua_pushnil(L);
	}
}

template <class T>
inline void push(lua_State* L, OverrideableProperty<T> const& v)
{
	push(L, v.Value);
}

template <class TValue>
inline void setfield(lua_State* L, char const* k, TValue const& v, int index = -2)
{
	push(L, v);
	lua_setfield(L, index, k);
}

template <class TKey, class TValue>
inline void settable(lua_State* L, TKey const& k, TValue const& v, int index = -3)
{
	push(L, k);
	push(L, v);
	lua_settable(L, index);
}

inline void push(lua_State* L, glm::ivec2 const& v)
{
	lua_createtable(L, 2, 0);
	settable(L, 1, v.x);
	settable(L, 2, v.y);
}

inline void push(lua_State* L, glm::ivec4 const& v)
{
	lua_createtable(L, 4, 0);
	settable(L, 1, v.x);
	settable(L, 2, v.y);
	settable(L, 3, v.z);
	settable(L, 4, v.w);
}

inline void push(lua_State* L, glm::i16vec2 const& v)
{
	lua_createtable(L, 2, 0);
	settable(L, 1, v.x);
	settable(L, 2, v.y);
}

inline void push(lua_State* L, Version const& v)
{
	lua_createtable(L, 4, 0);
	settable(L, 1, v.Major());
	settable(L, 2, v.Minor());
	settable(L, 3, v.Revision());
	settable(L, 4, v.Build());
}

END_NS()
