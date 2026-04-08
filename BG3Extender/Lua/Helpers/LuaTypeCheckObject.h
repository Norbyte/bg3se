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

template <class T> requires !IsByVal<T> && !std::is_pointer_v<T>
bool do_typecheck(lua_State* L, int index, Overload<T>)
{
    if constexpr (IsArray<T> || IsSet<T> || IsMap<T>) {
        // TODO - currently no typechecking supported for cppobject assignment or contents of variant array/set types
        return lua_type(L, index) == LUA_TTABLE;
    } else {
        return lua_typecheck_cppvalue(L, index, MetatableTag::ObjectRef, (int32_t)StructID<T>)
            || (lua_type(L, index) == LUA_TTABLE && lua_typecheck_struct(L, index, StructID<T>));
    }
}

template <class T> requires !IsByVal<T>&& std::is_pointer_v<T>
bool do_typecheck(lua_State* L, int index, Overload<T>)
{
    using TVal = std::remove_pointer_t<T>;
    if constexpr (IsArray<TVal> || IsSet<TVal> || IsMap<TVal>) {
        return false;
    } else {
        return lua_typecheck_cppvalue(L, index, MetatableTag::ObjectRef, (int32_t)StructID<TVal>)
            || (lua_type(L, index) == LUA_TTABLE && lua_typecheck_struct(L, index, StructID<TVal>));
    }
}

END_NS()
