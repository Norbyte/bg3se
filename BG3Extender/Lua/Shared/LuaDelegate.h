#pragma once

BEGIN_NS(lua)

template <class TArgs, class TReturn>
struct ProtectedFunctionCaller;

template <class T>
class LuaDelegate;

template<class TRet, class ...TArgs>
class LuaDelegate<TRet(TArgs...)>
{
public:
    inline LuaDelegate() {}
    inline LuaDelegate(lua_State* L, int index)
        : ref_(L, index)
    {}

    inline LuaDelegate(lua_State* L, Ref const& local)
        : ref_(L, local)
    {}

    inline ~LuaDelegate() {}

    LuaDelegate(LuaDelegate const&) = delete;

    inline LuaDelegate(LuaDelegate && o)
    {
        ref_ = std::move(o.ref_);
    }

    LuaDelegate& operator = (LuaDelegate const&) = delete;
    
    inline LuaDelegate& operator = (LuaDelegate && o)
    {
        ref_ = std::move(o.ref_);
        return *this;
    }

    explicit inline operator bool() const
    {
        return (bool)ref_;
    }

    inline void Push() const
    {
        ref_.Push();
    }

    TRet Call(TArgs... args)
    {
        auto L = ref_.GetState();
        StackCheck _(L);

        ref_.Push();
        Ref func(L, lua_absindex(L, -1));

        ProtectedFunctionCaller<std::tuple<TArgs...>, TRet> caller{ func, std::tuple(args...) };

		if constexpr (std::is_same_v<TRet, void>) {
			caller.Call(L);
			lua_pop(L, 1);
		} else {
			auto rval = caller.Call(L);
			lua_pop(L, 1);
			return rval;
		}
    }

private:
    RegistryEntry ref_;
};

END_NS()
