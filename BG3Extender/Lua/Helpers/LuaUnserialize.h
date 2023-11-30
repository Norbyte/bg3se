#pragma once

BEGIN_NS(lua)

template <class TK>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, Array<TK>* obj)
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
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, Array<TK>* obj)
{
	if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	auto arr = ArrayProxyMetatable::Get<DynamicArrayProxyImpl<Array<TK>, TK, 2>>(L, index);
	*obj = *arr;
	return PropertyOperationResult::Success;
}

template <class TK>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, StaticArray<TK>* obj)
{
	if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>)
	{
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);
	uint32_t size = (uint32_t) lua_rawlen(L, index);

	obj->clear();
	obj->Resize(size);
	int i = 0;
	for (auto idx : iterate(L, index))
	{
		Unserialize(L, idx, &obj[i++]);
	}

	return PropertyOperationResult::Success;
}

template <class TK>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, StaticArray<TK>* obj)
{
	if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>)
	{
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	auto arr = ArrayProxyMetatable::Get<ConstSizeArrayProxyImpl<StaticArray<TK>, TK, 2>>(L, index);
	*obj = *arr;
	return PropertyOperationResult::Success;
}

template <class TK>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, ObjectSet<TK>* obj)
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
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, ObjectSet<TK>* obj)
{
	if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	auto arr = ArrayProxyMetatable::Get<DynamicArrayProxyImpl<ObjectSet<TK>, TK, 3>>(L, index);
	*obj = *arr;
	return PropertyOperationResult::Success;
}

template <class TK, size_t Size>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, std::array<TK, Size>* obj)
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

template <class TK, size_t Size>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, std::array<TK, Size>* obj)
{
	if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	auto arr = ArrayProxyMetatable::Get<ConstSizeArrayProxyImpl<std::array<TK, Size>, TK, 1>>(L, index);
	*obj = *arr;
	return PropertyOperationResult::Success;
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromTable(lua_State* L, int index, MultiHashMap<TK, TV>* obj)
{
	if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
		|| !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);

	obj->clear();
	for (auto idx : iterate(L, index)) {
		auto key = get<TK>(L, -2);
		TV value;
		Unserialize(L, -1, &value);
		// FIXME - in-place unserialize
		obj->Set(key, value);
	}

	return PropertyOperationResult::Success;
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromUserdata(lua_State* L, int index, MultiHashMap<TK, TV>* obj)
{
	if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
		|| !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	auto map = MapProxyMetatable::Get<MultiHashMapProxyImpl<TK, TV>>(L, index);
	*obj = *map;
	return PropertyOperationResult::Success;
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromTable(lua_State* L, int index, RefMap<TK, TV>* obj)
{
	if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
		|| !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);

	obj->clear();
	for (auto idx : iterate(L, index)) {
		auto key = get<TK>(L, -2);
		auto value = obj->get_or_insert(key);
		Unserialize(L, -1, value);
	}

	return PropertyOperationResult::Success;
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromUserdata(lua_State* L, int index, RefMap<TK, TV>* obj)
{
	if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
		|| !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	auto map = MapProxyMetatable::Get<RefMapProxyImpl<TK, TV, RefMapInternals<TK, TV>, 2>>(L, index);
	*obj = *map;
	return PropertyOperationResult::Success;
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromTable(lua_State* L, int index, Map<TK, TV>* obj)
{
	if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
		|| !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);

	obj->clear();
	for (auto idx : iterate(L, index)) {
		auto key = get<TK>(L, -2);
		auto value = obj->get_or_insert(key);
		Unserialize(L, -1, value);
	}

	return PropertyOperationResult::Success;
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromUserdata(lua_State* L, int index, Map<TK, TV>* obj)
{
	if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
		|| !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
		return PropertyOperationResult::UnsupportedType;
	}

	StackCheck _(L);
	auto map = MapProxyMetatable::Get<RefMapProxyImpl<TK, TV, MapInternals<TK, TV>, 3>>(L, index);
	*obj = *map;
	return PropertyOperationResult::Success;
}

template <class TK>
void UnserializeSetFromTable(lua_State* L, int index, MultiHashSet<TK>* obj)
{
	StackCheck _(L);
	luaL_checktype(L, index, LUA_TTABLE);

	obj->clear();
	for (auto idx : iterate(L, index)) {
		obj->Add(get<TK>(L, idx));
	}
}

template <class TK>
void UnserializeSetFromUserdata(lua_State* L, int index, MultiHashSet<TK>* obj)
{
	StackCheck _(L);
	auto set = SetProxyMetatable::Get<MultiHashSetProxyImpl<TK>>(L, index);
	*obj = *set;
}

template <class... Args>
PropertyOperationResult UnserializeVariantFromTable(lua_State* L, int index, std::variant<Args...>* obj)
{
	StackCheck _(L);
	// FIXME - not supported until we figure out how to serialize type selection
	return PropertyOperationResult::UnsupportedType;
	// return std::visit([=](auto& var) { return Unserialize(L, index, &var); }, *obj);
}

void UnserializeRawObjectFromTable(lua_State* L, int index, void* obj, GenericPropertyMap const& pm);
void UnserializeRawObjectFromUserdata(lua_State* L, int index, void* obj, GenericPropertyMap const& pm);

template <class T>
inline void UnserializeObjectFromTable(lua_State* L, int index, T* obj)
{
	UnserializeRawObjectFromTable(L, index, obj, StaticLuaPropertyMap<T>::PropertyMap);
}

template <class T>
inline void UnserializeObjectFromUserdata(lua_State* L, int index, T* obj)
{
	UnserializeRawObjectFromUserdata(L, index, obj, StaticLuaPropertyMap<T>::PropertyMap);
}

template <class T>
inline PropertyOperationResult Unserialize(lua_State* L, int index, T* obj)
{
	index = lua_absindex(L, index);
	if constexpr (std::is_pointer_v<T>) {
		return PropertyOperationResult::UnsupportedType;
	} else if constexpr (IsByVal<T>) {
		*obj = get<T>(L, index);
	} else {
		bool isTable = lua_type(L, index) == LUA_TTABLE;
		if constexpr (IsArrayLike<T>::Value) {
			if (isTable) {
				return UnserializeArrayFromTable(L, index, obj);
			} else {
				return UnserializeArrayFromUserdata(L, index, obj);
			}
		} else if constexpr (IsMapLike<T>::Value) {
			if (isTable) {
				return UnserializeMapFromTable(L, index, obj);
			} else {
				return UnserializeMapFromUserdata(L, index, obj);
			}
		} else if constexpr (IsSetLike<T>::Value) {
			if (isTable) {
				UnserializeSetFromTable(L, index, obj);
			} else {
				UnserializeSetFromUserdata(L, index, obj);
			}
		} else if constexpr (IsVariantLike<T>::Value) {
			if (isTable) {
				return UnserializeVariantFromTable(L, index, obj);
			} else {
				// References to variants are not kept in userspace
				return PropertyOperationResult::UnsupportedType;
			}
		} else if constexpr (IsOptional<T>::Value) {
			if (lua_type(L, index) == LUA_TNIL) {
				*obj = {};
			} else {
				*obj = typename IsOptional<T>::ValueType{};
				return Unserialize(L, index, &obj->value());
			}
		} else {
			if (isTable) {
				UnserializeObjectFromTable(L, index, obj);
			} else {
				UnserializeObjectFromUserdata(L, index, obj);
			}
		}
	}

	return PropertyOperationResult::Success;
}

template <class T>
inline PropertyOperationResult Unserialize(lua_State* L, int index, OverrideableProperty<T>* obj)
{
	return Unserialize(L, index, &obj->Value);
}

END_NS()
