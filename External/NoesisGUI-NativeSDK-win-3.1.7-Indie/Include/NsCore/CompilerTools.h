////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_COMPILERTOOLS_H__
#define __CORE_COMPILERTOOLS_H__


#include <NsCore/Noesis.h>


namespace Noesis
{

class Symbol;
class BaseRefCounted;
template<class T> class Ptr;
NS_INTERFACE Interface;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Converts each integral constant into a unique type
////////////////////////////////////////////////////////////////////////////////////////////////////
template<int N> struct Int2Type
{
    enum { Result = N };
};

typedef Int2Type<false> FalseType;
typedef Int2Type<true> TrueType;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// If-Then-Else statements
////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool Cond, class Then, class Else> struct If_;

template<class Then, class Else> struct If_<true, Then, Else>
{
    typedef Then Result;
};

template<class Then, class Else> struct If_<false, Then, Else>
{
    typedef Else Result;
};

template<bool Cond, class Then, class Else> using If = typename If_<Cond, Then, Else>::Result;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is const-qualified 
///
///   Example:
///     IsConst<MyClass>::Result       -> false
///     IsConst<const MyClass>::Result -> true
///     RemoveConst<const MyClass>     -> MyClass
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsConst
{
    template<class U> struct ConstTraits
    {
        enum { Result = false };
        typedef U Type;
    };
    template<class U> struct ConstTraits<const U>
    {
        enum { Result = true };
        typedef U Type;
    };

public:
    enum { Result = ConstTraits<T>::Result };
    typedef typename ConstTraits<T>::Type NonConstType;
};

/// Helper variable template
template<class T> using RemoveConst = typename IsConst<T>::NonConstType;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is volatile-qualified 
///
///   Example:
///     IsVolatile<MyClass>::Result          -> false
///     IsVolatile<volatile MyClass>::Result -> true
///     RemoveVolatile<volatile MyClass>     -> MyClass
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsVolatile
{
    template<class U> struct VolatileTraits
    {
        enum { Result = false };
        typedef U Type;
    };
    template<class U> struct VolatileTraits<volatile U>
    {
        enum { Result = true };
        typedef U Type;
    };

public:
    enum { Result = VolatileTraits<T>::Result };
    typedef typename VolatileTraits<T>::Type NonVolatileType;
};

/// Helper variable templates
template<class T> using RemoveVolatile = typename IsVolatile<T>::NonVolatileType;
template<class T> using RemoveCV = RemoveVolatile<RemoveConst<T>>;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Returns true if Base is a base class of Derived, or if both types are the same
///
///   Example:
///     class Father { };
///     class Child: public Father { };
///     IsDerived<Child, Father>::Result -> true
///     IsDerived<Father, Child>::Result -> false
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Derived, class Base> struct IsDerived
{
    enum { Result = __is_base_of(Base, Derived) };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if two types are the same
///
///   Example:
///     class Father { };
///     class Child: public Father { };
///     IsSame<Child, Father>::Result -> false
///     IsSame<Child, Child>::Result  -> true
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class U> struct IsSame
{
    enum { Result = false };
};

template<class T> struct IsSame<T, T>
{
    enum { Result = true };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is a pointer type
///
///   Example:
///     IsPointer<MyClass>::Result  -> false
///     IsPointer<MyClass*>::Result -> true
///     RemovePointer<MyClass*>     -> MyClass
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsPointer
{
    template<class U> struct PointerTraits
    {
        enum { Result = false };
        typedef U Type;
    };
    template<class U> struct PointerTraits<U*>
    {
        enum { Result = true };
        typedef U Type;
    };

public:
    enum { Result = PointerTraits<RemoveCV<T>>::Result };
    typedef typename PointerTraits<RemoveCV<T>>::Type PointedType;
};

/// Helper variable template
template<class T> using RemovePointer = typename IsPointer<T>::PointedType;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is either lvalue reference or rvalue reference
///
///   Example:
///     IsReference<MyClass>::Result  -> false
///     IsReference<MyClass&>::Result -> true
///     RemoveReference<MyClass&>     -> MyClass
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsReference
{
    template<class U> struct ReferenceTraits
    {
        enum { Result = false };
        typedef U Type;
    };
    template<class U> struct ReferenceTraits<U&>
    {
        enum { Result = true };
        typedef U Type;
    };

public:
    enum { Result = ReferenceTraits<T>::Result };
    typedef typename ReferenceTraits<T>::Type NonReferenceType;
};

/// Helper variable template
template<class T> using RemoveReference = typename IsReference<T>::NonReferenceType;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is ref counted (derived from Interface or BaseRefCounted).
/// If Result is true, the type T can be stored inside a Ptr<>
///
///   Example:
///     IsRefCounted<int>::Result            -> false
///     IsRefCounted<ICommand>::Result       -> true
///     IsRefCounted<const ICommand>::Result -> true
///     IsRefCounted<Button>::Result         -> true
///     IsRefCounted<const Button>::Result   -> true
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct IsRefCounted
{
    enum { Result = IsDerived<T, BaseRefCounted>::Result || IsDerived<T, Interface>::Result };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is an interface (derived from Interface but not from BaseRefCounted)
///
///   Example:
///     IsInterface<int>::Result            -> false
///     IsInterface<Button>::Result         -> false
///     IsInterface<ICommand>::Result       -> true
///     IsInterface<const ICommand>::Result -> true
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct IsInterface
{
    enum { Result = !IsDerived<T, BaseRefCounted>::Result && IsDerived<T, Interface>::Result };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is Ptr<>
///
///   Example:
///     IsPtr<MyClass>::Result      -> false
///     IsPtr<Ptr<MyClass>>::Result -> true
///     RemovePtr<Ptr<MyClass>>     -> MyClass
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class IsPtr
{
    template<class U> struct PtrTraits
    {
        enum { Result = false };
        typedef U Type;
    };
    template<class U> struct PtrTraits<Ptr<U>>
    {
        enum { Result = true };
        typedef U Type;
    };

public:
    enum { Result = PtrTraits<RemoveCV<T>>::Result };
    typedef typename PtrTraits<RemoveCV<T>>::Type PointedType;
};

/// Helper variable template
template<class T> using RemovePtr = typename IsPtr<T>::PointedType;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if a type is a non-union class type
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct IsClass
{
    enum { Result = __is_class(T) };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if type is an enumeration type
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct IsEnum
{
    enum { Result = __is_enum(T) };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if type can be safely copied with memcpy
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct IsTriviallyCopiable
{
    enum { Result = __is_trivially_copyable(T) };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Checks if it is best to pass a type as copy or as reference
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct IsBestByCopy
{
    typedef RemoveCV<T> T_;

    enum
    {
        Result =
            IsSame<T_, bool>::Result ||
            IsSame<T_, char>::Result ||
            IsSame<T_, signed char>::Result ||
            IsSame<T_, unsigned char>::Result ||
            IsSame<T_, signed short>::Result ||
            IsSame<T_, unsigned short>::Result ||
            IsSame<T_, signed int>::Result ||
            IsSame<T_, unsigned int>::Result ||
            IsSame<T_, signed long>::Result ||
            IsSame<T_, unsigned long>::Result ||
            IsSame<T_, signed long long>::Result ||
            IsSame<T_, unsigned long long>::Result ||
            IsSame<T_, Symbol>::Result ||
            IsSame<T_, float>::Result ||
            IsSame<T_, double>::Result ||
            IsSame<T_, long double>::Result ||
            IsEnum<T_>::Result ||
            IsPointer<T_>::Result
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Calculates the type that must be used to efficiently pass T as a parameter to a function
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct Param
{
    typedef If<IsBestByCopy<T>::Result, T, const T&> Type;
};

template<class T> struct Param<T&>
{
    typedef T& Type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// This metafunction is a convenient way to leverage SFINAE to conditionally remove functions
/// from overload resolution based on type traits and to provide separate function overloads and
/// specializations for different type traits. 
////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool B, class T = void> struct EnableIf_ {};

template<class T> struct EnableIf_<true, T>
{
    typedef T Type;
};

template<bool B, class T = void> using EnableIf = typename EnableIf_<B,T>::Type;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper function to force move semantics on values
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> 
inline constexpr RemoveReference<T>&& MoveArg(T&& arg)
{
    return static_cast<RemoveReference<T>&&>(arg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper for perfect forwarding of function arguments
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline constexpr T&& ForwardArg(RemoveReference<T>& arg)
{
    return static_cast<T&&>(arg);
}

template<class T>
inline constexpr T&& ForwardArg(RemoveReference<T>&& arg)
{
    return static_cast<T&&>(arg);
}

}


#endif
