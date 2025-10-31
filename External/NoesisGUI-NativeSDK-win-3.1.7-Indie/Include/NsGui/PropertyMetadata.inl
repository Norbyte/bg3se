////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsGui/ValueStorageManagerImpl.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<PropertyMetadata> PropertyMetadata::Create(const T& defaultValue)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<PropertyMetadata> PropertyMetadata::Create(const T& defaultValue,
    const PropertyChangedCallback& changed)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, changed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<PropertyMetadata> PropertyMetadata::Create(const T& defaultValue,
    const CoerceValueCallback& coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<PropertyMetadata> PropertyMetadata::Create(const T& defaultValue,
    const PropertyChangedCallback& changed, const CoerceValueCallback& coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, changed, coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
void PropertyMetadata::SetDefaultValue(const T& defaultValue)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    SetDefaultValue(defaultValueManager, &defaultValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool PropertyMetadata::HasDefaultValue() const
{
    return (mLocalFlags & PropertyFlags_Default) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const void* PropertyMetadata::GetDefaultValue() const
{
    return mDefaultValueManager != 0 ? mDefaultValueManager->ToValue(&mDefaultValue) : nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Ptr<BaseComponent> PropertyMetadata::GetDefaultValueObject() const
{
    return mDefaultValueManager != 0 ? mDefaultValueManager->Box(GetDefaultValue()) : nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ValueStorageManager* PropertyMetadata::GetValueManager() const
{
    return mDefaultValueManager;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool PropertyMetadata::HasPropertyChangedCallback() const
{
    return (mLocalFlags & PropertyFlags_Changed) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const PropertyChangedCallback& PropertyMetadata::GetPropertyChangedCallback() const
{
    return mChanged;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool PropertyMetadata::HasCoerceValueCallback() const
{
    return (mLocalFlags & PropertyFlags_Coerce) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const CoerceValueCallback& PropertyMetadata::GetCoerceValueCallback() const
{
    return mCoerce;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool PropertyMetadata::IsUncached() const
{
    return (mLocalFlags & PropertyFlags_Uncached) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool PropertyMetadata::GetUncached() const
{
    return mUncached;
}

}
