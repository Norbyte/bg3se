#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <GameDefinitions/RootTemplates.h>

namespace bg3se::lua
{
	extern bool EnableWriteProtectedWrites;

	template <class T>
	inline void MakeObjectRef(lua_State* L, LifetimeHandle const& lifetime, T* value)
	{
		if (value == nullptr) {
			push(L, nullptr);
			return;
		}

		if constexpr (LuaPolymorphic<T>::IsPolymorphic) {
			return LuaPolymorphic<T>::MakeRef(L, value, lifetime);
		} else if constexpr (IsArrayLike<T>::Value) {
			if constexpr (ByVal<typename IsArrayLike<T>::TElement>::Value) {
				ArrayProxy::MakeByVal<typename IsArrayLike<T>::TElement>(L, value, lifetime);
			} else {
				ArrayProxy::MakeByRef<typename IsArrayLike<T>::TElement>(L, value, lifetime);
			}
		} else if constexpr (IsMapLike<T>::Value) {
			static_assert(ByVal<typename IsMapLike<T>::TKey>::Value, "Map key is a type that we cannot serialize by-value?");

			if constexpr (ByVal<typename IsMapLike<T>::TValue>::Value) {
				MapProxy::MakeByVal(L, value, lifetime);
			} else {
				MapProxy::MakeByRef(L, value, lifetime);
			}
		} else if constexpr (IsSetLike<T>::Value) {
			static_assert(ByVal<typename IsSetLike<T>::TKey>::Value, "Set key is a type that we cannot serialize by-value?");
			SetProxy::Make(L, value, lifetime);
		} else if constexpr (std::is_pointer_v<T>) {
			if constexpr (std::is_const_v<std::remove_pointer_t<T>>) {
				MakeObjectRef(L, lifetime, const_cast<std::remove_const_t<std::remove_pointer_t<T>>*>(*value));
			} else {
				MakeObjectRef(L, lifetime, *value);
			}
		} else {
			//if constexpr (LuaLifetimeInfo<T>::HasInfiniteLifetime) {
			//	ObjectProxy::MakeRef<T>(L, value, State::FromLua(L)->GetGlobalLifetime());
			//} else {
				ObjectProxy::MakeRef<T>(L, value, lifetime);
			//}
		}
	}

	template <class T>
	inline void MakeDirectObjectRef(lua_State* L, LifetimeHandle const& lifetime, T* value)
	{
		if (value == nullptr) {
			push(L, nullptr);
		//} else if constexpr (LuaLifetimeInfo<T>::HasInfiniteLifetime) {
		//	ObjectProxy::MakeRef<T>(L, value, State::FromLua(L)->GetGlobalLifetime());
		} else {
			ObjectProxy::MakeRef<T>(L, value, lifetime);
		}
	}

	template <class T>
	inline auto MakeObjectRef(lua_State* L, LifetimeHandle const& lifetime, OverrideableProperty<T>* value)
	{
		return MakeObjectRef(L, lifetime, &value->Value);
	}

	template <class T>
	inline auto MakeObjectRef(lua_State* L, T* value)
	{
		return MakeObjectRef(L, State::FromLua(L)->GetCurrentLifetime(), value);
	}


	template <class T>
	bool GenericGetOffsetProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, std::size_t offset)
	{
		auto* value = (T*)((std::uintptr_t)obj + offset);
		return LuaWrite(L, *value) == 1;
	}

	template <class T>
	bool GenericSetOffsetProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, int index, std::size_t offset)
	{
		auto* value = (T*)((std::uintptr_t)obj + offset);
		lua_pushvalue(L, index);
		LuaRead(L, *value);
		lua_pop(L, 1);
		return true;
	}

	template <class T>
	bool GenericSetOffsetWriteProtectedProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, int index, std::size_t offset)
	{
		if (EnableWriteProtectedWrites) {
			return GenericSetOffsetProperty<T>(L, lifetime, obj, index, offset);
		} else {
			OsiError("Attempted to set a write-protected property");
			return false;
		}
	}

	inline bool SetPropertyWriteProtected(lua_State* L, LifetimeHandle const& lifetime, void* obj, int index, std::size_t offset)
	{
		return false;
	}

	template <class T>
	bool GenericGetOffsetRefProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, std::size_t offset)
	{
		auto* value = (T*)((std::uintptr_t)obj + offset);
		MakeObjectRef(L, lifetime, value);
		return true;
	}

	template <class T>
	bool GenericGetOffsetPtrProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, std::size_t offset)
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

	inline bool GenericSetOffsetRefProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, int index, std::size_t offset)
	{
		return false;
	}

	template <class T>
	bool GenericGetProperty(lua_State* L, LifetimeHandle const& lifetime, T const& value)
	{
		return LuaWrite(L, value) == 1;
	}
	
	template <class T>
	bool GenericSetProperty(lua_State* L, LifetimeHandle const& lifetime, T& value, int index)
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
