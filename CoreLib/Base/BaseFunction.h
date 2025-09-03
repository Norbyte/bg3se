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

template <class TFun>
class FunctionStorage
{
    static_assert(AlwaysFalse<TFun>, "Function<T> expects a function template argument");
};

template <class R, class... Args>
struct FunctionStorage<R (Args...)>
{
    static constexpr unsigned UserDataSize = 4;

    FunctionStorage()
        : call_(nullptr),
        copy_(nullptr),
        move_(nullptr)
    {}

    FunctionStorage(FunctionStorage const& o)
        : call_(o.call_),
        copy_(o.copy_),
        move_(o.move_)
    {
        o.copy_(o, o, this);
    }

    FunctionStorage(FunctionStorage&& o)
        : call_(o.call_),
        copy_(o.copy_),
        move_(o.move_)
    {
        o.move_(o, std::move(o), this);
    }

    FunctionStorage& operator =(FunctionStorage const* o)
    {
        if (o) {
            call_ = o->call_;
            copy_ = o->copy_;
            move_ = o->move_;
            o->copy_(*o, *o, this);
        }
        return *this;
    }

    FunctionStorage& operator =(FunctionStorage const& o)
    {
        call_ = o.call_;
        copy_ = o.copy_;
        move_ = o.move_;
        o.copy_(o, o, this);
        return *this;
    }

    FunctionStorage& operator =(FunctionStorage&& o) noexcept
    {
        call_ = o.call_;
        copy_ = o.copy_;
        move_ = o.move_;
        o.move_(o, std::move(o), this);
        return *this;
    }

    ~FunctionStorage()
    {}

    R operator ()(Args&&... args) const
    {
        if constexpr (std::is_same_v<R, void>) {
            call_(*this, std::forward<Args>(args)...);
        } else {
            return call_(*this, std::forward<Args>(args)...);
        }
    }

    using CallProc = R (FunctionStorage const& self, Args... args);
    using CopyProc = FunctionStorage* (FunctionStorage const& self, FunctionStorage const& src, FunctionStorage* dst);
    using MoveProc = FunctionStorage* (FunctionStorage const& self, FunctionStorage&& src, FunctionStorage* dst);

    CallProc* call_;
    CopyProc* copy_;
    MoveProc* move_;
    std::uintptr_t data_[UserDataSize];

    template <class T>
    inline T* data()
    {
        static_assert(sizeof(T) <= sizeof(data_));
        return reinterpret_cast<T*>(&this->data_[0]);
    }

    template <class T>
    inline T const* data() const
    {
        static_assert(sizeof(T) <= sizeof(data_));
        return reinterpret_cast<T const*>(&this->data_[0]);
    }
};

template <class R, class... Args>
class Function<R (Args...)>
{
public:
    static constexpr unsigned UserDataSize = 4;

    Function()
        : pStorage_(nullptr)
    {}

    Function(Function const& o)
        : pStorage_(o ? &storage_ : nullptr)
    {
        if (o) {
            storage_ = o.storage_;
        }
    }

    Function(FunctionStorage<R(Args...)> const& o)
        : pStorage_(&storage_),
        storage_(o)
    {}

    Function(Function&& o) noexcept
        : pStorage_(o ? &storage_ : nullptr)
    {
        if (o) {
            storage_ = std::move(o.storage_);
        }
    }

    ~Function()
    {
        if (pStorage_) {
            pStorage_->move_(storage_, std::move(storage_), nullptr);
        }
    }

    Function& operator =(Function const& o)
    {
        pStorage_ = o ? &storage_ : nullptr;
        if (o) {
            storage_ = o.storage_;
        }
        return *this;
    }

    Function& operator =(Function&& o) noexcept
    {
        pStorage_ = o ? &storage_ : nullptr;
        if (o) {
            storage_ = std::move(o.storage_);
        }
        return *this;
    }

    inline operator bool() const
    {
        return pStorage_ != nullptr;
    }

    inline R operator ()(Args&&... args) const
    {
        if constexpr (std::is_same_v<R, void>) {
            storage_(std::forward<Args>(args)...);
        } else {
            return storage_(std::forward<Args>(args)...);
        }
    }

protected:
    friend FunctionImpl;

private:
    FunctionStorage<R(Args...)>* pStorage_;
    FunctionStorage<R(Args...)> storage_;
};


template <class R, class... Args>
struct FunctionImpl<R (Args...), void>
{
    using Fn = Function<R(Args...)>;
    using Storage = FunctionStorage<R(Args...)>;
    using UserCallProc = R (Args... args);

    struct Context
    {
        UserCallProc* Handler;
    };

    static Fn Make(UserCallProc* handler)
    {
        Storage fn;
        fn.call_ = [] (Storage const& self, Args... args) -> R {
            auto handler = self.data<Context>()->Handler;
            if constexpr (std::is_same_v<R, void>) {
                handler(std::forward<Args>(args)...);
            } else {
                return handler(std::forward<Args>(args)...);
            }
        };
        fn.copy_ = [] (Storage const& self, Storage const& src, Storage* dst) -> Storage* {
            dst->call_ = src.call_;
            dst->copy_ = src.copy_;
            dst->move_ = src.move_;
            *dst->data<Context>() = *src.data<Context>();
            return dst;
        };
        fn.move_ = [] (Storage const& self, Storage&& src, Storage* dst) -> Storage* {
            if (dst == nullptr) {
                src.data<Context>()->~Context();
                return nullptr;
            } else {
                dst->call_ = src.call_;
                dst->copy_ = src.copy_;
                dst->move_ = src.move_;
                *dst->data<Context>() = std::move(*src.data<Context>());
                return dst;
            }
        };
        new (fn.data<Context>()) Context{
            .Handler = handler
        };
        return Fn(fn);
    }
};


template <class R, class T, class... Args>
struct FunctionImpl<R (T::*)(Args...), void>
{
    using Fn = Function<R(Args...)>;
    using Storage = FunctionStorage<R(Args...)>;
    using UserCallProc = R (T::*)(Args... args);

    struct Context
    {
        T* This;
        UserCallProc Handler;
    };

    static Fn Make(T* this_, UserCallProc* handler)
    {
        Storage fn;
        fn.call_ = [](Storage const& self, Args... args) -> R {
            auto this_ = self.data<Context>()->This;
            auto handler = self.data<Context>()->Handler;
            if constexpr (std::is_same_v<R, void>) {
                (this_->*handler)(std::forward<Args>(args)...);
            } else {
                return (this_->*handler)(std::forward<Args>(args)...);
            }
        };
        fn.copy_ = [](Storage const& self, Storage const& src, Storage* dst) -> Storage* {
            dst->call_ = src.call_;
            dst->copy_ = src.copy_;
            dst->move_ = src.move_;
            *dst->data<Context>() = *src.data<Context>();
            return dst;
        };
        fn.move_ = [](Storage const& self, Storage&& src, Storage* dst) -> Storage* {
            if (dst == nullptr) {
                src.data<Context>()->~Context();
                return nullptr;
            } else {
                dst->call_ = src.call_;
                dst->copy_ = src.copy_;
                dst->move_ = src.move_;
                *dst->data<Context>() = std::move(*src.data<Context>());
                return dst;
            }
        };
        new (fn.data<Context>()) Context{
            .This = this_,
            .Handler = handler
        };
        return fn;
    }
};


template <class TData, class R, class... Args>
struct FunctionImpl<R (TData const&, Args...), TData>
{
    using Fn = Function<R(Args...)>;
    using Storage = FunctionStorage<R(Args...)>;
    using UserCallProc = R (TData const&, Args...);

    struct Context
    {
        UserCallProc* Handler;
        TData Data;
    };

    static Fn Make(UserCallProc* handler, TData const& data)
    {
        Storage fn;
        fn.call_ = [] (Storage const& self, Args... args) -> R {
            auto proc = self.data<Context>()->Handler;
            auto& data = self.data<Context>()->Data;
            if constexpr (std::is_same_v<R, void>) {
                proc(data, std::forward<Args>(args)...);
            } else {
                return proc(data, std::forward<Args>(args)...);
            }
        };
        fn.copy_ = [] (Storage const& self, Storage const& src, Storage* dst) -> Storage* {
            dst->call_ = src.call_;
            dst->copy_ = src.copy_;
            dst->move_ = src.move_;
            *dst->data<Context>() = *src.data<Context>();
            return dst;
        };
        fn.move_ = [] (Storage const& self, Storage&& src, Storage* dst) -> Storage* {
            if (dst == nullptr) {
                src.data<Context>()->~Context();
                return nullptr;
            } else {
                dst->call_ = src.call_;
                dst->copy_ = src.copy_;
                dst->move_ = src.move_;
                *dst->data<Context>() = std::move(*src.data<Context>());
                return dst;
            }
        };
        new (fn.data<Context>()) Context{
            .Handler = handler,
            .Data = data
        };
        return fn;
    }
};


template <class TData, class R, class T, class... Args>
struct FunctionImpl<R (T::*)(TData const&, Args...), TData>
{
    using Fn = Function<R(Args...)>;
    using Storage = FunctionStorage<R(Args...)>;
    using UserCallProc = R (T::*)(TData const&, Args...);

    struct Context
    {
        T* This;
        UserCallProc Handler;
        TData Data;
    };

    static Fn Make(T* this_, UserCallProc handler, TData const& data)
    {
        Storage fn;
        fn.call_ = [](Storage const& self, Args... args) -> R {
            auto this_ = self.data<Context>()->This;
            auto proc = self.data<Context>()->Handler;
            auto& data = self.data<Context>()->Data;
            if constexpr (std::is_same_v<R, void>) {
                (this_->*proc)(data, std::forward<Args>(args)...);
            } else {
                return (this_->*proc)(data, std::forward<Args>(args)...);
            }
        };
        fn.copy_ = [](Storage const& self, Storage const& src, Storage* dst) -> Storage* {
            dst->call_ = src.call_;
            dst->copy_ = src.copy_;
            dst->move_ = src.move_;
            *dst->data<Context>() = *src.data<Context>();
            return dst;
        };
        fn.move_ = [](Storage const& self, Storage&& src, Storage* dst) -> Storage* {
            if (dst == nullptr) {
                src.data<Context>()->~Context();
                return nullptr;
            } else {
                dst->call_ = src.call_;
                dst->copy_ = src.copy_;
                dst->move_ = src.move_;
                *dst->data<Context>() = std::move(*src.data<Context>());
                return dst;
            }
        };
        new (fn.data<Context>()) Context{
            .This = this_,
            .Handler = handler,
            .Data = data
        };
        return fn;
    }
};


template <class... Args>
struct Signal
{
    using Function = Function<void (Args...)>;

    struct Connection
    {
        Connection()
        {}

        Connection(Connection const& o)
            : Handler(o.Handler),
            RegistrantIndex(o.RegistrantIndex)
        {}

        Connection(Connection&& o) noexcept
            : Handler(std::move(o.Handler)),
            RegistrantIndex(o.RegistrantIndex)
        {}

        Connection(Function const& handler, uint64_t index)
            : Handler(handler),
            RegistrantIndex(index)
        {}

        Connection& operator =(Connection && o) noexcept
        {
            Handler = std::move(o.Handler);
            RegistrantIndex = o.RegistrantIndex;
            return *this;
        }

        Function Handler;
        uint64_t RegistrantIndex{ 0 };
    };

    uint64_t NextRegistrantId{ 1 };
    Array<Connection> Connections;

    uint64_t Add(Function const& handler)
    {
        auto index = NextRegistrantId++;
        Connections.push_back(Connection{ handler, index });
        return index;
    }

    uint64_t Add(Function&& handler)
    {
        auto index = NextRegistrantId++;
        Connections.push_back(Connection{ std::move(handler), index });
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


template <class... Args>
struct SignalSubscriber
{
    Signal<Args...>* Sig;
    uint64_t RegistrantId;
    Signal<Args...>::Function Handler;
};


template <class T>
Function<T> MakeFunction();

template <class R, class... Args>
Function<R(Args...)> MakeFunction(R (* fun)(Args...))
{
    return FunctionImpl<R (Args...), void>::Make(fun);
}

template <class R, class T, class... Args>
Function<R(Args...)> MakeFunction(R (T::* fun)(Args...), T* self)
{
    return FunctionImpl<R (T::*)(Args...), void>::Make(self, fun);
}

template <class R, class TData, class... Args>
Function<R (Args...)> MakeFunction(R (* fun)(TData const&, Args...), TData const& data)
{
    return FunctionImpl<R (TData const&, Args...), TData>::Make(fun, data);
}

template <class R, class T, class TData, class... Args>
Function<R (Args...)> MakeFunction(R (T::* fun)(TData const&, Args...), T* self, TData const& data)
{
    return FunctionImpl<R (T::*)(TData const&, Args...), TData>::Make(self, fun, data);
}

END_SE()
