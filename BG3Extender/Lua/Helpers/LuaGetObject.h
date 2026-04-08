#pragma once

BEGIN_NS(lua)

template <class T> requires !IsByVal<T> && !std::is_pointer_v<T>
T do_get(lua_State* L, int index, Overload<T>)
{
    T val{};
    Unserialize(L, index, &val);
    return val;
}

template <class T> requires !IsByVal<T> && std::is_pointer_v<T>
T do_get(lua_State* L, int index, Overload<T>)
{
    using TVal = std::remove_pointer_t<T>;
    if constexpr (IsArray<TVal> || IsSet<TVal> || IsMap<TVal>) {
        luaL_error(L, "Unable to unserialize container types");
        return nullptr;
    } else {
        return GetObjectRef<TVal>(L, index);
    }
}

template <class T>
inline bool typecheck(lua_State* L, int index)
{
    return do_typecheck(L, index, Overload<T>{});
}

END_NS()
