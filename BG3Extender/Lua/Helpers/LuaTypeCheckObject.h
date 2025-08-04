#pragma once

BEGIN_NS(lua)

template <class... Args>
typename bool do_typecheck(lua_State* L, int index, Overload<std::variant<Args...>>)
{
    // FIXME!
    return false;
}

template <class T>
typename std::enable_if_t<!IsByVal<T> && !std::is_pointer_v<T>, bool> do_typecheck(lua_State* L, int index, Overload<T>)
{
    return lua_typecheck_cppvalue(L, index, MetatableTag::ObjectRef, StructID<T>::ID)
        || lua_type(L, index) == LUA_TTABLE;
}

template <class T>
typename std::enable_if_t<!IsByVal<T>&& std::is_pointer_v<T>, bool> do_typecheck(lua_State* L, int index, Overload<T>)
{
    using TVal = std::remove_pointer_t<T>;
    if constexpr (IsArrayLike<TVal>::Value || IsSetLike<TVal>::Value || IsMapLike<TVal>::Value) {
        return false;
    } else {
        return lua_typecheck_cppvalue(L, index, MetatableTag::ObjectRef, StructID<TVal>::ID)
            // Assume any table is compatible with the definition until we figure out a better way
            || lua_type(L, index) == LUA_TTABLE;
    }
}

END_NS()
