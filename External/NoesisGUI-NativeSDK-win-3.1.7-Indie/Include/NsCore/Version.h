////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_VERSION_H__
#define __CORE_VERSION_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>


namespace Noesis
{
 
/// Returns the build version, for example "1.2.6f5"
NS_CORE_KERNEL_API const char* GetBuildVersion();

}

#endif
