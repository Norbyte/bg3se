#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/Proxies/LuaPropertyMapHelpers.h>

BEGIN_NS(lua)

template <class T>
inline void PushReturnValue(lua_State* L, T& v)
{
    if constexpr (std::is_pointer_v<T>) {
        MakeObjectRef(L, v);
    } else {
        LuaWrite(L, v);
    }
}

template <class T>
inline void PushReturnValue(lua_State* L, T const& v)
{
    if constexpr (std::is_pointer_v<T>) {
        MakeObjectRef(L, v);
    } else {
        LuaWrite(L, v);
    }
}

template <class T>
inline void PushReturnValue(lua_State* L, std::span<T>& v)
{
    MakeObjectRef(L, &v);
}

template <class T>
inline void PushReturnValue(lua_State* L, Array<T>* v)
{
    if constexpr (!IsByVal<T>) {
        MakeObjectRef(L, v);
    } else {
        LuaWrite(L, v);
    }
}

template <class T>
inline void PushReturnValue(lua_State* L, Vector<T>* v)
{
    if constexpr (!IsByVal<T>) {
        MakeObjectRef(L, v);
    } else {
        LuaWrite(L, v);
    }
}

template <class T>
inline void PushReturnValue(lua_State* L, ObjectSet<T>* v)
{
    if constexpr (!IsByVal<T>) {
        MakeObjectRef(L, v);
    } else {
        LuaWrite(L, v);
    }
}

template <class T>
inline void PushReturnValue(lua_State* L, TrackedCompactSet<T>* v)
{
    if constexpr (!IsByVal<T>) {
        MakeObjectRef(L, v);
    } else {
        LuaWrite(L, v);
    }
}

template <class T>
inline void PushReturnValue(lua_State* L, MiniCompactSet<T>* v)
{
    if constexpr (!IsByVal<T>) {
        MakeObjectRef(L, v);
    } else {
        LuaWrite(L, v);
    }
}

template <class TKey, class TValue>
inline void PushReturnValue(lua_State* L, LegacyRefMap<TKey, TValue>* v)
{
    if constexpr (!IsByVal<TValue>) {
        MakeObjectRef(L, v);
    } else {
        LuaWrite(L, v);
    }
}

template <class TKey, class TValue>
inline void PushReturnValue(lua_State* L, LegacyMap<TKey, TValue>* v)
{
    if constexpr (!IsByVal<TValue>) {
        MakeObjectRef(L, v);
    } else {
        LuaWrite(L, v);
    }
}

template <class T>
inline void PushReturnValue(lua_State* L, RefReturn<T> v)
{
    MakeObjectRef(L, v.Object);
}

template <class T>
inline void PushReturnValue(lua_State* L, CopyReturn<T>&& v)
{
    MakeObjectCopy(L, std::move(v.Object));
}

template <class T>
inline void PushReturnValue(lua_State* L, GlobalRefReturn<T> v)
{
    MakeObjectRef(L, v.Object, LifetimeHandle{});
}

template <class T>
inline void PushReturnValue(lua_State* L, ByValReturn<T> v)
{
    LuaWrite(L, v.Object);
}

template <class T>
inline void PushUserCallArg(lua_State* L, T const& v)
{
    if constexpr (std::is_pointer_v<std::remove_reference_t<T>>) {
        MakeObjectRef(L, v);
    } else {
        push(L, v);
    }
}

template <class ...Args, size_t ...Indices>
inline void PushTupleHelper(lua_State* L, std::tuple<Args...> const& v, std::index_sequence<Indices...>)
{
    (PushUserCallArg(L, std::get<Indices>(v)), ...);
}

template <class ...Args>
inline void PushUserCallArg(lua_State* L, std::tuple<Args...> const& v)
{
    PushTupleHelper(L, v, std::index_sequence_for<Args...>());
}

template <class ...Args>
inline void PushReturnValue(lua_State* L, std::tuple<Args...> v)
{
    PushTupleHelper(L, v, std::index_sequence_for<Args...>());
}

template <class T>
inline constexpr int TupleSize(Overload<T>)
{
    return 1;
}

template <class ...Args>
inline constexpr int TupleSize(Overload<std::tuple<Args...>>)
{
    return sizeof...(Args);
}

template <class T>
T* get_object(lua_State* L, int index)
{
    switch (lua_type(L, index)) {
    case LUA_TLIGHTCPPOBJECT:
        return ObjectProxy::Get<T>(L, index);

    default:
        luaL_error(L, "Argument %d: Expected %s, got %s", index, 
            GetStaticPropertyMap<T>().Name.GetString(), GetDebugName(L, index));
        return nullptr;
    }
}

template <class T>
inline ProxyParam<T> get_param(lua_State* L, int i, Overload<ProxyParam<T>>)
{
    return ProxyParam(get_object<T>(L, i));
}

template <class T>
inline std::optional<ProxyParam<T>> get_param(lua_State* L, int i, Overload<std::optional<ProxyParam<T>>>)
{
    if (lua_gettop(L) < i || lua_isnil(L, i)) {
        return {};
    } else {
        return ProxyParam(get_object<T>(L, i));
    }
}

template <class ...Args, size_t ...Indices>
inline std::tuple<Args...> FetchTupleHelper(lua_State* L, int index, std::index_sequence<Indices...>)
{
    return std::tuple(get_param<Args>(L, index + (int)Indices, Overload<Args>{})...);
}

// Index_sequence wrapper for FetchTupleHelper()
template <class ...Args>
inline std::tuple<Args...> get_param(lua_State* L, int i, Overload<std::tuple<Args...>>)
{
    return FetchTupleHelper<Args...>(L, i, std::index_sequence_for<Args...>());
}

// Helper for removing reference and CV-qualifiers before fetching a Lua value
template <class T>
inline std::remove_cv_t<std::remove_reference_t<T>> get_param_cv(lua_State* L, int i)
{
    return get_param(L, i, Overload<std::remove_cv_t<std::remove_reference_t<T>>>{});
}

int TracebackHandler(lua_State* L);

template <class TReturn>
struct ReturnValueContainer
{
    TReturn Value;

    void Fetch(lua_State* L)
    {
        Value = get_param_cv<TReturn>(L, -TupleSize(Overload<TReturn>{}));
    }
};

template <>
struct ReturnValueContainer<void>
{
    void Fetch(lua_State* L)
    {
    }
};


struct ProtectedMethodCallerBase
{
    Ref Self;
    char const* Method;

    bool ProtectedCall(lua_State* L, lua_CFunction fun);
    int CallUserFunctionWithTraceback(lua_State* L, lua_CFunction fun);
};

template <class TArgs, class TReturn>
struct ProtectedMethodCaller : public ProtectedMethodCallerBase
{
    TArgs Args;
    ReturnValueContainer<TReturn> Retval;
    bool Optional{ false };

    bool Call(lua_State* L)
    {
        return ProtectedCall(L, &ProtectedCtx);
    }

    static int ProtectedCtx(lua_State* L)
    {
        auto self = reinterpret_cast<ProtectedMethodCaller<TArgs, TReturn>*>(lua_touserdata(L, 1));

        LifetimeStackPin _p(State::FromLua(L)->GetStack());

        lua_pushvalue(L, 2);
        lua_getfield(L, -1, self->Method);
        if (self->Optional && lua_type(L, -1) == LUA_TNIL) {
            lua_pop(L, 1);
            return -1;
        }

        lua_pushvalue(L, 2);
        PushUserCallArg(L, self->Args);

        if (lua_pcall(L, 1 + TupleSize(Overload<TArgs>{}), TupleSize(Overload<TReturn>{}), 0) != LUA_OK) {
            return luaL_error(L, "%s", lua_tostring(L, -1));
        }

        self->Retval.Fetch(L);
        lua_pop(L, TupleSize(Overload<TReturn>{}));
        return 0;
    }
};


struct ProtectedFunctionCallerBase
{
    Ref Function;

    ProtectedFunctionCallerBase() {}
    ProtectedFunctionCallerBase(Ref const& fun) : Function(fun) {}

    bool ProtectedCall(lua_State* L, lua_CFunction fun, char const* funcDescription = nullptr);
    int CallUserFunction(lua_State* L, lua_CFunction fun);
};

template <class TArgs, class TReturn>
struct ProtectedFunctionCaller : public ProtectedFunctionCallerBase
{
    TArgs Args;
    ReturnValueContainer<TReturn> Retval;

    inline ProtectedFunctionCaller() {}

    inline ProtectedFunctionCaller(Ref const& fun, TArgs const& args) 
        : ProtectedFunctionCallerBase(fun),
        Args(args)
    {}

    inline ProtectedFunctionCaller(Ref const& fun, TArgs && args) 
        : ProtectedFunctionCallerBase(fun),
        Args(std::forward<TArgs>(args))
    {}

    bool Call(lua_State* L, char const* funcDescription = nullptr)
    {
        return ProtectedCall(L, &ProtectedCtx, funcDescription);
    }

    static int ProtectedCtx(lua_State* L)
    {
        auto self = reinterpret_cast<ProtectedFunctionCaller<TArgs, TReturn>*>(lua_touserdata(L, 1));

        LifetimeStackPin _p(L, State::FromLua(L)->GetStack());

        lua_pushcfunction(L, &TracebackHandler);
        int tracebackHandlerIdx = lua_gettop(L);

        lua_pushvalue(L, 2);
        PushUserCallArg(L, self->Args);

        auto result = lua_pcall(L, TupleSize(Overload<TArgs>{}), TupleSize(Overload<TReturn>{}), tracebackHandlerIdx);
        lua_remove(L, tracebackHandlerIdx);
        if (result != LUA_OK) {
            return luaL_error(L, "%s", lua_tostring(L, -1));
        }

        self->Retval.Fetch(L);
        lua_pop(L, TupleSize(Overload<TReturn>{}));
        return 0;
    }
};


// No return value, lua_State passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, void (T::* fun)(lua_State*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, 0);
    auto obj = get_object<T>(L, 1);
    (obj->*fun)(L, get_param_cv<Args>(L, 2 + (int)Indices)...);
    return 0;
}

// Const, No return value, lua_State passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, void (T::* fun)(lua_State*, Args...) const, std::index_sequence<Indices...>) {
    StackCheck _(L, 0);
    auto obj = get_object<T const>(L, 1);
    (obj->*fun)(L, get_param_cv<Args>(L, 2 + (int)Indices)...);
    return 0;
}

// Return values pushed by callee, lua_State passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, UserReturn (T::* fun)(lua_State*, Args...), std::index_sequence<Indices...>) {
    auto obj = get_object<T>(L, 1);
    auto nret = (obj->*fun)(L, get_param_cv<Args>(L, 2 + (int)Indices)...);
    return (int)nret;
}

// Const, Return values pushed by callee, lua_State passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, UserReturn (T::* fun)(lua_State*, Args...) const, std::index_sequence<Indices...>) {
    auto obj = get_object<T const>(L, 1);
    auto nret = (obj->*fun)(L, get_param_cv<Args>(L, 2 + (int)Indices)...);
    return (int)nret;
}

// 1 return value, lua_State passed
template <class R, class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, R (T::* fun)(lua_State*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, TupleSize(Overload<R>{}));
    auto obj = get_object<T>(L, 1);
    auto retval = (obj->*fun)(L, get_param_cv<Args>(L, 2 + (int)Indices)...);
    PushReturnValue(L, retval);
    return TupleSize(Overload<R>{});
}

// Const, 1 return value, lua_State passed
template <class R, class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, R (T::* fun)(lua_State*, Args...) const, std::index_sequence<Indices...>) {
    StackCheck _(L, TupleSize(Overload<R>{}));
    auto obj = get_object<T const>(L, 1);
    auto retval = (obj->*fun)(L, get_param_cv<Args>(L, 2 + (int)Indices)...);
    PushReturnValue(L, retval);
    return TupleSize(Overload<R>{});
}

// Index_sequence wrapper for CallMethodHelper(), lua_State passed
template <class R, class T, class ...Args>
inline int CallMethod(lua_State* L, R(T::* fun)(lua_State*, Args...)) {
    return CallMethodHelper(L, fun, std::index_sequence_for<Args...>());
}

// Const, Index_sequence wrapper for CallMethodHelper(), lua_State passed
template <class R, class T, class ...Args>
inline int CallMethod(lua_State* L, R(T::* fun)(lua_State*, Args...) const) {
    return CallMethodHelper(L, fun, std::index_sequence_for<Args...>());
}

// No return value, lua_State not passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, void (T::* fun)(Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, 0);
    auto obj = get_object<T>(L, 1);
    (obj->*fun)(get_param_cv<Args>(L, 2 + (int)Indices)...);
    return 0;
}

// Const, No return value, lua_State not passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, void (T::* fun)(Args...) const, std::index_sequence<Indices...>) {
    StackCheck _(L, 0);
    auto obj = get_object<T const>(L, 1);
    (obj->*fun)(get_param_cv<Args>(L, 2 + (int)Indices)...);
    return 0;
}

// 1 return value, lua_State not passed
template <class R, class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, R (T::* fun)(Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, TupleSize(Overload<R>{}));
    auto obj = get_object<T>(L, 1);
    auto retval = (obj->*fun)(get_param_cv<Args>(L, 2 + (int)Indices)...);
    PushReturnValue(L, retval);
    return TupleSize(Overload<R>{});
}

// Const, 1 return value, lua_State not passed
template <class R, class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, R (T::* fun)(Args...) const, std::index_sequence<Indices...>) {
    StackCheck _(L, TupleSize(Overload<R>{}));
    auto obj = get_object<T const>(L, 1);
    auto retval = (obj->*fun)(get_param_cv<Args>(L, 2 + (int)Indices)...);
    PushReturnValue(L, retval);
    return TupleSize(Overload<R>{});
}

// Index_sequence wrapper for CallMethodHelper(), lua_State not passed
template <class R, class T, class ...Args>
inline int CallMethod(lua_State* L, R (T::* fun)(Args...)) {
    return CallMethodHelper(L, fun, std::index_sequence_for<Args...>());
}

// Const, Index_sequence wrapper for CallMethodHelper(), lua_State not passed
template <class R, class T, class ...Args>
inline int CallMethod(lua_State* L, R (T::* fun)(Args...) const) {
    return CallMethodHelper(L, fun, std::index_sequence_for<Args...>());
}


// No return value, lua_State passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, void (* fun)(lua_State*, T*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, 0);
    auto obj = get_object<T>(L, 1);
    fun(L, obj, get_param_cv<Args>(L, 2 + (int)Indices)...);
    return 0;
}

// Const, No return value, lua_State passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, void (* fun)(lua_State*, T const*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, 0);
    auto obj = get_object<T const>(L, 1);
    fun(L, obj, get_param_cv<Args>(L, 2 + (int)Indices)...);
    return 0;
}

// Return values pushed by callee, lua_State passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, UserReturn (* fun)(lua_State*, T*, Args...), std::index_sequence<Indices...>) {
    auto obj = get_object<T>(L, 1);
    auto nret = fun(L, obj, get_param_cv<Args>(L, 2 + (int)Indices)...);
    return (int)nret;
}

// Const, Return values pushed by callee, lua_State passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, UserReturn (* fun)(lua_State*, T const*, Args...), std::index_sequence<Indices...>) {
    auto obj = get_object<T const>(L, 1);
    auto nret = fun(L, obj, get_param_cv<Args>(L, 2 + (int)Indices)...);
    return (int)nret;
}

// 1 return value, lua_State passed
template <class R, class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, R (* fun)(lua_State*, T*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, TupleSize(Overload<R>{}));
    auto obj = get_object<T>(L, 1);
    auto retval = fun(L, obj, get_param_cv<Args>(L, 2 + (int)Indices)...);
    PushReturnValue(L, retval);
    return TupleSize(Overload<R>{});
}

// Const, 1 return value, lua_State passed
template <class R, class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, R (* fun)(lua_State*, T const*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, TupleSize(Overload<R>{}));
    auto obj = get_object<T const>(L, 1);
    auto retval = fun(L, obj, get_param_cv<Args>(L, 2 + (int)Indices)...);
    PushReturnValue(L, retval);
    return TupleSize(Overload<R>{});
}

// Index_sequence wrapper for CallFunctionHelper(), lua_State passed
template <class R, class T, class ...Args>
inline int CallMethod(lua_State* L, R(* fun)(lua_State*, T*, Args...)) {
    return CallMethodHelper(L, fun, std::index_sequence_for<Args...>());
}

// Const, Index_sequence wrapper for CallFunctionHelper(), lua_State passed
template <class R, class T, class ...Args>
inline int CallMethod(lua_State* L, R(* fun)(lua_State*, T const*, Args...)) {
    return CallMethodHelper(L, fun, std::index_sequence_for<Args...>());
}

// No return value, lua_State not passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, void (* fun)(T*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, 0);
    auto obj = get_object<T>(L, 1);
    fun(obj, get_param_cv<Args>(L, 2 + (int)Indices)...);
    return 0;
}

// Const, No return value, lua_State not passed
template <class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, void (* fun)(T const*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, 0);
    auto obj = get_object<T const>(L, 1);
    fun(obj, get_param_cv<Args>(L, 2 + (int)Indices)...);
    return 0;
}

// 1 return value, lua_State not passed
template <class R, class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, R (* fun)(T*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, TupleSize(Overload<R>{}));
    auto obj = get_object<T>(L, 1);
    auto retval = fun(obj, get_param_cv<Args>(L, 2 + (int)Indices)...);
    PushReturnValue(L, retval);
    return TupleSize(Overload<R>{});
}

// Const, 1 return value, lua_State not passed
template <class R, class T, class ...Args, size_t ...Indices>
inline int CallMethodHelper(lua_State* L, R (* fun)(T const*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, TupleSize(Overload<R>{}));
    auto obj = get_object<T const>(L, 1);
    auto retval = fun(obj, get_param_cv<Args>(L, 2 + (int)Indices)...);
    PushReturnValue(L, retval);
    return TupleSize(Overload<R>{});
}

// Index_sequence wrapper for CallFunctionHelper(), lua_State not passed
template <class R, class T, class ...Args>
inline int CallMethod(lua_State* L, R (* fun)(T*, Args...)) {
    return CallMethodHelper(L, fun, std::index_sequence_for<Args...>());
}

// Const, Index_sequence wrapper for CallFunctionHelper(), lua_State not passed
template <class R, class T, class ...Args>
inline int CallMethod(lua_State* L, R (* fun)(T const*, Args...)) {
    return CallMethodHelper(L, fun, std::index_sequence_for<Args...>());
}


template <class R, class T>
inline void CallGetter(lua_State* L, T const* obj, R(T::* fun)() const) {
    static_assert(TupleSize(Overload<R>{}) == 1, "Can only push 1 value to stack in a getter.");
    StackCheck _(L, 1);
    PushReturnValue(L, (obj->*fun)());
}

template <class R, class T>
inline void CallGetter(lua_State* L, T const* obj, R const& (T::* fun)() const) {
    static_assert(TupleSize(Overload<R>{}) == 1, "Can only push 1 value to stack in a getter.");
    StackCheck _(L, 1);
    PushReturnValue(L, (obj->*fun)());
}

template <class R, class T>
inline void CallGetter(lua_State* L, T const* obj, R(* fun)(T const*)) {
    static_assert(TupleSize(Overload<R>{}) == 1, "Can only push 1 value to stack in a getter.");
    StackCheck _(L, 1);
    PushReturnValue(L, (*fun)(obj));
}

template <class R, class T>
inline void CallGetter(lua_State* L, T const* obj, R const& (* fun)(T const*)) {
    static_assert(TupleSize(Overload<R>{}) == 1, "Can only push 1 value to stack in a getter.");
    StackCheck _(L, 1);
    PushReturnValue(L, (*fun)(obj));
}

template <class R, class T>
inline void CallGetter(lua_State* L, T const* obj, R(* fun)(lua_State*, T const*)) {
    static_assert(TupleSize(Overload<R>{}) == 1, "Can only push 1 value to stack in a getter.");
    StackCheck _(L, 1);
    PushReturnValue(L, (*fun)(L, obj));
}

template <class R, class T>
inline void CallGetter(lua_State* L, T const* obj, R const& (* fun)(lua_State*, T const*)) {
    static_assert(TupleSize(Overload<R>{}) == 1, "Can only push 1 value to stack in a getter.");
    StackCheck _(L, 1);
    PushReturnValue(L, (*fun)(L, obj));
}

template <class T>
inline void CallGetter(lua_State* L, T const* obj, UserReturn (T::* fun)(lua_State* L) const) {
    StackCheck _(L, 1);
    (obj->*fun)(L);
}

template <class T>
inline void CallGetter(lua_State* L, T const* obj, UserReturn (* fun)(lua_State* L, T const*)) {
    StackCheck _(L, 1);
    (*fun)(L, obj);
}


template <class R, class T, class TEnum>
inline void CallFlagGetter(lua_State* L, T const* obj, R(T::* fun)(TEnum) const, TEnum flag) {
    static_assert(TupleSize(Overload<R>{}) == 1, "Can only push 1 value to stack in a getter.");
    StackCheck _(L, 1);
    auto retval = (obj->*fun)(flag);
    PushReturnValue(L, retval);
}


template <class T, class TArg>
inline void CallSetter(lua_State* L, T* obj, int index, void(T::* fun)(TArg)) {
    static_assert(TupleSize(Overload<TArg>{}) == 1, "Can only get 1 value from stack in a setter.");
    StackCheck _(L, 0);
    (obj->*fun)(get_param_cv<TArg>(L, index));
}


template <class T, class TArg>
inline void CallSetter(lua_State* L, T* obj, int index, void(T::* fun)(TArg const&)) {
    static_assert(TupleSize(Overload<TArg>{}) == 1, "Can only get 1 value from stack in a setter.");
    StackCheck _(L, 0);
    (obj->*fun)(get_param_cv<TArg>(L, index));
}


template <class T, class TArg>
inline void CallSetter(lua_State* L, T* obj, int index, void(T::* fun)(lua_State*, TArg)) {
    static_assert(TupleSize(Overload<TArg>{}) == 1, "Can only get 1 value from stack in a setter.");
    StackCheck _(L, 0);
    (obj->*fun)(L, get_param_cv<TArg>(L, index));
}


template <class T, class TArg>
inline void CallSetter(lua_State* L, T* obj, int index, void(T::* fun)(lua_State*, TArg const&)) {
    static_assert(TupleSize(Overload<TArg>{}) == 1, "Can only get 1 value from stack in a setter.");
    StackCheck _(L, 0);
    (obj->*fun)(L, get_param_cv<TArg>(L, index));
}


template <class T, class TArg, class TEnum>
inline void CallFlagSetter(lua_State* L, T* obj, int index, void(T::* fun)(TEnum, TArg), TEnum flag) {
    static_assert(TupleSize(Overload<TArg>{}) == 1, "Can only get 1 value from stack in a setter.");
    StackCheck _(L, 0);
    auto val = get_param_cv<TArg>(L, index);
    (obj->*fun)(flag, val);
}


// No return value, lua_State passed
template <class ...Args, size_t ...Indices>
inline int CallFunctionHelper(lua_State* L, void (* fun)(lua_State*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, 0);
    fun(L, get_param_cv<Args>(L, 1 + (int)Indices)...);
    return 0;
}

// Return values pushed by callee, lua_State passed
template <class ...Args, size_t ...Indices>
inline int CallFunctionHelper(lua_State* L, UserReturn (* fun)(lua_State*, Args...), std::index_sequence<Indices...>) {
    auto nret = fun(L, get_param_cv<Args>(L, 1 + (int)Indices)...);
    return (int)nret;
}

// 1 return value, lua_State passed
template <class R, class ...Args, size_t ...Indices>
inline int CallFunctionHelper(lua_State* L, R (* fun)(lua_State*, Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, TupleSize(Overload<R>{}));
    auto retval = fun(L, get_param_cv<Args>(L, 1 + (int)Indices)...);
    PushReturnValue(L, retval);
    return TupleSize(Overload<R>{});
}

// Index_sequence wrapper for CallFunctionHelper(), lua_State passed
template <class R, class ...Args>
inline int CallFunction(lua_State* L, R(* fun)(lua_State*, Args...)) {
    return CallFunctionHelper(L, fun, std::index_sequence_for<Args...>());
}

// No return value, lua_State not passed
template <class ...Args, size_t ...Indices>
inline int CallFunctionHelper(lua_State* L, void (* fun)(Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, 0);
    fun(get_param_cv<Args>(L, 1 + (int)Indices)...);
    return 0;
}

// 1 return value, lua_State not passed
template <class R, class ...Args, size_t ...Indices>
inline int CallFunctionHelper(lua_State* L, R (* fun)(Args...), std::index_sequence<Indices...>) {
    StackCheck _(L, TupleSize(Overload<R>{}));
    auto retval = fun(get_param_cv<Args>(L, 1 + (int)Indices)...);
    PushReturnValue(L, retval);
    return TupleSize(Overload<R>{});
}

// Index_sequence wrapper for CallFunctionHelper(), lua_State not passed
template <class R, class ...Args>
inline int CallFunction(lua_State* L, R (* fun)(Args...)) {
    return CallFunctionHelper(L, fun, std::index_sequence_for<Args...>());
}

template <class R, class ...Args>
inline std::optional<R> ProtectedCallFunction(lua_State* L, Ref const& fun, Args&&... args)
{
    ProtectedFunctionCaller<std::tuple<Args...>, R> caller{ fun, std::tuple<Args...>(std::forward<Args>(args)...) };
    if (caller.Call(L)) {
        return std::move(caller.Retval.Value);
    } else {
        return {};
    }
}

#define LuaWrapFunction(fun) [](lua_State* L) { return CallFunction(L, fun); }

END_NS()
