////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef __GUI_PROVIDERVALUE_H__
#define __GUI_PROVIDERVALUE_H__


#include <NsCore/Noesis.h> 
#include <NsCore/Ptr.h>
#include <NsGui/DependencySystemApi.h>


namespace Noesis
{

class BaseComponent;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

struct NS_GUI_DEPENDENCYSYSTEM_API ProviderValue
{
    ProviderValue();

    // Return values can be a boxed value or a raw value
    //@{
    // TODO: This can be changed to a BaseComponent* because providers doesn't create elements
    // TODO: Put into a union
    Ptr<BaseComponent> boxed;
    const void* rawValue;
    //@}

    /// Priority of the provider which returned the values
    uint8_t providerPriority;
    
    // Indicates which of the above fields is valid. If its a raw value, the caching can be avoided
    // TODO [srodriguez] This will dissapear and the condition to not cache an element will be
    // comparing priority with InheritedValueProviderPriority or DefaultValueProviderPriority
    bool isRawValueAndNonCached;
    
    // Use 8 bits to store provider priority (256 possible providers)
    // User providers can't utilize LocalProviderPriority nor DefaultValueProviderPriority,
    // so there can be 254 max user providers
    static const uint8_t LocalProviderPriority = 0;
    static const uint8_t InheritedValueProviderPriority = 0xFE;
    static const uint8_t DefaultValueProviderPriority = 0xFF;
};

NS_WARNING_POP

}

#endif
