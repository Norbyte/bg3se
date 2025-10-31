////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Boxing.h>
#include <NsCore/Ptr.h>
#include <NsCore/String.h>
#include <NsGui/Expression.h>
#include <NsGui/ProviderValue.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline DependencyPropertyChangedEventArgs::DependencyPropertyChangedEventArgs(
    const DependencyProperty* dp, const void* oldValue_, const void* newValue_,
    const PropertyMetadata* metadata_): prop(dp), oldValue(oldValue_), newValue(newValue_),
    metadata(metadata_)
{
    NS_ASSERT(dp != 0);
    NS_ASSERT(oldValue_ != 0);
    NS_ASSERT(newValue_ != 0);
    NS_ASSERT(metadata != 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> const T& DependencyPropertyChangedEventArgs::OldValue() const
{
    return *(const T*) oldValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> const T& DependencyPropertyChangedEventArgs::NewValue() const
{
    return *(const T*) newValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
const T& DependencyObject::GetValue(const DependencyProperty* dp) const
{
    return *static_cast<const T*>(InternalGetValue(dp, sizeof(T)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
const T& DependencyObject::GetBaseValue(const DependencyProperty* dp) const
{
    return *static_cast<const T*>(InternalGetBaseValue(dp, sizeof(T)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyObject::SetValue(const DependencyProperty* dp, typename SetValueType<T>::Type value)
{
    if (dp == 0)
    {
        NS_ERROR("Can't set value, DependencyProperty is null");
        return;
    }

    if (!CheckReadOnly(dp))
    {
        return;
    }

    enum { ValueTypeId = IsSame<T, String>::Result ? 2 : IsPtr<T>::Result ? 1 : 0 };
    SetValue_<T>(Int2Type<ValueTypeId>(), dp, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyObject::SetReadOnlyProperty(const DependencyProperty* dp,
    typename SetValueType<T>::Type value) const
{
    if (dp == 0)
    {
        NS_ERROR("Can't set value, DependencyProperty is null");
        return;
    }

    DependencyObject* this_ = const_cast<DependencyObject*>(this);
    enum { ValueTypeId = IsSame<T, String>::Result ? 2 : IsPtr<T>::Result ? 1 : 0 };
    this_->SetValue_<T>(Int2Type<ValueTypeId>(), dp, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyObject::SetCurrentValue(const DependencyProperty* dp,
    typename SetValueType<T>::Type value)
{
    if (dp == 0)
    {
        NS_ERROR("Can't set current value, DependencyProperty is null");
        return;
    }

    if (!CheckReadOnly(dp))
    {
        return;
    }

    enum { ValueTypeId = IsSame<T, String>::Result ? 2 : IsPtr<T>::Result ? 1 : 0 };
    SetValue_<T>(Int2Type<ValueTypeId>(), dp, value, Value::Destination_CurrentValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyObject::SetAnimation(const DependencyProperty* dp,
    typename SetValueType<T>::Type value)
{
    if (dp == 0)
    {
        NS_ERROR("Can't set animation value, DependencyProperty is null");
        return;
    }

    if (!CheckReadOnly(dp))
    {
        return;
    }

    enum { ValueTypeId = IsSame<T, String>::Result ? 2 : IsPtr<T>::Result ? 1 : 0 };
    SetValue_<T>(Int2Type<ValueTypeId>(), dp, value, Value::Destination_Animation);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyObject::ClearAnimation(const DependencyProperty* dp)
{
    if (dp == 0)
    {
        NS_ERROR("Can't clear value, DependencyProperty is null");
        return;
    }

    T oldValue, coercedValue, baseValue;
    InternalClearAnimation(dp, &oldValue, &coercedValue, &baseValue, IsPtr<T>::Result);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyObject::CoerceValue(const DependencyProperty* dp)
{
    if (dp == 0)
    {
        NS_ERROR("Can't coerce value, DependencyProperty is null");
        return;
    }

    T oldValue, coercedValue;
    InternalCoerceValue(dp, &oldValue, &coercedValue, IsPtr<T>::Result);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
DependencyPropertyChangedEventHandler& DependencyObject::DependencyPropertyChanged()
{
    VerifyAccess();

    return mDependencyPropertyChangedEvent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
DependencyObject::DestroyedDelegate& DependencyObject::Destroyed()
{
    VerifyAccess();

    return mDestroyedDelegate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyObject::SetValue_(IsNotBaseComponent, const DependencyProperty* dp,
    typename Param<T>::Type value, Value::Destination destination)
{
    if (!CheckTypes(TypeOf<T>(), dp))
    {
        return;
    }

    T oldValue, coercedValue;
    InternalSetValue(dp, &oldValue, &value, &coercedValue, 0, 0, 0, destination, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyObject::SetValue_(IsBaseComponent, const DependencyProperty* dp,
    typename T::Type* value, Value::Destination destination)
{
    Ptr<BaseComponent> value_(value);
    if (!CheckTypes(value_, dp))
    {
        return;
    }

    Ptr<BaseComponent> oldValue, coercedValue;
    InternalSetValue(dp, &oldValue, &value_, &coercedValue, 0, 0, 0, destination, true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyObject::SetValue_(IsString, const DependencyProperty* dp,
    const char* value, Value::Destination destination)
{
    if (!CheckTypes(TypeOf<String>(), dp))
    {
        return;
    }

    FixedString<512> oldValue, coercedValue, newValue(value);
    InternalSetValue(dp, &oldValue, &newValue, &coercedValue, 0, 0, 0, destination, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void DependencyObject::InternalCoerceValue(const DependencyProperty* dp, StoredValue* sv,
    const void* defaultValue, const PropertyMetadata* metadata)
{
    T oldValue, coercedValue;
    InternalCoerceValue(dp, sv, defaultValue, &oldValue, &coercedValue, metadata, IsPtr<T>::Result);
}

}
