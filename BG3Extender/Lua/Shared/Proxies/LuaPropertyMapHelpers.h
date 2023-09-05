#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <Lua/Shared/LuaTypeValidators.h>
#include <GameDefinitions/RootTemplates.h>

BEGIN_NS(lua)

LifetimeHandle GetCurrentLifetime(lua_State* L);

template <class T>
inline void MakeObjectRef(lua_State* L, T* value, LifetimeHandle const& lifetime)
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
			MakeObjectRef(L, const_cast<std::remove_const_t<std::remove_pointer_t<T>>*>(*value), lifetime);
		} else {
			MakeObjectRef(L, *value, lifetime);
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
inline void MakeDirectObjectRef(lua_State* L, T* value, LifetimeHandle const& lifetime)
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
inline auto MakeObjectRef(lua_State* L, OverrideableProperty<T>* value, LifetimeHandle const& lifetime)
{
	return MakeObjectRef(L, &value->Value, lifetime);
}

template <class T>
inline auto MakeObjectRef(lua_State* L, T* value)
{
	return MakeObjectRef(L, value, GetCurrentLifetime(L));
}

template <class T>
PropertyOperationResult GenericGetOffsetProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, std::size_t offset, uint64_t)
{
	auto* value = (T*)((std::uintptr_t)obj + offset);
	return (LuaWrite(L, *value) == 1) ? PropertyOperationResult::Success : PropertyOperationResult::Unknown;
}

template <class T>
PropertyOperationResult GenericSetOffsetProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, int index, std::size_t offset, uint64_t)
{
	auto* value = (T*)((std::uintptr_t)obj + offset);
	lua_pushvalue(L, index);
	LuaRead(L, *value);
	lua_pop(L, 1);
	return PropertyOperationResult::Success;
}

template <class UnderlyingType>
PropertyOperationResult GenericGetOffsetBitmaskFlag(lua_State* L, LifetimeHandle const& lifetime, void* obj, std::size_t offset, uint64_t flag)
{
	auto value = *(UnderlyingType*)((std::uintptr_t)obj + offset);
	push(L, (value & (UnderlyingType)flag) == (UnderlyingType)flag);
	return PropertyOperationResult::Success;
}

template <class UnderlyingType>
PropertyOperationResult GenericSetOffsetBitmaskFlag(lua_State* L, LifetimeHandle const& lifetime, void* obj, int index, std::size_t offset, uint64_t flag)
{
	auto* value = (UnderlyingType*)((std::uintptr_t)obj + offset);
	auto set = get<bool>(L, index);
	if (set) {
		*value |= (UnderlyingType)flag;
	} else {
		*value &= (UnderlyingType)~flag;
	}

	return PropertyOperationResult::Success;
}

template <class T>
PropertyOperationResult GenericGetOffsetRefProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, std::size_t offset, uint64_t)
{
	auto* value = (T*)((std::uintptr_t)obj + offset);
	MakeObjectRef(L, value, lifetime);
	return PropertyOperationResult::Success;
}

template <class T>
bool GenericValidateOffsetProperty(void* obj, std::size_t offset, uint64_t flag)
{
	auto value = reinterpret_cast<T*>((std::uintptr_t)obj + offset);
	return ValidateAny(value);
}

void CopyRawProperties(GenericPropertyMap const& base, GenericPropertyMap& child);

template <class T, class T2>
inline void InheritProperties(LuaPropertyMap<T> const& base, LuaPropertyMap<T2>& child)
{
	static_assert(std::is_base_of_v<T, T2>, "Can only copy properties from base class");
	static_assert(static_cast<T*>(reinterpret_cast<T2*>(nullptr)) == reinterpret_cast<T*>(nullptr), "Base and child class should start at same base ptr");

	// Check to make sure that the property map we're inheriting from is already initialized
	assert(base.Initialized);
	assert(child.IsInitializing);

	CopyRawProperties(base, child);

	if (child.FallbackGetter == nullptr) {
		child.FallbackGetter = base.FallbackGetter;
	}

	if (child.FallbackSetter == nullptr) {
		child.FallbackSetter = base.FallbackSetter;
	}
}

END_NS()
