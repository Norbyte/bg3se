#pragma once

#include <cstdint>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <optional>

#include <GameDefinitions/Base/Base.h>
#include <Lua/LuaHelpers.h>

namespace bg3se::lua
{
	class Callable {};
	class Indexable {};
	class NewIndexable {};
	class Lengthable {};
	class Iterable {};
	class Stringifiable {};
	class Pushable {};
	class GarbageCollected {};

	template <class T>
	class Userdata
	{
	public:
		static T* AsUserData(lua_State* L, int index)
		{
			if (lua_type(L, index) == LUA_TUSERDATA) {
				auto obj = luaL_testudata(L, index, T::MetatableName);
				return reinterpret_cast<T*>(obj);
			} else {
				return nullptr;
			}
		}

		static T* CheckUserData(lua_State* L, int index)
		{
			return reinterpret_cast<T*>(luaL_checkudata(L, index, T::MetatableName));
		}

		template <class... Args>
		static T* New(lua_State* L, Args... args)
		{
			auto obj = reinterpret_cast<T*>(lua_newuserdata(L, sizeof(T)));
			new (obj) T(std::forward<Args>(args)...);
			luaL_setmetatable(L, T::MetatableName);
			return obj;
		}

		template <class... Args>
		static T* NewWithExtraData(lua_State* L, std::size_t extraDataSize, Args... args)
		{
			auto obj = reinterpret_cast<T*>(lua_newuserdata(L, extraDataSize + sizeof(T)));
			new (obj) T(std::forward<Args>(args)...);
			luaL_setmetatable(L, T::MetatableName);
			return obj;
		}

		static int CallProxy(lua_State* L)
		{
			if constexpr (std::is_base_of_v<Callable, T>) {
				auto self = CheckUserData(L, 1);
				return self->LuaCall(L);
			} else {
				return luaL_error(L, "Not callable!");
			}
		}

		static int IndexProxy(lua_State* L)
		{
			if constexpr (std::is_base_of_v<Indexable, T>) {
				auto self = CheckUserData(L, 1);
				return self->Index(L);
			} else {
				return luaL_error(L, "Not indexable!");
			}
		}

		static int NewIndexProxy(lua_State* L)
		{
			if constexpr (std::is_base_of_v<NewIndexable, T>) {
				auto self = CheckUserData(L, 1);
				return self->NewIndex(L);
			} else {
				return luaL_error(L, "Not newindexable!");
			}
		}

		static int LengthProxy(lua_State* L)
		{
			if constexpr (std::is_base_of_v<Lengthable, T>) {
				auto self = CheckUserData(L, 1);
				return self->Length(L);
			} else {
				return luaL_error(L, "Not lengthable!");
			}
		}

		static int PairsProxy(lua_State* L)
		{
			if constexpr (std::is_base_of_v<Iterable, T>) {
				auto self = CheckUserData(L, 1);
				return self->Pairs(L);
			} else {
				return luaL_error(L, "Not iterable!");
			}
		}

		static int ToStringProxy(lua_State* L)
		{
			if constexpr (std::is_base_of_v<Stringifiable, T>) {
				auto self = CheckUserData(L, 1);
				return self->ToString(L);
			} else {
				return luaL_error(L, "Not stringifiable!");
			}
		}

		static int GCProxy(lua_State* L)
		{
			if constexpr (std::is_base_of_v<GarbageCollected, T>) {
				auto self = CheckUserData(L, 1);
				return self->GC(L);
			} else {
				return luaL_error(L, "Not garbage collected!");
			}
		}

		// Default __pairs implementation
		int Pairs(lua_State* L)
		{
			StackCheck _(L, 3);
			lua_pushcfunction(L, &NextProxy);
			lua_pushvalue(L, 1);
			push(L, nullptr);

			return 3;
		}

		static int NextProxy(lua_State* L)
		{
			if constexpr (std::is_base_of_v<Iterable, T>) {
				auto self = CheckUserData(L, 1);
				return self->Next(L);
			} else {
				return luaL_error(L, "Not iterable!");
			}
		}

		static void PopulateMetatable(lua_State* L)
		{
			// Add custom metatable items by overriding this in subclasses
		}

		static void RegisterMetatable(lua_State* L)
		{
			lua_register(L, T::MetatableName, nullptr);
			luaL_newmetatable(L, T::MetatableName); // stack: mt

			push(L, T::MetatableName);
			lua_setfield(L, -2, "__metatable");

			if constexpr (std::is_base_of_v<Callable, T>) {
				lua_pushcfunction(L, &CallProxy); // stack: mt, &LuaCall
				lua_setfield(L, -2, "__call"); // mt.__call = &LuaCall; stack: mt
			}

			if constexpr (std::is_base_of_v<Indexable, T>) {
				lua_pushcfunction(L, &IndexProxy); // stack: mt, &Index
				lua_setfield(L, -2, "__index"); // mt.__index = &Index; stack: mt
			}

			if constexpr (std::is_base_of_v<NewIndexable, T>) {
				lua_pushcfunction(L, &NewIndexProxy); // stack: mt, &NewIndex
				lua_setfield(L, -2, "__newindex"); // mt.__index = &NewIndex; stack: mt
			}

			if constexpr (std::is_base_of_v<Lengthable, T>) {
				lua_pushcfunction(L, &LengthProxy); // stack: mt, &Length
				lua_setfield(L, -2, "__len"); // mt.__index = &Length; stack: mt
			}

			if constexpr (std::is_base_of_v<Iterable, T>) {
				lua_pushcfunction(L, &PairsProxy); // stack: mt, &Length
				lua_setfield(L, -2, "__pairs"); // mt.__index = &Length; stack: mt
			}

			if constexpr (std::is_base_of_v<Stringifiable, T>) {
				lua_pushcfunction(L, &ToStringProxy);
				lua_setfield(L, -2, "__tostring");
			}

			if constexpr (std::is_base_of_v<GarbageCollected, T>) {
				lua_pushcfunction(L, &GCProxy);
				lua_setfield(L, -2, "__gc");
			}

			T::PopulateMetatable(L);

			lua_pop(L, 1); // stack: -
		}
	};

	template <class T>
	inline std::optional<T*> safe_get_userdata(lua_State* L, int index)
	{
		if (lua_isnil(L, index)) {
			return {};
		} else {
			auto val = T::AsUserData(L, index);
			if (val) {
				return val;
			} else {
				ERR("Expected userdata of type '%s'", T::MetatableName);
				return {};
			}
		}
	}

	template <class T>
	inline typename std::enable_if_t<std::is_base_of_v<Userdata<std::remove_pointer_t<T>>, std::remove_pointer_t<T>>, T> do_get(lua_State* L, int index, Overload<T>)
	{
		return std::remove_pointer_t<T>::CheckUserData(L, index);
	}
}
