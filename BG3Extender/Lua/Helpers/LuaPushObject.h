#pragma once

BEGIN_NS(lua)

LifetimeHandle GetCurrentLifetime(lua_State* L);

template <class T>
inline void MakeObjectRef(lua_State* L, T* value, LifetimeHandle const& lifetime)
{
	using TVal = std::remove_cv_t<T>;

	if (value == nullptr) {
		push(L, nullptr);
		return;
	}

	if constexpr (LuaPolymorphic<TVal>::IsPolymorphic) {
		return LuaPolymorphic<TVal>::MakeRef(L, value, lifetime);
	} else if constexpr (IsArrayLike<TVal>::Value) {
		ArrayProxyMetatable::Make(L, value, lifetime);
	} else if constexpr (IsMapLike<TVal>::Value) {
		MapProxyMetatable::Make(L, value, lifetime);
	} else if constexpr (IsSetLike<TVal>::Value) {
		SetProxyMetatable::Make(L, value, lifetime);
	} else if constexpr (std::is_pointer_v<T>) {
		if constexpr (std::is_const_v<std::remove_pointer_t<T>>) {
			MakeObjectRef(L, const_cast<std::remove_const_t<std::remove_pointer_t<T>>*>(*value), lifetime);
		} else {
			MakeObjectRef(L, *value, lifetime);
		}
	} else if constexpr (IsOptional<TVal>::Value) {
		if (value->has_value()) {
			MakeObjectRef(L, &value->value(), lifetime);
		} else {
			push(L, nullptr);
		}
	} else {
		if constexpr (LuaLifetimeInfo<TVal>::HasInfiniteLifetime) {
			ObjectProxy::MakeRef<T>(L, value, State::FromLua(L)->GetGlobalLifetime());
		} else {
			ObjectProxy::MakeRef<T>(L, value, lifetime);
		}
	}
}

template <class T>
inline void MakeObjectCopy(lua_State* L, T&& value)
{
	using TVal = std::remove_cv_t<T>;

	static_assert(!LuaPolymorphic<TVal>::IsPolymorphic, "Copying polymorphic types not supported");
	static_assert(
		!IsArrayLike<TVal>::Value && !IsMapLike<TVal>::Value && !IsSetLike<TVal>::Value && !std::is_pointer_v<T> && !IsOptional<TVal>::Value,
		"Copying special types not supported");
	// FIXME - ObjectProxy::MakeRef<T>(L, std::move(value));
}

inline void MakeDirectObjectRef(lua_State* L, GenericPropertyMap& pm, void* value, LifetimeHandle const& lifetime)
{
	if (value == nullptr) {
		push(L, nullptr);
	} else {
		ObjectProxy::MakeRef(L, pm, value, lifetime);
	}
}

template <class T>
inline void MakeDirectObjectRef(lua_State* L, T* value, LifetimeHandle const& lifetime)
{
	if (value == nullptr) {
		push(L, nullptr);
	} else if constexpr (LuaLifetimeInfo<T>::HasInfiniteLifetime) {
		ObjectProxy::MakeRef<T>(L, value, State::FromLua(L)->GetGlobalLifetime());
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
inline auto MakeObjectRef(lua_State* L, OverrideableProperty<T> const* value, LifetimeHandle const& lifetime)
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

template <class... Args>
inline auto MakeObjectRef(lua_State* L, std::variant<Args...> const* value, LifetimeHandle const& lifetime)
{
	std::visit([=](auto& arg) { push(L, &arg, lifetime); }, *value);
}

template <class T>
inline void push(lua_State* L, T& value, LifetimeHandle const& lifetime)
{
	if constexpr (IsByVal<std::remove_cv_t<T>>) {
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
	} else if constexpr (IsByVal<std::remove_cv_t<T>>) {
		push(L, *value);
	} else {
		MakeObjectRef(L, value, lifetime);
	}
}

END_NS()
