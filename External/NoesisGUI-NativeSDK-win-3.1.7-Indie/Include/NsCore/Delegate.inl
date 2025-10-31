////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

template<class Ret, class ...Args>
class DelegateImpl<Ret (Args...)>
{
public:
    typedef Noesis::Delegate<Ret (Args...)> Delegate;
    typedef typename Delegate::Invoker Invoker;

    enum Type
    {
        Type_Null,
        Type_FreeFunc,
        Type_Functor,
        Type_MemberFunc,
        Type_MultiDelegate
    };

    struct Base
    {
        virtual ~Base() {};
        virtual Type GetType() const = 0;
        virtual uint32_t Size() const = 0;
        virtual bool Equal(const Base* base) const = 0;
        virtual Ret Invoke(Args&&... args) const = 0;
        virtual Invoker* GetInvoker() const { return nullptr; }
        virtual void Copy(Base* dest, bool move) = 0;
        virtual void Add(const Delegate& d) = 0;
        virtual void Remove(const Delegate& d) = 0;
    };

    /// Implementation for null delegates
    class NullStub final: public Base
    {
    public:
        Type GetType() const override
        {
            return Type_Null;
        }

        uint32_t Size() const override
        {
            return 0;
        }

        bool Equal(const Base* base) const override
        {
            return GetType() == base->GetType();
        }

        Ret Invoke(Args&&...) const override
        {
            return Ret();
        }

        void Copy(Base* dest, bool) override
        {
            new(Placement(), dest) NullStub();
        }

        void Add(const Delegate& d) override
        {
            if (!d.Empty())
            {
                this->~NullStub();
                ((Delegate&)d).GetBase()->Copy(this, false);
            }
        }

        void Remove(const Delegate&) override {}
    };

    /// Base class implementation for single delegates
    class SingleDelegate: public Base
    {
    protected:
        uint32_t Size() const override
        {
            return 1;
        }

        void Add(const Delegate& d) override
        {
            if (!d.Empty())
            {
                Delegate self(this, Int2Type<0>());
                this->~SingleDelegate();

                MultiDelegate* x = new(Placement(), this) MultiDelegate();
                x->Add(self);
                x->Add(d);
            }
        }

        void Remove(const Delegate& d) override
        {
            if (this->Equal(d.GetBase()))
            {
                this->~SingleDelegate();
                new(Placement(), this) NullStub();
            }
        }
    };

    /// Implementation for free functions
    template<class Func> class FreeFuncStub final: public SingleDelegate
    {
    public:
        FreeFuncStub(Func func): mFunc(func) {}

        Type GetType() const override
        {
            return Type_FreeFunc;
        }

        bool Equal(const Base* base) const override
        {
            const FreeFuncStub* stub = (const FreeFuncStub*)base;
            return GetType() == base->GetType() && mFunc == stub->mFunc;
        }

        Ret Invoke(Args&&... args) const override
        {
            return mFunc(ForwardArg<Args>(args)...);
        }

        void Copy(Base* dest, bool) override
        {
            new(Placement(), dest) FreeFuncStub(*this);
        }

    private:
        Func mFunc;
    };

    /// Implementation for functors
    template<class F> class FunctorStub final: public SingleDelegate
    {
    public:
        FunctorStub(const F& f): mFunctor(f) {}

        Type GetType() const override
        {
            return Type_Functor;
        }

        bool Equal(const Base*) const override
        {
            return false;
        }

        Ret Invoke(Args&&... args) const override
        {
            return ((F*)&mFunctor)->operator()(ForwardArg<Args>(args)...);
        }

        void Copy(Base* dest, bool move) override
        {
            if (!move)
            {
                new(Placement(), dest) FunctorStub(*this);
            }
            else
            {
                new(Placement(), dest) FunctorStub(MoveArg(*this));
                this->~FunctorStub();
                new(Placement(), this) NullStub();
            }
        }

    private:
        F mFunctor;
    };

    /// Implementation for member functions
    template<class C, class Func> class MemberFuncStub final: public SingleDelegate
    {
    public:
        MemberFuncStub(C* obj, Func func): mObj(obj), mFunc(func) {}

        Type GetType() const override
        {
            return Type_MemberFunc;
        }

        bool Equal(const Base* base) const override
        {
            const MemberFuncStub* memFunc = (const MemberFuncStub*)base;
            return GetType() == base->GetType() && mObj == memFunc->mObj && mFunc == memFunc->mFunc;
        }

        Ret Invoke(Args&&... args) const override
        {
            return (mObj->*mFunc)(ForwardArg<Args>(args)...);
        }

        void Copy(Base* dest, bool) override
        {
            new(Placement(), dest) MemberFuncStub(*this);
        }

    protected:
        C* mObj;
        Func mFunc;
    };

    /// Implementation for MultiDelegates
    class MultiDelegate final: public Base
    {
    public:
        MultiDelegate(): mInvoker(MakePtr<Invoker>())
        {
        }

        MultiDelegate(const MultiDelegate& d): mInvoker(MakePtr<Invoker>())
        {
            mInvoker->v = d.mInvoker->v;
        }

        MultiDelegate(MultiDelegate&& d): mInvoker(MoveArg(d.mInvoker))
        {
        }

        Type GetType() const override
        {
            return Type_MultiDelegate;
        }

        uint32_t Size() const override
        {
            return mInvoker->v.Size() - mInvoker->pendingDeletes;
        }

        bool Equal(const Base* base) const override
        {
            if (GetType() == base->GetType())
            {
                const MultiDelegate* x = (const MultiDelegate*)base;
                return mInvoker->v == x->mInvoker->v;
            }

            return false;
        }

        struct InvokerGuard
        {
            InvokerGuard(Invoker* invoker): _invoker(invoker)
            {
                _invoker->BeginInvoke();
            }

            ~InvokerGuard()
            {
                _invoker->EndInvoke();
            }

            Invoker* _invoker;
        };

        Ret Invoke(Args&&... args) const override
        {
            InvokerGuard guard(mInvoker);
            uint32_t numDelegates = mInvoker->Size();

            if (numDelegates == 0)
            {
                return Ret();
            }
            else
            {
                for (uint32_t i = 0; i < numDelegates - 1; ++i)
                {
                    guard._invoker->Invoke(i, ForwardArg<Args>(args)...);
                }

                // last delegate is used to return a value
                return guard._invoker->Invoke(numDelegates - 1, ForwardArg<Args>(args)...);
            }
        }

        void Copy(Base* dest, bool move) override
        {
            if (!move)
            {
                new(Placement(), dest) MultiDelegate(*this);
            }
            else
            {
                new(Placement(), dest) MultiDelegate(MoveArg(*this));
                this->~MultiDelegate();
                new(Placement(), this) NullStub();
            }
        }

        void Add(const Delegate& d) override
        {
            if (!d.Empty())
            {
                mInvoker->v.PushBack(d);
            }
        }

        void Remove(const Delegate& d) override
        {
            if (!d.Empty())
            {
                auto& v = mInvoker->v;
                auto it = v.Find(d);

                if (it != v.End())
                {
                    it->Reset();
                    mInvoker->pendingDeletes++;
                    mInvoker->Compact();
                }
            }
        }

        Invoker* GetInvoker() const override
        {
            return mInvoker;
        }

    private:
        Ptr<Invoker> mInvoker;
    };
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Delegate<Ret (Args...)>::Delegate()
{
    InitEmpty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Delegate<Ret (Args...)>::Delegate(Ret (*func)(Args...))
{
    FromFreeFunc(func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
template<class F, class>
inline Delegate<Ret (Args...)>::Delegate(const F& f)
{
    FromFunctor(f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
template<class C> 
inline Delegate<Ret (Args...)>::Delegate(C* obj, Ret (C::*func)(Args...))
{
    FromMemberFunc(obj, func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
template<class C> 
inline Delegate<Ret (Args...)>::Delegate(const C* obj, Ret (C::*func)(Args...) const)
{
    FromMemberFunc(obj, func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
template<class C> 
inline Delegate<Ret (Args...)>::Delegate(const Ptr<C>& obj, Ret (C::*func)(Args...))
{
    FromMemberFunc(obj.GetPtr(), func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
template<class C> 
inline Delegate<Ret (Args...)>::Delegate(const Ptr<C>& obj, Ret (C::*func)(Args...) const)
{
    FromMemberFunc(obj.GetPtr(), func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
template<class C> 
inline Delegate<Ret (Args...)>::Delegate(const Ptr<const C>& obj, Ret (C::*func)(Args...) const)
{
    FromMemberFunc(obj.GetPtr(), func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Delegate<Ret (Args...)>::Delegate(const Delegate& d)
{
    ((Delegate&)d).GetBase()->Copy(GetBase(), false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Delegate<Ret (Args...)>::Delegate(Delegate&& d)
{
    d.GetBase()->Copy(GetBase(), true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Delegate<Ret (Args...)>::~Delegate()
{
    GetBase()->~Base();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Delegate<Ret (Args...)>& Delegate<Ret (Args...)>::operator=(const Delegate& d)
{
    if (this != &d)
    {
        GetBase()->~Base();
        ((Delegate&)d).GetBase()->Copy(GetBase(), false);
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Delegate<Ret (Args...)>& Delegate<Ret (Args...)>::operator=(Delegate&& d)
{
    if (this != &d)
    {
        GetBase()->~Base();
        d.GetBase()->Copy(GetBase(), true);
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline void Delegate<Ret (Args...)>::Reset()
{
    GetBase()->~Base();
    InitEmpty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline bool Delegate<Ret (Args...)>::Empty() const
{
    return GetBase()->Size() == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Delegate<Ret (Args...)>::operator bool() const
{
    return GetBase()->Size() != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline bool Delegate<Ret (Args...)>::operator==(const Delegate& d) const
{
    return GetBase()->Equal(d.GetBase());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline bool Delegate<Ret (Args...)>::operator!=(const Delegate& d) const
{
    return !(GetBase()->Equal(d.GetBase()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline void Delegate<Ret (Args...)>::Add(const Delegate& d)
{
    GetBase()->Add(d);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline void Delegate<Ret (Args...)>::operator+=(const Delegate& d)
{
    GetBase()->Add(d);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline void Delegate<Ret (Args...)>::Remove(const Delegate& d)
{
    GetBase()->Remove(d);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline void Delegate<Ret (Args...)>::operator-=(const Delegate& d)
{
    GetBase()->Remove(d);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline uint32_t Delegate<Ret (Args...)>::Size() const
{
    return GetBase()->Size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Ret Delegate<Ret (Args...)>::operator()(Args... args) const
{
    return GetBase()->Invoke(ForwardArg<Args>(args)...);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Ret Delegate<Ret (Args...)>::Invoke(Args... args) const
{
    return GetBase()->Invoke(ForwardArg<Args>(args)...);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline uint32_t Delegate<Ret (Args...)>::Invoker::Size() const
{
    return v.Size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline void Delegate<Ret (Args...)>::Invoker::BeginInvoke()
{
    AddReference();
    nestingCount++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Ret Delegate<Ret (Args...)>::Invoker::Invoke(uint32_t i, Args... args) const
{
    return (v[i])(ForwardArg<Args>(args)...);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline void Delegate<Ret (Args...)>::Invoker::EndInvoke()
{
    nestingCount--;
    Compact();
    Release();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline void Delegate<Ret (Args...)>::Invoker::Compact()
{
    if (nestingCount == 0 && pendingDeletes > v.Size() / 4)
    {
        v.EraseIf([](const Delegate& d) { return d.Empty(); });
        pendingDeletes = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline typename Delegate<Ret (Args...)>::Invoker* Delegate<Ret (Args...)>::GetInvoker() const
{
    return GetBase()->GetInvoker();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline void Delegate<Ret (Args...)>::FromFreeFunc(Ret (*func)(Args...))
{
    typedef typename Impl::template FreeFuncStub<Ret (*)(Args...)> T;
    static_assert(sizeof(T) <= sizeof(data), "Insufficient buffer size");
    new(Placement(), data) T(func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
template<class F>
inline void Delegate<Ret (Args...)>::FromFunctor(const F& f)
{
    typedef typename Impl::template FunctorStub<F> T;
    static_assert(sizeof(T) <= sizeof(data), "Insufficient buffer size");
    new(Placement(), data) T(f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
template<class C>
inline void Delegate<Ret (Args...)>::FromMemberFunc(C* obj, Ret (C::*func)(Args...))
{
    typedef typename Impl::template MemberFuncStub<C, Ret (C::*)(Args...)> T;
    static_assert(sizeof(T) <= sizeof(data), "Insufficient buffer size");
    new(Placement(), data) T(obj, func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
template<class C>
inline void Delegate<Ret (Args...)>::FromMemberFunc(const C* obj, Ret (C::*func)(Args...) const)
{
    typedef typename Impl::template MemberFuncStub<C, Ret (C::*)(Args...) const> T;
    static_assert(sizeof(T) <= sizeof(data), "Insufficient buffer size");
    new(Placement(), data) T(const_cast<C*>(obj), func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline void Delegate<Ret (Args...)>::InitEmpty()
{
    typedef typename Impl::NullStub T;
    static_assert(sizeof(T) <= sizeof(data), "Insufficient buffer size");
    new(Placement(), data) T();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline typename Delegate<Ret (Args...)>::Impl::Base* Delegate<Ret (Args...)>::GetBase()
{
    return (typename Impl::Base*)data;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline const typename Delegate<Ret (Args...)>::Impl::Base* Delegate<Ret (Args...)>::GetBase() const
{
    return (const typename Impl::Base*)data;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
inline Delegate<Ret (Args...)>::Delegate(typename Impl::Base* base, Int2Type<0>)
{
    base->Copy(GetBase(), false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class C, class Ret , class ...Args>
inline Delegate<Ret (Args...)> MakeDelegate(C* obj, Ret (C::*func)(Args...))
{
    return Delegate<Ret (Args...)>(obj, func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class C, class Ret, class ...Args>
inline Delegate<Ret (Args...)> MakeDelegate(const Ptr<C>& obj, Ret (C::*func)(Args...))
{
    return Delegate<Ret (Args...)>(obj, func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class C, class Ret, class ...Args>
inline Delegate<Ret (Args...)> MakeDelegate(const C* obj, Ret (C::*func)(Args...) const)
{
    return Delegate<Ret (Args...)>(obj, func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class C, class Ret, class ...Args>
inline Delegate<Ret (Args...)> MakeDelegate(const Ptr<C>& obj, Ret (C::*func)(Args...) const)
{
    return Delegate<Ret (Args...)>(obj, func);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class C, class Ret, class ...Args>
inline Delegate<Ret (Args...)> MakeDelegate(const Ptr<const C>& obj, Ret (C::*func)(Args...) const)
{
    return Delegate<Ret (Args...)>(obj, func);
}

}
