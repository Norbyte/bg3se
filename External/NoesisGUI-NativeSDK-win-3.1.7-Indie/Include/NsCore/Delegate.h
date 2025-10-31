////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_DELEGATE_H__
#define __CORE_DELEGATE_H__


#include <NsCore/Noesis.h>
#include <NsCore/CompilerTools.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Vector.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

template<class T> class Delegate;
template<class T> class DelegateImpl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A generic implementation for callbacks, better than std::function:
///
/// * Faster compilation times
/// * Easier to debug
/// * Fixed size (4 pointers), no memory allocations
/// * Support for multicast (like in C#) using += (this allocates memory)
/// * Delegates can be compared for equality
///
///   Delegate<uint32_t (uint32_t)> d = [](uint32_t x) { return x + 1; };
///
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Ret, class ...Args>
class Delegate<Ret (Args...)>
{
public:
    /// Constructor for empty delegates
    Delegate();

    /// Constructor from free function
    Delegate(Ret (*func)(Args...));

    /// Constructor from functor (lambda)
    template<class F, class = EnableIf<IsClass<F>::Result>>
    Delegate(const F& f);

    /// Constructor from class member function
    template<class C> Delegate(C* obj, Ret (C::*func)(Args...));

    /// Constructor from class member function (const)
    template<class C> Delegate(const C* obj, Ret (C::*func)(Args...) const);

    /// Constructor from ref-counted class member function
    template<class C> Delegate(const Ptr<C>& obj, Ret (C::*func)(Args...));

    /// Constructor from ref-counted class member function (const)
    template<class C> Delegate(const Ptr<C>& obj, Ret (C::*func)(Args...) const);

    /// Constructor from ref-counted class member function (const)
    template<class C> Delegate(const Ptr<const C>& obj, Ret (C::*func)(Args...) const);

    /// Copy constructor
    Delegate(const Delegate& d);

    /// Move constructor
    Delegate(Delegate&& d);

    /// Destructor
    ~Delegate();

    /// Copy operator
    Delegate& operator=(const Delegate& d);

    /// Move operator
    Delegate& operator=(Delegate&& d);

    /// Reset to empty
    void Reset();

    /// Check if delegate is empty
    bool Empty() const;

    /// Boolean conversion
    operator bool() const;

    /// Equality
    bool operator==(const Delegate& d) const;

    /// Non-equality
    bool operator!=(const Delegate& d) const;

    /// Add delegate
    void Add(const Delegate& d);
    void operator+=(const Delegate& d);

    /// Remove delegate
    void Remove(const Delegate& d);
    void operator-=(const Delegate& d);

    /// Numbers of contained delegates
    uint32_t Size() const;

    /// Delegate invocation. For multidelegates, returned value corresponds to last invocation
    Ret operator()(Args... args) const;
    Ret Invoke(Args... args) const;

    /// Delegate invoker for manual invocations of multidelegates items
    struct Invoker: public BaseComponent
    {
        uint32_t Size() const;

        void BeginInvoke();
        Ret Invoke(uint32_t i, Args... args) const;
        void EndInvoke();

        void Compact();

        uint16_t nestingCount = 0;
        uint16_t pendingDeletes = 0;
        Vector<Delegate, 2> v;
    };

    /// Returns the invoker of the delegate, or null for simple delegates
    Invoker* GetInvoker() const;

private:
    void FromFreeFunc(Ret (*func)(Args...));
    template<class F> void FromFunctor(const F& f);
    template<class C> void FromMemberFunc(C* obj, Ret (C::*func)(Args...));
    template<class C> void FromMemberFunc(const C* obj, Ret (C::*func)(Args...) const);

    void InitEmpty();

    typedef DelegateImpl<Ret (Args...)> Impl;
    friend Impl;

    typename Impl::Base* GetBase();
    const typename Impl::Base* GetBase() const;

    Delegate(typename Impl::Base* base, Int2Type<0>);

private:
    alignas(void*) uint8_t data[4 * sizeof(void*)];
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper functions to deduce automatically the type when creating a delegate
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class C, class Ret , class ...Args>
Delegate<Ret (Args...)> MakeDelegate(C* obj, Ret (C::*func)(Args...));

template<class C, class Ret, class ...Args>
Delegate<Ret (Args...)> MakeDelegate(const Ptr<C>& obj, Ret (C::*func)(Args...));

template<class C, class Ret, class ...Args>
Delegate<Ret (Args...)> MakeDelegate(const C* obj, Ret (C::*func)(Args...) const);

template<class C, class Ret, class ...Args>
Delegate<Ret (Args...)> MakeDelegate(const Ptr<C>& obj, Ret (C::*func)(Args...) const);

template<class C, class Ret, class ...Args>
Delegate<Ret (Args...)> MakeDelegate(const Ptr<const C>& obj, Ret (C::*func)(Args...) const);

}

#include <NsCore/Delegate.inl>

#endif
