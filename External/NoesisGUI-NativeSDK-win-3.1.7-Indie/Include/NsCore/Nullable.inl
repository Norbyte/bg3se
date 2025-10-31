////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Error.h>
#include <NsCore/ValueHelper.h>
#include <NsCore/Boxing.h>
#include <NsCore/TypeClassCreator.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
class TypeNullable: public TypeClass
{
public:
    TypeNullable(Symbol name, const Type* type_): TypeClass(name, false), type(type_) {}

    bool IsAssignableFrom(const Type* type_) const override
    {
        return TypeClass::IsAssignableFrom(type_) || type_ == type;
    }

    const Type* type;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct TypeClassFor<Nullable<T>>
{
    static Type* Create(Symbol name)
    {
        return new TypeNullable(name, TypeOf<T>());
    }
};


////////////////////////////////////////////////////////////////////////////////////////////////////
inline BaseNullable::BaseNullable(bool hasValue): mHasValue(hasValue)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool BaseNullable::HasValue() const
{
    return mHasValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Nullable<T>::Nullable(): BaseNullable(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Nullable<T>::Nullable(NullPtrT): BaseNullable(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Nullable<T>::Nullable(typename Param<T>::Type value): BaseNullable(true), mValue(value)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline typename Param<T>::Type Nullable<T>::GetValue() const
{
    NS_ASSERT(mHasValue);
    return mValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline typename Param<T>::Type Nullable<T>::GetValueOrDefault() const
{
    return mHasValue ? mValue : T();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Nullable<T>& Nullable<T>::operator=(typename Param<T>::Type value)
{
    mHasValue = true;
    mValue = value;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Nullable<T>& Nullable<T>::operator=(NullPtrT)
{
    mHasValue = false;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Nullable<T>::operator T() const
{
    return GetValue();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline bool Nullable<T>::operator==(const Nullable<T>& v) const
{
    return (!mHasValue && !v.mHasValue) || (mHasValue && v.mHasValue && mValue == v.mValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline bool Nullable<T>::operator!=(const Nullable<T>& value) const
{
    return !(*this == value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline bool Nullable<T>::operator==(typename Param<T>::Type value) const
{
    return mHasValue && mValue == value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline bool Nullable<T>::operator!=(typename Param<T>::Type value) const
{
    return !(*this == value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline bool Nullable<T>::operator==(NullPtrT) const
{
    return !mHasValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline bool Nullable<T>::operator!=(NullPtrT) const
{
    return mHasValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
String Nullable<T>::ToString() const
{
    return mHasValue ? Noesis::ToString(mValue) : "";
}

namespace Boxing
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class Boxer<Nullable<T>>
{
public:
    /// Boxing a nullable will return null if it has no value or will box the underlying type
    typedef Nullable<T> UnboxType;

    static Ptr<BoxedValue> Box(const Nullable<T>& value)
    {
        if (value.HasValue())
        {
            return Boxing::Box(value.GetValue());
        }

        return nullptr;
    }

    static bool CanUnbox(BaseComponent* object)
    {
        return object == nullptr || Boxing::CanUnbox<T>(object);
    }

    static UnboxType Unbox(BaseComponent* object)
    {
        if (object != 0)
        {
            return Nullable<T>(Boxing::Unbox<T>(object));
        }

        return nullptr;
    }
};

}
}
