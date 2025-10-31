////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_NULLABLE_H__
#define __CORE_NULLABLE_H__


#include <NsCore/Noesis.h>
#include <NsCore/IdOf.h>
#include <NsCore/ReflectionImplementEmpty.h>


namespace Noesis
{

typedef decltype(nullptr) NullPtrT;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for Nullable objects
////////////////////////////////////////////////////////////////////////////////////////////////////
class BaseNullable
{
public:
    /// Gets a value indicating whether the nullable has a valid value of its underlying type
    bool HasValue() const;

protected:
    BaseNullable(bool hasValue);

protected:
    bool mHasValue;

    NS_IMPLEMENT_INLINE_REFLECTION_(BaseNullable, NoParent)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a value type that can be assigned nullptr
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class Nullable: public BaseNullable
{
public:
    /// Initializes a new null instance
    Nullable();
    Nullable(NullPtrT);

    /// Constructor from value
    Nullable(typename Param<T>::Type value);

    /// Gets the value if it has been assigned a valid underlying value (HasValue() == true)
    typename Param<T>::Type GetValue() const;

    /// Gets the value if it has been assigned or the object's default value
    typename Param<T>::Type GetValueOrDefault() const;

    /// Assigns from value
    Nullable<T>& operator=(typename Param<T>::Type value);

    /// Assigns from nullptr
    Nullable<T>& operator=(NullPtrT);

    /// Explicit conversion to value
    explicit operator T() const;
    
    /// Equality operators
    bool operator==(const Nullable<T>& other) const;
    bool operator!=(const Nullable<T>& other) const;
    bool operator==(typename Param<T>::Type value) const;
    bool operator!=(typename Param<T>::Type value) const;
    bool operator==(NullPtrT) const;
    bool operator!=(NullPtrT) const;

    /// Returns a string that represents the current nullable
    String ToString() const;

private:
    T mValue;

    NS_IMPLEMENT_INLINE_REFLECTION_(Nullable, BaseNullable, IdOf("Nullable<", IdOf<T>(), ">"))
};

}

#include "Nullable.inl"

#endif
