#pragma once

#include <Lua/Shared/Proxies/LuaPropertyMap.h>
#include <Lua/Shared/Proxies/LuaCppClass.h>

BEGIN_NS(lua)

template <class T>
class LightCppValueMetatable
{
public:
	static std::optional<CppValueMetadata> TryGet(lua_State* L, int index)
	{
		CppValueMetadata meta;
		if (lua_try_get_cppvalue(L, index, T::MetaTag, meta)) {
			return meta;
		} else {
			return {};
		}
	}

	static int CallProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Callable, T>) {
			CppValueMetadata self;
			lua_get_cppvalue(L, 1, T::MetaTag, self);
			return T::Call(L, self);
		} else {
			return luaL_error(L, "Not callable!");
		}
	}

	static int IndexProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Indexable, T>) {
			StackCheck _(L, 1);
			CppValueMetadata self;
			lua_get_cppvalue(L, 1, T::MetaTag, self);
			return T::Index(L, self);
		} else {
			return luaL_error(L, "Not indexable!");
		}
	}

	static int NewIndexProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<NewIndexable, T>) {
			StackCheck _(L, 0);
			CppValueMetadata self;
			lua_get_cppvalue(L, 1, T::MetaTag, self);
			return T::NewIndex(L, self);
		} else {
			return luaL_error(L, "Not newindexable!");
		}
	}

	static int LengthProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Lengthable, T>) {
			StackCheck _(L, 1);
			CppValueMetadata self;
			lua_get_cppvalue(L, 1, T::MetaTag, self);
			return T::Length(L, self);
		} else {
			return luaL_error(L, "Not lengthable!");
		}
	}

	static int PairsProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Iterable, T>) {
			CppValueMetadata self;
			lua_get_cppvalue(L, 1, T::MetaTag, self);
			return T::Pairs(L, self);
		} else {
			return luaL_error(L, "Not iterable!");
		}
	}

	static int ToStringProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Stringifiable, T>) {
			StackCheck _(L, 1);
			CppValueMetadata self;
			lua_get_cppvalue(L, 1, T::MetaTag, self);
			return T::ToString(L, self);
		} else {
			return luaL_error(L, "Not stringifiable!");
		}
	}

	static int EqualProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<EqualityComparable, T>) {
			StackCheck _(L, 1);

			// __eq is called for both (TValue == x) and (x == TValue) scenarios, i.e.
			// the CppValue can be either in the 1st, 2nd or both arguments.
			// We'll always try to cast the first argument first and if it fails check the second.
			CppValueMetadata self;
			if (lua_try_get_cppvalue(L, 1, T::MetaTag, self)) {
				push(L, T::IsEqual(L, self, 2));
			} else {
				lua_get_cppvalue(L, 2, T::MetaTag, self);
				push(L, T::IsEqual(L, self, 1));
			}

			return 1;
		} else {
			return luaL_error(L, "Not comparable!");
		}
	}

	static int BAndProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<EqualityComparable, T>) {
			StackCheck _(L, 1);
			CppValueMetadata self;
			if (lua_try_get_cppvalue(L, 1, T::MetaTag, self)) {
				return T::BAnd(L, self, 2);
			} else {
				lua_get_cppvalue(L, 2, T::MetaTag, self);
				return T::BAnd(L, self, 1);
			}
		} else {
			return luaL_error(L, "Binary ops not supported!");
		}
	}

	static int BOrProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<EqualityComparable, T>) {
			StackCheck _(L, 1);
			CppValueMetadata self;
			if (lua_try_get_cppvalue(L, 1, T::MetaTag, self)) {
				return T::BOr(L, self, 2);
			} else {
				lua_get_cppvalue(L, 2, T::MetaTag, self);
				return T::BOr(L, self, 1);
			}
		} else {
			return luaL_error(L, "Binary ops not supported!");
		}
	}

	static int BXorProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<EqualityComparable, T>) {
			StackCheck _(L, 1);
			CppValueMetadata self;
			if (lua_try_get_cppvalue(L, 1, T::MetaTag, self)) {
				return T::BXor(L, self, 2);
			} else {
				lua_get_cppvalue(L, 2, T::MetaTag, self);
				return T::BXor(L, self, 1);
			}
		} else {
			return luaL_error(L, "Binary ops not supported!");
		}
	}

	static int BNotProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<EqualityComparable, T>) {
			StackCheck _(L, 1);
			CppValueMetadata self;
			lua_get_cppvalue(L, 2, T::MetaTag, self);
			return T::BNot(L, self);
		} else {
			return luaL_error(L, "Binary ops not supported!");
		}
	}

	// Default __pairs implementation
	static int Pairs(lua_State* L, CppValueMetadata const& self)
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
			CppValueMetadata self;
			lua_get_cppvalue(L, 1, T::MetaTag, self);
			return T::Next(L, self);
		} else {
			return luaL_error(L, "Not iterable!");
		}
	}

	static int NameProxy(lua_State* L)
	{
		StackCheck _(L, 1);
		CppValueMetadata self;
		lua_get_cppvalue(L, 1, T::MetaTag, self);
		push(L, T::GetTypeName(L, self));
		return 1;
	}

	static void PopulateMetatable(lua_State* L, CMetatable* mt)
	{
		// Add custom metatable items by overriding this in subclasses
	}

	static void RegisterMetatable(lua_State* L)
	{
		auto mt = lua_alloc_cmetatable(L);

		if constexpr (std::is_base_of_v<Callable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Call, &CallProxy);
		}

		if constexpr (std::is_base_of_v<Indexable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Index, &IndexProxy);
		}

		if constexpr (std::is_base_of_v<NewIndexable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::NewIndex, &NewIndexProxy);
		}

		if constexpr (std::is_base_of_v<Lengthable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Len, &LengthProxy);
		}

		if constexpr (std::is_base_of_v<Iterable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Pairs, &PairsProxy);
		}

		if constexpr (std::is_base_of_v<Stringifiable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::ToString, &ToStringProxy);
		}

		// Light cppobjects are not garbage collected (they're passed by value)

		if constexpr (std::is_base_of_v<EqualityComparable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Eq, &EqualProxy);
		}

		if constexpr (std::is_base_of_v<HasBinaryOps, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::BAnd, &BAndProxy);
			lua_cmetatable_set(L, mt, (int)MetamethodName::BOr, &BOrProxy);
			lua_cmetatable_set(L, mt, (int)MetamethodName::BXor, &BXorProxy);
			lua_cmetatable_set(L, mt, (int)MetamethodName::BNot, &BNotProxy);
		}

		lua_cmetatable_set(L, mt, (int)MetamethodName::Name, &NameProxy);

		T::PopulateMetatable(L, mt);
		CppMetatableManager::FromLua(L).RegisterMetatable(T::MetaTag, mt);
	}
};

END_NS()
