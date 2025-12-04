#pragma once

BEGIN_NS(lua)

template <class TV, unsigned N>
bool do_typecheck_variant(lua_State* L, int index)
{
    if constexpr (N < std::variant_size_v<TV>) {
        using T = std::variant_alternative_t<N, TV>;
        if (do_typecheck(L, index, Overload<T>{})) {
            return true;
        } else {
            return do_typecheck_variant<TV, N + 1>(L, index);
        }
    } else {
        return false;
    }
}

template <class... Args>
typename bool do_typecheck(lua_State* L, int index, Overload<std::variant<Args...>>)
{
    return do_typecheck_variant<std::variant<Args...>, 0>(L, index);
}

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
