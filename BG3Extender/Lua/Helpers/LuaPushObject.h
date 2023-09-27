#pragma once

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
		ArrayProxy::Make<typename IsArrayLike<T>::TElement>(L, value, lifetime);
	} else if constexpr (IsMapLike<T>::Value) {
		static_assert(IsByVal<typename IsMapLike<T>::TKey>, "Map key is a type that we cannot serialize by-value?");
		MapProxy::Make(L, value, lifetime);
	} else if constexpr (IsSetLike<T>::Value) {
		static_assert(IsByVal<typename IsSetLike<T>::TKey>, "Set key is a type that we cannot serialize by-value?");
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

template <class... Args>
inline auto MakeObjectRef(lua_State* L, std::variant<Args...>* value, LifetimeHandle const& lifetime)
{
	std::visit([=](auto& arg) { push(L, &arg, lifetime); }, *value);
}

template <class T>
inline void push(lua_State* L, T& value, LifetimeHandle const& lifetime)
{
	if constexpr (IsByVal<T>) {
		push(L, value);
	} else {
		MakeObjectRef(L, &value, lifetime);
	}
}

template <class T>
inline void push(lua_State* L, T* value, LifetimeHandle const& lifetime)
{
	if (value == nullptr) {
		push(L, nullptr);
	} else if constexpr (IsByVal<T>) {
		push(L, *value);
	} else {
		MakeObjectRef(L, value, lifetime);
	}
}

END_NS()
