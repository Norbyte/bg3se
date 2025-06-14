#pragma once

BEGIN_NS(lua)

template <class TK>
void SerializeArray(lua_State* L, Array<TK> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, (int)obj->size(), 0);
    for (uint32_t i = 0; i < obj->size(); i++) {
        Serialize(L, &(*obj)[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

template <class TK>
void SerializeArray(lua_State* L, StaticArray<TK> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, (int)obj->size(), 0);
    for (uint32_t i = 0; i < obj->size(); i++)
    {
        Serialize(L, &(*obj)[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

template <class TK, class TAlloc>
void SerializeArray(lua_State* L, std::vector<TK, TAlloc> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, (int)obj->size(), 0);
    for (uint32_t i = 0; i < obj->size(); i++) {
        Serialize(L, &(*obj)[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

template <class TK>
void SerializeArray(lua_State* L, ObjectSet<TK> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, (int)obj->size(), 0);
    for (uint32_t i = 0; i < obj->size(); i++) {
        Serialize(L, &(*obj)[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

template <class TK>
void SerializeArray(lua_State* L, TrackedCompactSet<TK> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, (int)obj->size(), 0);
    for (uint32_t i = 0; i < obj->size(); i++) {
        Serialize(L, &(*obj)[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

template <class TK>
void SerializeArray(lua_State* L, MiniCompactSet<TK> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, (int)obj->size(), 0);
    for (uint32_t i = 0; i < obj->size(); i++) {
        Serialize(L, &(*obj)[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

template <class TK>
void SerializeArray(lua_State* L, Queue<TK> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, (int)obj->size(), 0);
    for (uint32_t i = 0; i < obj->size(); i++) {
        Serialize(L, &(*obj)[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

template <class TK, unsigned N>
void SerializeArray(lua_State* L, Noesis::Vector<TK, N> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, (int)obj->Size(), 0);
    for (uint32_t i = 0; i < obj->Size(); i++) {
        Serialize(L, &(*obj)[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

void SerializeArray(lua_State* L, Noesis::BaseCollection const* obj);

template <class TK, size_t Size>
void SerializeArray(lua_State* L, std::array<TK, Size> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, (int)obj->size(), 0);
    for (uint32_t i = 0; i < obj->size(); i++) {
        Serialize(L, &(*obj)[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

template <class TWord, unsigned Words>
void SerializeArray(lua_State* L, BitArray<TWord, Words> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, obj->size(), 0);
    for (uint32_t i = 0; i < obj->size(); i++) {
        bool isSet = obj->IsSet(i);
        Serialize(L, &isSet);
        lua_rawseti(L, -2, i + 1);
    }
}

template <class T, size_t Extent>
void SerializeArray(lua_State* L, std::span<T, Extent> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, (int)obj->size(), 0);
    for (uint32_t i = 0; i < obj->size(); i++) {
        Serialize(L, &(*obj)[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

template <class TK, class TV>
void SerializeMap(lua_State* L, HashMap<TK, TV> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, 0, (int)obj->size());
    for (auto& kv : *obj) {
        Serialize(L, &kv.Key());
        Serialize(L, &kv.Value());
        lua_rawset(L, -3);
    }
}

template <class TK, class TV>
void SerializeMap(lua_State* L, LegacyRefMap<TK, TV> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, 0, (int)obj->size());
    for (auto& kv : *obj) {
        Serialize(L, &kv.Key);
        Serialize(L, &kv.Value);
        lua_rawset(L, -3);
    }
}

template <class TK, class TV>
void SerializeMap(lua_State* L, LegacyMap<TK, TV> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, 0, (int)obj->size());
    for (auto& kv : *obj) {
        Serialize(L, &kv.Key);
        Serialize(L, &kv.Value);
        lua_rawset(L, -3);
    }
}

template <class TK>
void SerializeSet(lua_State* L, HashSet<TK> const* obj)
{
    StackCheck _(L, 1);
    lua_createtable(L, (int)obj->size(), 0);
    for (uint32_t i = 0; i < obj->size(); i++) {
        Serialize(L, &obj->keys()[i]);
        lua_rawseti(L, -2, i + 1);
    }
}

template <class... Args>
void SerializeVariant(lua_State* L, std::variant<Args...> const* obj)
{
    StackCheck _(L, 1);
    return std::visit([=](auto& var) { return Serialize(L, &var); }, *obj);
}

void SerializeRawObject(lua_State* L, void const* obj, GenericPropertyMap const& pm);

template <class T>
inline void SerializeObject(lua_State* L, T const* obj)
{
    SerializeRawObject(L, obj, GetStaticPropertyMap<T>());
}

template <class T>
inline void SerializeObject(lua_State* L, std::optional<T> const* obj)
{
    if (obj->has_value()) {
        SerializeObject(L, &obj->value());
    } else {
        push(L, nullptr);
    }
}

template <class T>
inline void Serialize(lua_State* L, T const* obj)
{
    if constexpr (std::is_pointer_v<T>) {
        if (*obj) {
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
inline void Serialize(lua_State* L, OverrideableProperty<T> const* obj)
{
    Serialize(L, &obj->Value);
}

END_NS()
