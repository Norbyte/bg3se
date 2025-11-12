#pragma once

BEGIN_NS(lua)

template <class T>
typename std::enable_if_t<!IsByVal<T> && !std::is_pointer_v<T>, bool> do_typecheck(lua_State* L, int index, Overload<T>)
{
    if constexpr (IsArrayLike<T>::Value || IsSetLike<T>::Value || IsMapLike<T>::Value) {
        // TODO - currently no typechecking supported for cppobject assignment or contents of variant array/set types
        return lua_type(L, index) == LUA_TTABLE;
    } else {
        return lua_typecheck_cppvalue(L, index, MetatableTag::ObjectRef, StructID<T>::ID)
            || (lua_type(L, index) == LUA_TTABLE && lua_typecheck_struct(L, index, StructID<T>::ID));
    }
}

template <class T>
typename std::enable_if_t<!IsByVal<T>&& std::is_pointer_v<T>, bool> do_typecheck(lua_State* L, int index, Overload<T>)
{
    using TVal = std::remove_pointer_t<T>;
    if constexpr (IsArrayLike<TVal>::Value || IsSetLike<TVal>::Value || IsMapLike<TVal>::Value) {
        return false;
    } else {
        return lua_typecheck_cppvalue(L, index, MetatableTag::ObjectRef, StructID<TVal>::ID)
            || (lua_type(L, index) == LUA_TTABLE && lua_typecheck_struct(L, index, StructID<TVal>::ID));
    }
}

END_NS()
