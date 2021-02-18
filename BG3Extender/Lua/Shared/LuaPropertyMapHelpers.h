#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>

namespace bg3se::lua
{
	template <class T>
	bool GenericGetProperty(lua_State* L, T const& value)
	{
		return LuaWrite(L, value) == 1;
	}
	
	template <class T>
	bool GenericSetProperty(lua_State* L, T& value, int index)
	{
		lua_pushvalue(L, index);
		LuaRead(L, value);
		lua_pop(L, 1);
		return true;
	}

	template <class T, class T2>
	void CopyProperties(LuaPropertyMap<T> const& base, LuaPropertyMap<T2>& child)
	{
		static_assert(std::is_base_of_v<T, T2>, "Can only copy properties from base class");
		for (auto const& prop : base.Properties) {
			auto getter = prop.second.Get;
			auto setter = prop.second.Set;
			child.AddProperty(
				prop.first,
				reinterpret_cast<LuaPropertyMap<T2>::PropertyAccessors::Getter*>(prop.second.Get),
				reinterpret_cast<LuaPropertyMap<T2>::PropertyAccessors::Setter*>(prop.second.Set)
			);
		}
	}

#define DECLARE_CLS(name) \
	char const* const ObjectProxy2<name>::MetatableName = #name; \
	LuaPropertyMap<name> StaticLuaPropertyMap<name>::PropertyMap

#define REGISTER_CLS(name) ObjectProxy2<name>::RegisterMetatable(L)

#define CLS(cls) \
	using PM = StaticLuaPropertyMap<cls>; \
	auto& pm = StaticLuaPropertyMap<cls>::PropertyMap

#define INHERIT(base) { \
		auto& basePm = StaticLuaPropertyMap<base>::PropertyMap; \
		CopyProperties(basePm, pm); \
	}

#define P(prop) \
	pm.AddProperty(#prop, \
		[](lua_State* L, PM::ObjectType* obj) { \
			return GenericGetProperty(L, obj->prop); \
		}, \
		[](lua_State* L, PM::ObjectType* obj, int index) { \
			return GenericSetProperty(L, obj->prop, index); \
		} \
	)

#define P_RO(prop) \
	pm.AddProperty(#prop, \
		[](lua_State* L, PM::ObjectType* obj) { \
			return GenericGetProperty(L, obj->prop); \
		}, \
		[](lua_State* L, PM::ObjectType* obj, int index) { \
			return false; \
		} \
	)

#define PN(name, prop) \
	pm.AddProperty(#name, \
		[](lua_State* L, PM::ObjectType* obj) { \
			return GenericGetProperty(L, obj->prop); \
		}, \
		[](lua_State* L, PM::ObjectType* obj, int index) { \
			return GenericSetProperty(L, obj->prop, index); \
		} \
	)
}
