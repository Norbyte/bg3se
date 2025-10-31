////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_UIPROPERTYMETADATA_H__
#define __GUI_UIPROPERTYMETADATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/PropertyMetadata.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides property metadata for non-framework properties that do have rendering/user interface
/// impact at the core level.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API UIPropertyMetadata: public PropertyMetadata
{
public:
    // Helper functions to create a new PropertyMetadata
    //@{
    static Ptr<UIPropertyMetadata> Create();

    static Ptr<UIPropertyMetadata> Create(const PropertyChangedCallback& changed);

    static Ptr<UIPropertyMetadata> Create(const CoerceValueCallback& coerce);

    template<class T>
    inline static Ptr<UIPropertyMetadata> Create(const T& defaultValue);

    template<class T>
    inline static Ptr<UIPropertyMetadata> Create(const T& defaultValue,
        const PropertyChangedCallback& changed);

    template<class T>
    inline static Ptr<UIPropertyMetadata> Create(const T& defaultValue,
        const CoerceValueCallback& coerce);

    template<class T>
    inline static Ptr<UIPropertyMetadata> Create(const T& defaultValue,
        const PropertyChangedCallback& changed, const CoerceValueCallback& coerce);
    //@}

protected:
    // Constructors
    //@{
    UIPropertyMetadata();
    static Ptr<UIPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue);
    static Ptr<UIPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, const PropertyChangedCallback& changed);
    static Ptr<UIPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, const CoerceValueCallback& coerce);
    static Ptr<UIPropertyMetadata> Create(ValueStorageManager* defaultValueManager,
        const void* defaultValue, const PropertyChangedCallback& changed,
        const CoerceValueCallback& coerce);
    //@}

    NS_DECLARE_REFLECTION(UIPropertyMetadata, PropertyMetadata)
};

}

// Inline Include
#include <NsGui/UIPropertyMetadata.inl>


#endif
