#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>

namespace bg3se::lua
{
	template <class T>
	void MakeObjectRef(lua_State* L, LifetimeHolder const& lifetime, T* value)
	{
		ObjectProxy::MakeRef<T>(L, value, lifetime);
	}

	template <class T>
	void MakeObjectRef(lua_State* L, LifetimeHolder const& lifetime, OverrideableProperty<T>* value)
	{
		MakeObjectRef(L, lifetime, &value->Value);
	}

	template <class T>
	void MakeObjectRef(lua_State* L, LifetimeHolder const& lifetime, Array<T>* value)
	{
		// FIXME!
		push(L, "Array PROXIES NOT SUPPORTED YET!");
	}

	template <class T>
	void MakeObjectRef(lua_State* L, LifetimeHolder const& lifetime, ObjectSet<T>* value)
	{
		// FIXME!
		push(L, "ObjectSet PROXIES NOT SUPPORTED YET!");
	}

	template <class TKey, class TValue>
	void MakeObjectRef(lua_State* L, LifetimeHolder const& lifetime, Map<TKey, TValue>* value)
	{
		// FIXME!
		push(L, "Map PROXIES NOT SUPPORTED YET!");
	}

	template <class TKey, class TValue>
	void MakeObjectRef(lua_State* L, LifetimeHolder const& lifetime, RefMap<TKey, TValue>* value)
	{
		// FIXME!
		push(L, "RefMap PROXIES NOT SUPPORTED YET!");
	}

	template <class TKey, class TValue>
	void MakeObjectRef(lua_State* L, LifetimeHolder const& lifetime, MultiHashMap<TKey, TValue>* value)
	{
		// FIXME!
		push(L, "MultiHashMap PROXIES NOT SUPPORTED YET!");
	}

	template <class TKey, class TValue>
	void MakeObjectRef(lua_State* L, LifetimeHolder const& lifetime, VirtualMultiHashMap<TKey, TValue>* value)
	{
		// FIXME!
		push(L, "VirtualMultiHashMap PROXIES NOT SUPPORTED YET!");
	}

	template <class T>
	bool GenericGetProperty(lua_State* L, LifetimeHolder const& lifetime, T const& value)
	{
		return LuaWrite(L, value) == 1;
	}
	
	template <class T>
	bool GenericSetProperty(lua_State* L, LifetimeHolder const& lifetime, T& value, int index)
	{
		lua_pushvalue(L, index);
		LuaRead(L, value);
		lua_pop(L, 1);
		return true;
	}

	template <class T, class T2>
	void CopyProperties(LuaPropertyMap<T> const& base, LuaPropertyMap<T2>& child, STDString const& baseClsName)
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

		for (auto const& parent : base.Parents) {
			child.Parents.push_back(parent);
		}

		child.Parents.push_back(baseClsName);
	}
}
