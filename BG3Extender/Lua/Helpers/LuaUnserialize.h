#pragma once

BEGIN_NS(lua)

template <class TK>
PropertyOperationResult UnserializeArray(lua_State* L, int index, Array<TK>* obj)
{
	if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);

	obj->clear();
	for (auto idx : iterate(L, index)) {
		TK value;
		Unserialize(L, idx, &value);
		// FIXME - in-place unserialize
		obj->push_back(value);
	}

	return PropertyOperationResult::Success;
}

template <class TK>
PropertyOperationResult UnserializeArray(lua_State* L, int index, ObjectSet<TK>* obj)
{
	if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);

	obj->clear();
	for (auto idx : iterate(L, index)) {
		TK value;
		Unserialize(L, idx, &value);
		// FIXME - in-place unserialize
		obj->push_back(value);
	}

	return PropertyOperationResult::Success;
}

template <class TK, size_t Size>
PropertyOperationResult UnserializeArray(lua_State* L, int index, std::array<TK, Size>* obj)
{
	if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);

	for (uint32_t i = 0; i < obj->size(); i++) {
		lua_rawgeti(L, index, i + 1);
		TK value;
		Unserialize(L, index + 1, &value);
		// FIXME - in-place unserialize
		(*obj)[i] = value;
		lua_pop(L, 1);
	}

	return PropertyOperationResult::Success;
}

template <class TK, class TV>
PropertyOperationResult UnserializeMap(lua_State* L, int index, MultiHashMap<TK, TV>* obj)
{
	if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
		|| !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);

	obj->clear();
	for (auto idx : iterate(L, index)) {
		auto key = get<TK>(L, idx);
		TV value;
		Unserialize(L, idx + 1, &value);
		// FIXME - in-place unserialize
		obj->Set(key, value);
	}

	return PropertyOperationResult::Success;
}

template <class TK, class TV>
PropertyOperationResult UnserializeMap(lua_State* L, int index, RefMap<TK, TV>* obj)
{
	if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
		|| !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);

	obj->clear();
	for (auto idx : iterate(L, index)) {
		auto key = get<TK>(L, idx);
		auto value = obj->get_or_insert(key);
		Unserialize(L, idx + 1, value);
	}

	return PropertyOperationResult::Success;
}

template <class TK, class TV>
PropertyOperationResult UnserializeMap(lua_State* L, int index, Map<TK, TV>* obj)
{
	if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
		|| !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);

	obj->clear();
	for (auto idx : iterate(L, index)) {
		auto key = get<TK>(L, idx);
		auto value = obj->get_or_insert(key);
		Unserialize(L, idx + 1, value);
	}

	return PropertyOperationResult::Success;
}

template <class TK>
void UnserializeSet(lua_State* L, int index, MultiHashSet<TK>* obj)
{
	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);

	obj->clear();
	for (auto idx : iterate(L, index)) {
		obj->Add(get<TK>(L, idx));
	}
}

template <class... Args>
PropertyOperationResult UnserializeVariant(lua_State* L, int index, std::variant<Args...>* obj)
{
	StackCheck _(L);
	// FIXME - not supported until we figure out how to serialize type selection
	return PropertyOperationResult::UnsupportedType;
	// return std::visit([=](auto& var) { return Unserialize(L, index, &var); }, *obj);
}

void UnserializeRawObject(lua_State* L, int index, void* obj, GenericPropertyMap const& pm);

template <class T>
inline void UnserializeObject(lua_State* L, int index, T* obj)
{
	UnserializeRawObject(L, index, obj, StaticLuaPropertyMap<T>::PropertyMap);
}

template <class T>
inline PropertyOperationResult Unserialize(lua_State* L, int index, T* obj)
{
	if constexpr (std::is_pointer_v<T>) {
		return PropertyOperationResult::UnsupportedType;
	} else if constexpr (IsByVal<T>) {
		*obj = get<T>(L, index);
	} else if constexpr (IsArrayLike<T>::Value) {
		return UnserializeArray(L, lua_absindex(L, index), obj);
	} else if constexpr (IsMapLike<T>::Value) {
		return UnserializeMap(L, lua_absindex(L, index), obj);
	} else if constexpr (IsSetLike<T>::Value) {
		UnserializeSet(L, lua_absindex(L, index), obj);
	} else if constexpr (IsVariantLike<T>::Value) {
		return UnserializeVariant(L, index, obj);
	} else {
		UnserializeObject(L, lua_absindex(L, index), obj);
	}

	return PropertyOperationResult::Success;
}

template <class T>
inline PropertyOperationResult Unserialize(lua_State* L, int index, OverrideableProperty<T>* obj)
{
	return Unserialize(L, index, &obj->Value);
}

END_NS()
