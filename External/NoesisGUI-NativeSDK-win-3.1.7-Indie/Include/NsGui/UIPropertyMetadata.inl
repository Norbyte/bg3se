////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsGui/ValueStorageManagerImpl.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<UIPropertyMetadata> UIPropertyMetadata::Create(const T& defaultValue)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<UIPropertyMetadata> UIPropertyMetadata::Create(const T& defaultValue,
    const PropertyChangedCallback& changed)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, changed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<UIPropertyMetadata> UIPropertyMetadata::Create(const T& defaultValue,
    const CoerceValueCallback& coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<UIPropertyMetadata> UIPropertyMetadata::Create(const T& defaultValue,
    const PropertyChangedCallback& changed, const CoerceValueCallback& coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, changed, coerce);
}

}