////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsGui/ValueStorageManagerImpl.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    const PropertyChangedCallback& changed)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, changed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    const CoerceValueCallback& coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    uint32_t options)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, options);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    uint32_t options, UpdateSourceTrigger defaultTrigger)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, options, defaultTrigger);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    uint32_t options, const PropertyChangedCallback& changed)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, options, changed);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    uint32_t options, const CoerceValueCallback& coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, options, coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    uint32_t options, const PropertyChangedCallback& changed, const CoerceValueCallback& coerce)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, options, changed, coerce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Ptr<FrameworkPropertyMetadata> FrameworkPropertyMetadata::Create(const T& defaultValue,
    uint32_t options, const PropertyChangedCallback& changed, const CoerceValueCallback& coerce,
    UpdateSourceTrigger defaultTrigger)
{
    ValueStorageManagerImpl<T>* defaultValueManager = new ValueStorageManagerImpl<T>();
    return Create(defaultValueManager, &defaultValue, options, changed, coerce, defaultTrigger);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::AffectsMeasure() const
{
    return (mOptions & FrameworkPropertyMetadataOptions_AffectsMeasure) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::AffectsArrange() const
{
    return (mOptions & FrameworkPropertyMetadataOptions_AffectsArrange) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::AffectsParentMeasure() const
{
    return (mOptions & FrameworkPropertyMetadataOptions_AffectsParentMeasure) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::AffectsParentArrange() const
{
    return (mOptions & FrameworkPropertyMetadataOptions_AffectsParentArrange) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::AffectsRender() const
{
    return (mOptions & FrameworkPropertyMetadataOptions_AffectsRender) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::Inherits() const
{
    return (mOptions & FrameworkPropertyMetadataOptions_Inherits) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::OverridesInheritanceBehavior() const
{
    return (mOptions & FrameworkPropertyMetadataOptions_OverridesInheritanceBehavior) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::IsNotDataBindable() const
{
    return (mOptions & FrameworkPropertyMetadataOptions_NotDataBindable) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::BindsTwoWayByDefault() const
{
    return (mOptions & FrameworkPropertyMetadataOptions_BindsTwoWayByDefault) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::Journal() const
{
    return (mOptions & FrameworkPropertyMetadataOptions_Journal) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool FrameworkPropertyMetadata::SubPropertiesDoNotAffectRender() const
{
    return (mOptions & FrameworkPropertyMetadataOptions_SubPropertiesDoNotAffectRender) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline UpdateSourceTrigger FrameworkPropertyMetadata::DefaultUpdateSourceTrigger() const
{
    return static_cast<UpdateSourceTrigger>((mOptions >> 30) & 0x3);
}


}
