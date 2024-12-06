#pragma once

BEGIN_SE()

template <class TFun, class TData>
class FunctionImpl
{
    static_assert(AlwaysFalse<TFun>, "FunctionImpl<T> expects a function template argument");
};

template <class TFun>
class Function
{
    static_assert(AlwaysFalse<TFun>, "Function<T> expects a function template argument");
};

template <class R, class... Args>
class Function<R (Args...)>
{
public:
    static constexpr unsigned UserDataSize = 4;

    Function(Function const& o)
    {
        call_ = o.call_;
        copy_ = o.copy_;
        move_ = o.move_;
        o.copy_(o, o, this);
    }

    Function(Function&& o)
    {
        call_ = o.call_;
        copy_ = o.copy_;
        move_ = o.move_;
        o.move_(o, std::move(o), this);
    }

    Function& operator =(Function const& o)
    {
        call_ = o.call_;
        copy_ = o.copy_;
        move_ = o.move_;
        o.copy_(o, o, this);
        return *this;
    }

    Function& operator =(Function&& o)
    {
        call_ = o.call_;
        copy_ = o.copy_;
        move_ = o.move_;
        o.move_(o, std::move(o), this);
        return *this;
    }

    ~Function()
    {
        move_(*this, std::move(*this), nullptr);
    }

    R operator ()(Args&&... args) const
    {
        if constexpr (std::is_same_v<R, void>) {
            return call_(*this, std::forward<Args>(args)...);
        } else {
            call_(*this, std::forward<Args>(args)...);
        }
    }

protected:
    friend FunctionImpl;

    Function() {}

private:
    using CallProc = R (Function const& self, Args... args);
    using CopyProc = Function* (Function const& self, Function const& src, Function* dst);
    using MoveProc = Function* (Function const& self, Function&& src, Function* dst);

    CallProc* call_;
    CopyProc* copy_;
    MoveProc* move_;
    std::uintptr_t data_[UserDataSize];
};


template <class R, class... Args>
class FunctionImpl<R (Args...), void> : public Function<R (Args...)>
{
private:
    using Base = Function<R(Args...)>;
    using UserCallProc = R (Args... args);

    struct Context
    {
        UserCallProc* Handler;
    };

    static_assert(sizeof(Context) <= sizeof(void*) * Base::UserDataSize);

public:
    FunctionImpl(UserCallProc* handler) : Function()
    {
        this->call_ = [](Base const& self, Args... args) -> R {
            if constexpr (std::is_same_v<R, void>) {
                static_cast<FunctionImpl const&>(self)(std::forward<Args>(args)...);
            } else {
                return static_cast<FunctionImpl const&>(self)(std::forward<Args>(args)...);
            }
        };
        this->copy_ = [](Base const& self, Base const& src, Base* dst) -> Base* {
            *static_cast<FunctionImpl*>(dst)->Data() = *static_cast<FunctionImpl const&>(src).Data();
            return dst;
        };
        this->move_ = [](Base const& self, Base&& src, Base* dst) -> Base* {
            if (dst == nullptr) {
                static_cast<FunctionImpl&>(src).Data()->~Context();
                return nullptr;
            } else {
                *static_cast<FunctionImpl*>(dst)->Data() = std::move(*static_cast<FunctionImpl const&>(src).Data());
                return dst;
            }
        };
        *Data() = Context{
            .Handler = handler
        };
    }

private:
    R operator ()(Args&&... args) const
    {
        if constexpr (std::is_same_v<R, void>) {
            return (Data()->Handler)(std::forward<Args>(args)...);
        } else {
            (Data()->Handler)(std::forward<Args>(args)...);
        }
    }

    inline Context* Data()
    {
        return reinterpret_cast<Context*>(&this->data_[0]);
    }

    inline Context const* Data() const
    {
        return reinterpret_cast<Context const*>(&this->data_[0]);
    }
};


template <class R, class T, class... Args>
class FunctionImpl<R (T::*)(Args...), void> : public Function<R (Args...)>
{
private:
    using Base = Function<R (Args...)>;
    using UserCallProc = R (T::*)(Args... args);

    struct Context
    {
        T* This;
        UserCallProc Handler;
    };

    static_assert(sizeof(Context) <= sizeof(void*) * Base::UserDataSize);

public:
    FunctionImpl(T* this_, UserCallProc handler)
    {
        this->call_ = [](Base const& self, Args... args) -> R {
            if constexpr (std::is_same_v<R, void>) {
                static_cast<FunctionImpl const&>(self)(std::forward<Args>(args)...);
            } else {
                return static_cast<FunctionImpl const&>(self)(std::forward<Args>(args)...);
            }
        };
        this->copy_ = [](Base const& self, Base const& src, Base* dst) -> Base* {
            *static_cast<FunctionImpl*>(dst)->Data() = *static_cast<FunctionImpl const&>(src).Data();
            return dst;
        };
        this->move_ = [](Base const& self, Base&& src, Base* dst) -> Base* {
            if (dst == nullptr) {
                static_cast<FunctionImpl&>(src).Data()->~Context();
                return nullptr;
            } else {
                *static_cast<FunctionImpl*>(dst)->Data() = std::move(*static_cast<FunctionImpl const&>(src).Data());
                return dst;
            }
        };
        *Data() = Context{
            .This = this_,
            .Handler = handler
        };
    }

private:
    R operator ()(Args&&... args) const
    {
        auto this_ = Data()->This;
        auto proc = Data()->Handler;
        if constexpr (std::is_same_v<R, void>) {
            return (this_->*proc)(std::forward<Args>(args)...);
        } else {
            (this_->*proc)(std::forward<Args>(args)...);
        }
    }

    inline Context* Data()
    {
        return reinterpret_cast<Context*>(&this->data_[0]);
    }

    inline Context const* Data() const
    {
        return reinterpret_cast<Context const*>(&this->data_[0]);
    }
};


template <class TData, class R, class T, class... Args>
class FunctionImpl<R (T::*)(TData, Args...), TData> : public Function<R (Args...)>
{
private:
    using Base = Function<R (Args...)>;
    using UserCallProc = R (T::*)(TData, Args...);

    struct Context
    {
        T* This;
        UserCallProc Handler;
        TData Data;
    };

    static_assert(sizeof(Context) <= sizeof(void*) * Base::UserDataSize);

public:
    FunctionImpl(T* this_, UserCallProc handler, TData const& data)
    {
        this->call_ = [](Base const& self, Args... args) -> R {
            if constexpr (std::is_same_v<R, void>) {
                static_cast<FunctionImpl const&>(self)(std::forward<Args>(args)...);
            } else {
                return static_cast<FunctionImpl const&>(self)(std::forward<Args>(args)...);
            }
        };
        this->copy_ = [](Base const& self, Base const& src, Base* dst) -> Base* {
            *static_cast<FunctionImpl*>(dst)->Data() = *static_cast<FunctionImpl const&>(src).Data();
            return dst;
        };
        this->move_ = [](Base const& self, Base&& src, Base* dst) -> Base* {
            if (dst == nullptr) {
                static_cast<FunctionImpl&>(src).Data()->~Context();
                return nullptr;
            } else {
                *static_cast<FunctionImpl*>(dst)->Data() = std::move(*static_cast<FunctionImpl const&>(src).Data());
                return dst;
            }
        };
        *Data() = Context{
            .This = this_,
            .Handler = handler,
            .Data = data
        };
    }

private:
    R operator ()(Args&&... args) const
    {
        auto this_ = Data()->This;
        auto proc = Data()->Handler;
        auto& data = Data()->Data;
        if constexpr (std::is_same_v<R, void>) {
            return (this_->*proc)(data, std::forward<Args>(args)...);
        } else {
            (this_->*proc)(data, std::forward<Args>(args)...);
        }
    }

    inline Context* Data()
    {
        return reinterpret_cast<Context*>(&this->data_[0]);
    }

    inline Context const* Data() const
    {
        return reinterpret_cast<Context const*>(&this->data_[0]);
    }
};


template <class... Args>
struct Signal
{
    using Function = Function<void (Args...)>;

    struct Connection
    {
        Connection()
        {
            pHandler = &Handler;
        }

        Connection(Connection const& o)
            : pHandler(&Handler),
            Handler(o.Handler),
            RegistrantIndex(o.RegistrantIndex)
        {}

        Connection(Connection&& o)
            : pHandler(&Handler),
            Handler(std::move(o.Handler)),
            RegistrantIndex(o.RegistrantIndex)
        {}

        Connection(Function const& handler, uint64_t index)
            : pHandler(&Handler),
            Handler(handler),
            RegistrantIndex(index)
        {}

        Connection& operator =(Connection && o)
        {
            pHandler = &Handler;
            Handler = std::move(o.Handler);
            RegistrantIndex = o.RegistrantIndex;
            return *this;
        }

        Function* pHandler{ nullptr };
        Function Handler;
        uint64_t RegistrantIndex{ 0 };
    };

    uint64_t NextRegistrantId;
    Array<Connection> Connections;

    uint64_t Add(Function const& handler)
    {
        auto index = NextRegistrantId++;
        Connections.push_back(Connection{ handler, index });
        return index;
    }

    bool Remove(uint64_t registrantIndex)
    {
        for (uint32_t i = 0; i < Connections.size(); i++) {
            if (Connections[i].RegistrantIndex == registrantIndex) {
                Connections.ordered_remove_at(i);
                return true;
            }
        }

        return false;
    }

    void Invoke(Args... args)
    {
        for (auto const& conn : Connections) {
            conn.Handler(std::forward<Args>(args)...);
        }
    }
};


template <class T>
Function<T> MakeFunction();

template <class R, class... Args>
Function<R(Args...)> MakeFunction(R (* fun)(Args...))
{
    return FunctionImpl<R (Args...), void>(fun);
}

template <class R, class T, class... Args>
Function<R(Args...)> MakeFunction(R (T::* fun)(Args...), T* self)
{
    return FunctionImpl<R (T::*)(Args...), void>(self, fun);
}

template <class R, class T, class TData, class... Args>
Function<R (Args...)> MakeFunction(R (T::* fun)(TData, Args...), T* self, TData const& data)
{
    return FunctionImpl<R (T::*)(TData, Args...), TData>(self, fun, data);
}

END_SE()
