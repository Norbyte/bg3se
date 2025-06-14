#pragma once

BEGIN_NS(lua)

template <class TK>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, Array<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        luaL_checktype(L, index, LUA_TTABLE);

        // FIXME - in-place resize object by using raw size from the Lua table meta
        // (this will also require the table to be array-like)
        obj->clear();
        for (auto idx : iterate(L, index)) {
            TK value;
            Unserialize(L, idx, &value);
            obj->push_back(std::move(value));
        }

        return PropertyOperationResult::Success;
    }
}

template <class TK>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, Array<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto arr = ArrayProxyMetatable::Get<DynamicArrayProxyImpl<Array<TK>, TK, 2>>(L, index);
        *obj = *arr;
        return PropertyOperationResult::Success;
    }
}

template <class TK>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, StaticArray<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        // FIXME - this will not work well for hash-like tables
        StackCheck _(L);
        luaL_checktype(L, index, LUA_TTABLE);
        uint32_t size = (uint32_t)lua_rawlen(L, index);

        obj->clear();
        obj->Resize(size);
        int i = 0;
        for (auto idx : iterate(L, index)) {
            obj[i++] = {};
            Unserialize(L, idx, &obj[i++]);
        }

        return PropertyOperationResult::Success;
    }
}

template <class TK>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, StaticArray<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto arr = ArrayProxyMetatable::Get<ConstSizeArrayProxyImpl<StaticArray<TK>, TK, 2>>(L, index);
        *obj = *arr;
        return PropertyOperationResult::Success;
    }
}

template <class TK, class TAllocator>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, std::vector<TK, TAllocator>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        luaL_checktype(L, index, LUA_TTABLE);

        // FIXME - in-place resize object by using raw size from the Lua table meta
        // (this will also require the table to be array-like)
        obj->clear();
        for (auto idx : iterate(L, index)) {
            TK value;
            Unserialize(L, idx, &value);
            obj->push_back(std::move(value));
        }

        return PropertyOperationResult::Success;
    }
}

template <class TK, class TAllocator>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, std::vector<TK, TAllocator>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto arr = ArrayProxyMetatable::Get<DynamicArrayProxyImpl<std::vector<TK, TAllocator>, TK, 4>>(L, index);
        *obj = *arr;
        return PropertyOperationResult::Success;
    }
}

template <class TK>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, ObjectSet<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        luaL_checktype(L, index, LUA_TTABLE);

        // FIXME - in-place resize object by using raw size from the Lua table meta
        // (this will also require the table to be array-like)
        obj->clear();
        for (auto idx : iterate(L, index)) {
            TK value;
            Unserialize(L, idx, &value);
            obj->push_back(std::move(value));
        }

        return PropertyOperationResult::Success;
    }
}

template <class TK>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, ObjectSet<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto arr = ArrayProxyMetatable::Get<DynamicArrayProxyImpl<ObjectSet<TK>, TK, 3>>(L, index);
        *obj = *arr;
        return PropertyOperationResult::Success;
    }
}

template <class TK>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, TrackedCompactSet<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        luaL_checktype(L, index, LUA_TTABLE);

        // FIXME - in-place resize object by using raw size from the Lua table meta
        // (this will also require the table to be array-like)
        obj->clear();
        for (auto idx : iterate(L, index)) {
            TK value;
            Unserialize(L, idx, &value);
            obj->push_back(std::move(value));
        }

        return PropertyOperationResult::Success;
    }
}

template <class TK>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, TrackedCompactSet<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto arr = ArrayProxyMetatable::Get<DynamicArrayProxyImpl<TrackedCompactSet<TK>, TK, 8>>(L, index);
        *obj = *arr;
        return PropertyOperationResult::Success;
    }
}

template <class TK>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, MiniCompactSet<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        luaL_checktype(L, index, LUA_TTABLE);

        // FIXME - in-place resize object by using raw size from the Lua table meta
        // (this will also require the table to be array-like)
        obj->clear();
        for (auto idx : iterate(L, index)) {
            TK value;
            Unserialize(L, idx, &value);
            obj->push_back(std::move(value));
        }

        return PropertyOperationResult::Success;
    }
}

template <class TK>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, MiniCompactSet<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto arr = ArrayProxyMetatable::Get<DynamicArrayProxyImpl<MiniCompactSet<TK>, TK, 8>>(L, index);
        *obj = *arr;
        return PropertyOperationResult::Success;
    }
}

template <class TK>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, Queue<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        luaL_checktype(L, index, LUA_TTABLE);

        // FIXME - in-place resize object by using raw size from the Lua table meta
        // (this will also require the table to be array-like)
        obj->clear();
        for (auto idx : iterate(L, index)) {
            TK value;
            Unserialize(L, idx, &value);
            obj->push_back(std::move(value));
        }

        return PropertyOperationResult::Success;
    }
}

template <class TK>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, Queue<TK>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto arr = ArrayProxyMetatable::Get<DynamicArrayProxyImpl<Queue<TK>, TK, 6>>(L, index);
        *obj = *arr;
        return PropertyOperationResult::Success;
    }
}

template <class TK, unsigned N>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, Noesis::Vector<TK, N>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        luaL_checktype(L, index, LUA_TTABLE);

        // FIXME - in-place resize object by using raw size from the Lua table meta
        // (this will also require the table to be array-like)
        obj->Clear();
        for (auto idx : iterate(L, index)) {
            TK value;
            Unserialize(L, idx, &value);
            obj->PushBack(std::move(value));
        }

        return PropertyOperationResult::Success;
    }
}

template <class TK, unsigned N>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, Noesis::Vector<TK, N>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto arr = ArrayProxyMetatable::Get<DynamicArrayProxyImpl<Noesis::Vector<TK, N>, TK, 7>>(L, index);
        *obj = *arr;
        return PropertyOperationResult::Success;
    }
}

inline PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, Noesis::BaseCollection* obj)
{
    return PropertyOperationResult::UnsupportedType;
}

inline PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, Noesis::BaseCollection* obj)
{
    return PropertyOperationResult::UnsupportedType;
}

template <class TK, size_t Size>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, std::array<TK, Size>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        luaL_checktype(L, index, LUA_TTABLE);

        for (uint32_t i = 0; i < obj->size(); i++) {
            lua_rawgeti(L, index, i + 1);
            (*obj)[i] = TK{};
            Unserialize(L, index + 1, &(*obj)[i]);
            lua_pop(L, 1);
        }

        return PropertyOperationResult::Success;
    }
}

template <class TK, size_t Size>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, std::array<TK, Size>* obj)
{
    if constexpr (std::is_pointer_v<TK> || !std::is_default_constructible_v<TK>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto arr = ArrayProxyMetatable::Get<ConstSizeArrayProxyImpl<std::array<TK, Size>, TK, 1>>(L, index);
        *obj = *arr;
        return PropertyOperationResult::Success;
    }
}

template <class T, size_t Extent>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, std::span<T, Extent>* obj)
{
    if constexpr (std::is_pointer_v<T> || !std::is_default_constructible_v<T>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        luaL_checktype(L, index, LUA_TTABLE);

        for (uint32_t i = 0; i < obj->size(); i++) {
            lua_rawgeti(L, index, i + 1);
            (*obj)[i] = T{};
            Unserialize(L, index + 1, &(*obj)[i]);
            lua_pop(L, 1);
        }

        return PropertyOperationResult::Success;
    }
}

template <class T, size_t Extent>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, std::span<T, Extent>* obj)
{
    if constexpr (std::is_pointer_v<T> || !std::is_default_constructible_v<T>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto arr = ArrayProxyMetatable::Get<ConstSizeArrayProxyImpl<std::span<T, Extent>, T, 6>>(L, index);
        *obj = *arr;
        return PropertyOperationResult::Success;
    }
}

template <class TWord, unsigned Words>
PropertyOperationResult UnserializeArrayFromTable(lua_State* L, int index, BitArray<TWord, Words>* obj)
{
    StackCheck _(L);
    luaL_checktype(L, index, LUA_TTABLE);

    for (uint32_t i = 0; i < obj->size(); i++) {
        lua_rawgeti(L, index, i + 1);
        bool value;
        Unserialize(L, index + 1, &value);
        if (value)
        {
            obj->Set(i);
        }
        else
        {
            obj->Clear(i);
        }
        lua_pop(L, 1);
    }

    return PropertyOperationResult::Success;
}

template <class TWord, unsigned Words>
PropertyOperationResult UnserializeArrayFromUserdata(lua_State* L, int index, BitArray<TWord, Words>* obj)
{
    StackCheck _(L);
    auto arr = ArrayProxyMetatable::Get<ConstSizeArrayProxyImpl<BitArray<TWord, Words>, bool, 5>>(L, index);
    *obj = *arr;
    return PropertyOperationResult::Success;
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromTable(lua_State* L, int index, HashMap<TK, TV>* obj)
{
    if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
        || !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        luaL_checktype(L, index, LUA_TTABLE);

        obj->clear();
        for (auto idx : iterate(L, index)) {
            auto key = get<TK>(L, -2);
            auto value = obj->add_key(key);
            Unserialize(L, -1, &value);
        }

        return PropertyOperationResult::Success;
    }
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromUserdata(lua_State* L, int index, HashMap<TK, TV>* obj)
{
    if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
        || !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto map = MapProxyMetatable::Get<MultiHashMapProxyImpl<TK, TV>>(L, index);
        *obj = *map;
        return PropertyOperationResult::Success;
    }
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromTable(lua_State* L, int index, LegacyRefMap<TK, TV>* obj)
{
    if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
        || !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
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
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromUserdata(lua_State* L, int index, LegacyRefMap<TK, TV>* obj)
{
    if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
        || !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto map = MapProxyMetatable::Get<RefMapProxyImpl<TK, TV, RefMapInternals<TK, TV>, 2>>(L, index);
        *obj = *map;
        return PropertyOperationResult::Success;
    }
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromTable(lua_State* L, int index, LegacyMap<TK, TV>* obj)
{
    if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
        || !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
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
}

template <class TK, class TV>
PropertyOperationResult UnserializeMapFromUserdata(lua_State* L, int index, LegacyMap<TK, TV>* obj)
{
    if constexpr (std::is_pointer_v<TK> || std::is_pointer_v<TV>
        || !std::is_default_constructible_v<TK> || !std::is_default_constructible_v<TV>) {
        return PropertyOperationResult::UnsupportedType;
    } else {
        StackCheck _(L);
        auto map = MapProxyMetatable::Get<RefMapProxyImpl<TK, TV, MapInternals<TK, TV>, 3>>(L, index);
        *obj = *map;
        return PropertyOperationResult::Success;
    }
}

template <class TK>
void UnserializeSetFromTable(lua_State* L, int index, HashSet<TK>* obj)
{
    StackCheck _(L);
    luaL_checktype(L, index, LUA_TTABLE);

    obj->clear();
    for (auto idx : iterate(L, index)) {
        obj->insert(get<TK>(L, idx));
    }
}

template <class TK>
void UnserializeSetFromUserdata(lua_State* L, int index, HashSet<TK>* obj)
{
    StackCheck _(L);
    auto set = SetProxyMetatable::Get<MultiHashSetProxyImpl<TK>>(L, index);
    *obj = *set;
}

template <class TV, unsigned N>
PropertyOperationResult TryUnserializeVariantOpt(lua_State* L, int index, TV* obj)
{
    if constexpr (N < std::variant_size_v<TV>) {
        using T = std::variant_alternative_t<N, TV>;
        if (typecheck<T>(L, index)) {
            T alternative;
            auto ret = Unserialize(L, index, &alternative);
            if (ret == PropertyOperationResult::Success) {
                *obj = std::move(alternative);
            }
            return ret;
        } else {
            return TryUnserializeVariantOpt<TV, N+1>(L, index, obj);
        }
    } else {
        luaL_error(L, "Value type does not match any alternative in the variant");
        return PropertyOperationResult::Unknown;
    }
}

template <class T>
PropertyOperationResult UnserializeVariant(lua_State* L, int index, T* obj)
{
    StackCheck _(L);
    return TryUnserializeVariantOpt<T, 0>(L, index, obj);
}

void UnserializeRawObjectFromTable(lua_State* L, int index, void* obj, GenericPropertyMap const& pm);
void UnserializeRawObjectFromUserdata(lua_State* L, int index, void* obj, GenericPropertyMap const& pm);

template <class T>
inline void UnserializeObjectFromTable(lua_State* L, int index, T* obj)
{
    UnserializeRawObjectFromTable(L, index, obj, GetStaticPropertyMap<T>());
}

template <class T>
inline void UnserializeObjectFromUserdata(lua_State* L, int index, T* obj)
{
    UnserializeRawObjectFromUserdata(L, index, obj, GetStaticPropertyMap<T>());
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
            return UnserializeVariant(L, index, obj);
        } else if constexpr (IsOptional<T>::Value) {
            if constexpr (std::is_default_constructible_v<typename IsOptional<T>::ValueType>) {
                if (lua_type(L, index) == LUA_TNIL) {
                    *obj = {};
                } else {
                    *obj = typename IsOptional<T>::ValueType{};
                    return Unserialize(L, index, &obj->value());
                }
            } else {
                return PropertyOperationResult::UnsupportedType;
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
