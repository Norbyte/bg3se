////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_REGISTERCOMPONENT_H__
#define __CORE_REGISTERCOMPONENT_H__


#include <NsCore/TypeOf.h>
#include <NsCore/KernelApi.h>


namespace Noesis
{

/// Registers the given type in the factory. Registered types can be created in XAML
template<class T> void RegisterComponent();

/// Unregisters given type from the factory
template<class T> void UnregisterComponent();

}

#include <NsCore/RegisterComponent.inl>

#endif
