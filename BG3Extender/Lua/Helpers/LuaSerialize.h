#pragma once

BEGIN_NS(lua)

template <class TK>
void SerializeArray(lua_State* L, Array<TK>* obj)
{
	StackCheck _(L, 1);
	lua_createtable(L, (int)obj->size(), 0);
	for (uint32_t i = 0; i < obj->size(); i++) {
		Serialize(L, &(*obj)[i]);
		lua_rawseti(L, -2, i);
	}
}

template <class TK>
void SerializeArray(lua_State* L, StaticArray<TK>* obj)
{
	StackCheck _(L, 1);
	lua_createtable(L, (int)obj->size(), 0);
	for (uint32_t i = 0; i < obj->size(); i++)
	{
		Serialize(L, &(*obj)[i]);
		lua_rawseti(L, -2, i);
	}
}

template <class TK>
void SerializeArray(lua_State* L, ObjectSet<TK>* obj)
{
	StackCheck _(L, 1);
	lua_createtable(L, (int)obj->size(), 0);
	for (uint32_t i = 0; i < obj->size(); i++) {
		Serialize(L, &(*obj)[i]);
		lua_rawseti(L, -2, i);
	}
}

template <class TK, size_t Size>
void SerializeArray(lua_State* L, std::array<TK, Size>* obj)
{
	StackCheck _(L, 1);
	lua_createtable(L, (int)obj->size(), 0);
	for (uint32_t i = 0; i < obj->size(); i++) {
		Serialize(L, &(*obj)[i]);
		lua_rawseti(L, -2, i);
	}
}

template <class TK, class TV>
void SerializeMap(lua_State* L, MultiHashMap<TK, TV>* obj)
{
	StackCheck _(L, 1);
	lua_createtable(L, 0, (int)obj->size());
	for (auto& kv : *obj) {
		Serialize(L, &kv.Key());
		Serialize(L, &kv.Value());
		lua_rawset(L, -2);
	}
}

template <class TK, class TV>
void SerializeMap(lua_State* L, RefMap<TK, TV>* obj)
{
	StackCheck _(L, 1);
	lua_createtable(L, 0, (int)obj->size());
	for (auto& kv : *obj) {
		Serialize(L, &kv.Key);
		Serialize(L, &kv.Value);
		lua_rawset(L, -2);
	}
}

template <class TK, class TV>
void SerializeMap(lua_State* L, Map<TK, TV>* obj)
{
	StackCheck _(L, 1);
	lua_createtable(L, 0, (int)obj->size());
	for (auto& kv : *obj) {
		Serialize(L, &kv.Key);
		Serialize(L, &kv.Value);
		lua_rawset(L, -2);
	}
}

template <class TK>
void SerializeSet(lua_State* L, MultiHashSet<TK>* obj)
{
	StackCheck _(L, 1);
	lua_createtable(L, (int)obj->size(), 0);
	for (uint32_t i = 0; i < obj->size(); i++) {
		Serialize(L, &obj->Keys[i]);
		lua_rawseti(L, -2, i);
	}
}

template <class... Args>
void SerializeVariant(lua_State* L, std::variant<Args...>* obj)
{
	StackCheck _(L, 1);
	return std::visit([=](auto& var) { return Serialize(L, &var); }, *obj);
}

void SerializeRawObject(lua_State* L, void* obj, GenericPropertyMap const& pm);

template <class T>
inline void SerializeObject(lua_State* L, T* obj)
{
	SerializeRawObject(L, obj, StaticLuaPropertyMap<T>::PropertyMap);
}

template <class T>
inline void SerializeObject(lua_State* L, std::optional<T>* obj)
{
	if (obj->has_value()) {
		SerializeObject(L, &obj->value());
	} else {
		push(L, nullptr);
	}
}

template <class T>
inline void Serialize(lua_State* L, T* obj)
{
	if constexpr (std::is_pointer_v<T>) {
		if (obj) {
			Serialize(L, *obj);
		} else {
			push(L, nullptr);
		}
	} else if constexpr (IsByVal<T>) {
		push(L, *obj);
	} else if constexpr (IsArrayLike<T>::Value) {
		SerializeArray(L, obj);
	} else if constexpr (IsMapLike<T>::Value) {
		SerializeMap(L, obj);
	} else if constexpr (IsSetLike<T>::Value) {
		SerializeSet(L, obj);
	} else if constexpr (IsVariantLike<T>::Value) {
		SerializeVariant(L, obj);
	} else if constexpr (IsOptional<T>::Value) {
		if (obj->has_value()) {
			Serialize(L, &obj->value());
		} else {
			push(L, nullptr);
		}
	} else {
		SerializeObject(L, obj);
	}
}

template <class T>
inline void Serialize(lua_State* L, OverrideableProperty<T>* obj)
{
	Serialize(L, &obj->Value);
}

END_NS()
