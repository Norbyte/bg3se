////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_PTR_H__
#define __CORE_PTR_H__


namespace Noesis
{

typedef decltype(nullptr) NullPtrT;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Automatically handles lifetime of BaseRefCounted objects by doing AddReference() and Release().
/// Ptr<> overloads certain operators in order to behave similar to a pointer but ensuring that
/// objects are deleted when they are no longer needed, to avoid memory leaks.
///
/// BaseRefCounted objects start with reference counter set to 1. When they are stored inside a
/// Ptr<> the counter is increased by 1 again. To avoid having to do a manual Release(), Ptr<>
/// supports being constructed from a reference. In this case the reference counter is not
/// incremented again:
///
///     Ptr<Cube> cube = *new Cube(50.0f);
///
/// Or you can use MakePtr for the same effect:
///
///     Ptr<Cube> cube = MakePtr<Cube>(50.0f);
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class Ptr
{
public:
    typedef T Type;

    /// Constructs empty smart pointer
    inline Ptr();
    inline Ptr(NullPtrT);

    /// Constructor from pointer, increasing reference counter
    inline explicit Ptr(T* ptr);

    /// Constructor from dereferenced pointer, without increasing reference counter.
    /// Very useful for assigning from new operator: Ptr<Cube> cube = *new Cube(50.0f);
    /// MakePtr is a more flexible alternative to this pattern.
    inline Ptr(T& ptr);

    /// Copy constructors
    inline Ptr(const Ptr& ptr);
    template<class S> inline Ptr(const Ptr<S>& ptr);

    /// Move constructors
    inline Ptr(Ptr&& ptr);
    template<class S> inline Ptr(Ptr<S>&& ptr);

    /// Destructor
    inline ~Ptr();

    /// Copy operators
    inline Ptr& operator=(const Ptr& ptr);
    template<class S> inline Ptr& operator=(const Ptr<S>& ptr);

    /// Move operators
    inline Ptr& operator=(Ptr&& ptr);
    template<class S> inline Ptr& operator=(Ptr<S>&& ptr);

    /// Copy from from dereferenced pointer. without increasing reference counter.
    /// Very useful for assigning from new operator: cube = *new Cube(50.0f);
    inline Ptr& operator=(T& ptr);

    /// Resets to null pointer
    inline void Reset();

    /// Resets to pointer, increasing reference counter
    inline void Reset(T* ptr);

    /// Clears the stored pointer without decrementing the reference counter
    inline T* GiveOwnership();

    /// Dereferences the stored pointer
    inline T* operator->() const;

    /// Returns the stored pointer 
    inline T* GetPtr() const;

    /// Automatic conversion to pointer
    inline operator T*() const;

private:
    T* mPtr;
};

/// Constructs an object of type T and wraps it in a Ptr<>
template<class T, class... Args> Ptr<T> MakePtr(Args&&... args);

}

#include <NsCore/Ptr.inl>

#endif
