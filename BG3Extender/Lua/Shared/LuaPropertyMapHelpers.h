#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <GameDefinitions/RootTemplates.h>

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
		if constexpr (ByValArray<T>::Value || std::is_enum_v<T>) {
			ArrayProxy::MakeByVal<T>(L, value, lifetime);
		} else {
			ArrayProxy::MakeByRef<T>(L, value, lifetime);
		}
	}

	template <class T>
	void MakeObjectRef(lua_State* L, LifetimeHolder const& lifetime, ObjectSet<T>* value)
	{
		// FIXME!
		push(L, "ObjectSet PROXIES NOT SUPPORTED YET!");
	}

	template <class T>
	void MakeObjectRef(lua_State* L, LifetimeHolder const& lifetime, MultiHashSet<T>* value)
	{
		// FIXME!
		push(L, "MultiHashSet PROXIES NOT SUPPORTED YET!");
	}

	template <class T>
	void MakeObjectRef(lua_State* L, LifetimeHolder const& lifetime, VirtualMultiHashSet<T>* value)
	{
		// FIXME!
		push(L, "VirtualMultiHashSet PROXIES NOT SUPPORTED YET!");
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
	bool GenericGetOffsetProperty(lua_State* L, LifetimeHolder const& lifetime, void* obj, std::size_t offset)
	{
		auto* value = (T*)((std::uintptr_t)obj + offset);
		return LuaWrite(L, *value) == 1;
	}

	template <class T>
	bool GenericSetOffsetProperty(lua_State* L, LifetimeHolder const& lifetime, void* obj, int index, std::size_t offset)
	{
		auto* value = (T*)((std::uintptr_t)obj + offset);
		lua_pushvalue(L, index);
		LuaRead(L, *value);
		lua_pop(L, 1);
		return true;
	}

	inline bool SetPropertyWriteProtected(lua_State* L, LifetimeHolder const& lifetime, void* obj, int index, std::size_t offset)
	{
		return false;
	}

	template <class T>
	bool GenericGetOffsetRefProperty(lua_State* L, LifetimeHolder const& lifetime, void* obj, std::size_t offset)
	{
		auto* value = (T*)((std::uintptr_t)obj + offset);
		MakeObjectRef(L, lifetime, value);
		return true;
	}

	template <class T>
	bool GenericGetOffsetPtrProperty(lua_State* L, LifetimeHolder const& lifetime, void* obj, std::size_t offset)
	{
		auto* value = (T*)((std::uintptr_t)obj + offset);
		if (*value) {
			MakeObjectRef(L, lifetime, *value);
		}
		else {
			push(L, nullptr);
		}

		return true;
	}

	inline bool GenericSetOffsetRefProperty(lua_State* L, LifetimeHolder const& lifetime, void* obj, int index, std::size_t offset)
	{
		return false;
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

	void CopyRawProperties(GenericPropertyMap const& base, GenericPropertyMap& child, STDString const& baseClsName);

	template <class T, class T2>
	inline void CopyProperties(LuaPropertyMap<T> const& base, LuaPropertyMap<T2>& child, STDString const& baseClsName)
	{
		static_assert(std::is_base_of_v<T, T2>, "Can only copy properties from base class");
		CopyRawProperties(base, child, baseClsName);
	}
}
