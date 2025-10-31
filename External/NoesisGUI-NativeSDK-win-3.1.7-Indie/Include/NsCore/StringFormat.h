////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_STRINGFORMAT_H__
#define __CORE_STRINGFORMAT_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>


namespace Noesis
{

class BaseComponent;
class BaseString;
template<typename T> class ArrayRef;
struct CultureInfo;


/// String formatting using C# style ("F4", "###.0", "######e0")
NS_CORE_KERNEL_API void StringFormat(ArrayRef<BaseComponent*> values, const char* format,
    const CultureInfo& culture, BaseString& str);
NS_CORE_KERNEL_API void StringFormat(BaseComponent* value, const char* format,
    const CultureInfo& culture, BaseString& str);

/// Float formatting using general ("G") format
NS_CORE_KERNEL_API void StringFormat(float value, BaseString& str);

}

#endif
