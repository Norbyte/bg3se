////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_FACTORY_H__
#define __CORE_FACTORY_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>


namespace Noesis
{

class Symbol;
class BaseComponent;
template<class T> class Ptr;
template<class T> class ArrayRef;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A factory where instances that need to be created from string are registered
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Factory
{

/// Checks if a component is registered in the factory
NS_CORE_KERNEL_API bool IsComponentRegistered(Symbol name);

/// Enumerates components belonging to the specified category
NS_CORE_KERNEL_API ArrayRef<Symbol> EnumComponents(Symbol category);

/// Creates an instance of the specified component 
NS_CORE_KERNEL_API Ptr<BaseComponent> CreateComponent(Symbol name);

/// Registers a component
typedef BaseComponent* (*CreatorFn)(Symbol name);
NS_CORE_KERNEL_API void RegisterComponent(Symbol name, Symbol category, CreatorFn func);

/// Unregisters a previously registered component
NS_CORE_KERNEL_API void UnregisterComponent(Symbol name);

/// Clears the factory and deallocates all internal memory
void Shutdown();

};

}

#endif
