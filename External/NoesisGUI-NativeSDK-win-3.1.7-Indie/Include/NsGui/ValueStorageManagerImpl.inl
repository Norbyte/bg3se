////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Ptr.h>
#include <NsCore/Boxing.h>
#include <NsCore/Memory.h>
#include <NsCore/CompilerTools.h>
#include <NsGui/DependencyObject.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> struct ValueStorageManagerHelper
{
    static bool CheckType(const Type*, BaseComponent* value)
    {
        return Boxing::CanUnbox<T>(value);
    }

    static Ptr<BaseComponent> Box(const void* value)
    {
        return Boxing::Box<T>(*static_cast<const T*>(value));
    }

    static typename Boxing::Boxer<T>::UnboxType Unbox(BaseComponent* value)
    {
        return Boxing::Unbox<T>(value);
    }
};

template<class T> struct ValueStorageManagerHelper<Ptr<T>>
{
    static bool CheckType(const Type* type, BaseComponent* value)
    {
        return value != 0 ? type->IsAssignableFrom(value->GetClassType()) : true;
    }

    static Ptr<BaseComponent> Box(const void* value)
    {
        return *static_cast<const Ptr<T>*>(value);
    }

    static Ptr<T> Unbox(BaseComponent* value)
    {
        return Ptr<T>(static_cast<T*>(value));
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
ValueStorageManager* ValueStorageManagerImpl<T>::Clone() const
{
    return new ValueStorageManagerImpl<T>();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
uint32_t ValueStorageManagerImpl<T>::Size() const
{
    return sizeof(T);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool ValueStorageManagerImpl<T>::CheckType(const Type* type) const
{
    return type->IsAssignableFrom(TypeOf<RemovePtr<T>>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool ValueStorageManagerImpl<T>::CheckType(const Type* type,
    BaseComponent* value) const
{
    return ValueStorageManagerHelper<T>::CheckType(type, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool ValueStorageManagerImpl<T>::Validate(ValidateValueCallback validate,
    BaseComponent* value) const
{
    const T& value_ = ValueStorageManagerHelper<T>::Unbox(value);
    return validate(&value_);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void ValueStorageManagerImpl<T>::Construct(ValueStorage* storage, const void* value) const
{
    switch ((int)(sizeof(T) <= sizeof(ValueStorage)))
    {
        case true:
        {
            new(storage) T(*static_cast<const T*>(value));
            break;
        }
        case false:
        {
            void* storage_ = Alloc(sizeof(T));
            *storage = new(storage_) T(*static_cast<const T*>(value));
            break;
        }
        default: NS_ASSERT_UNREACHABLE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void ValueStorageManagerImpl<T>::Destroy(ValueStorage* storage) const
{
    switch ((int)(sizeof(T) <= sizeof(ValueStorage)))
    {
        case true:
        {
            static_cast<T*>(static_cast<void*>(storage))->~T();
            break;
        }
        case false:
        {
            static_cast<T*>(static_cast<void*>(*storage))->~T();
            Dealloc(*storage);
            break;
        }
        default: NS_ASSERT_UNREACHABLE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void* ValueStorageManagerImpl<T>::ToValue(const ValueStorage* storage) const
{
    switch ((int)(sizeof(T) <= sizeof(ValueStorage)))
    {
        case true:
        {
            return const_cast<void*>(static_cast<const void*>(storage));
        }
        case false:
        {
            return const_cast<void*>(*static_cast<void*const*>(storage));
        }
        default: NS_ASSERT_UNREACHABLE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void ValueStorageManagerImpl<T>::Copy(void* dst, const void* value) const
{
    T& dst_ = *static_cast<T*>(dst);
    dst_ = *static_cast<const T*>(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void ValueStorageManagerImpl<T>::Copy(void* dst, BaseComponent* value) const
{
    const T& val = ValueStorageManagerHelper<T>::Unbox(value);
    ValueStorageManagerImpl::Copy(dst, &val);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
Ptr<BaseComponent> ValueStorageManagerImpl<T>::Box(const void* value) const
{
    return ValueStorageManagerHelper<T>::Box(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
const void* ValueStorageManagerImpl<T>::GetValue(const DependencyObject* dob,
    const DependencyProperty* dp) const
{
    return &dob->GetValue<T>(dp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
const void* ValueStorageManagerImpl<T>::GetBaseValue(const DependencyObject* dob,
    const DependencyProperty* dp) const
{
    return &dob->GetBaseValue<T>(dp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> bool IsSameValue(const T& l, const T& r)
{
    return Noesis::Equals(l, r);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool IsSameValue(const Ptr<BaseComponent>& l, const Ptr<BaseComponent>& r)
{
    return BaseObject::Equals(l, r);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool ValueStorageManagerImpl<T>::IsSame(const void* left, const void* right) const
{
    const T& l = *static_cast<const T*>(left);
    const T& r = *static_cast<const T*>(right);
    return IsSameValue(l, r);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool ValueStorageManagerImpl<T>::IsSame(const void* left, BaseComponent* right) const
{
    const T& l = *static_cast<const T*>(left);
    const T& r = ValueStorageManagerHelper<T>::Unbox(right);
    return IsSameValue(l, r);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void ValueStorageManagerImpl<T>::SetValue(DependencyObject* dob, const DependencyProperty* dp,
    const void* value, uint8_t priority, Expression* expression, const PropertyMetadata* metadata,
    Value::Destination destination) const
{
    const T& value_ = *static_cast<const T*>(value);

    T oldValue, coercedValue;
    dob->InternalSetValue(dp, &oldValue, &value_, &coercedValue, priority, expression, metadata,
        destination, IsPtr<T>::Result);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void ValueStorageManagerImpl<T>::SetValue(DependencyObject* dob, const DependencyProperty* dp,
    BaseComponent* value, uint8_t priority, Expression* expression,
    const PropertyMetadata* metadata, Value::Destination destination) const
{
    const T& value_ = ValueStorageManagerHelper<T>::Unbox(value);

    T oldValue, coercedValue;
    dob->InternalSetValue(dp, &oldValue, &value_, &coercedValue, priority, expression, metadata,
        destination, IsPtr<T>::Result);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void ValueStorageManagerImpl<T>::ClearAnimation(DependencyObject* dob,
    const DependencyProperty* dp) const
{
    dob->ClearAnimation<T>(dp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void ValueStorageManagerImpl<T>::CoerceValue(DependencyObject* dob, const DependencyProperty* dp,
    StoredValue* storedValue, const void* defaultValue, const PropertyMetadata* metadata) const
{
    dob->InternalCoerceValue<T>(dp, storedValue, defaultValue, metadata);
}

}
