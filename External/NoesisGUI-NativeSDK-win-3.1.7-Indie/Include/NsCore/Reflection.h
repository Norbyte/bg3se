////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_REFLECTION_H__
#define __CORE_REFLECTION_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>


namespace Noesis
{

class Symbol;
class Type;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Database with reflection types
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Reflection
{

/// Finds the given reflection type by name. Note that reflection types are lazily registered on
/// demand. Sometimes the registration must be forced
NS_CORE_KERNEL_API const Type* GetType(Symbol name);

/// Iterates the registered types calling the passed in function
typedef void (*EnumTypesCallback)(const Type* type);
NS_CORE_KERNEL_API void EnumTypes(EnumTypesCallback callback);

/// Lazy-registration of a type. The type is contructed with the given creator and filler only
/// if the given name is not already registered. This funcion is needed to avoid multiple
/// registrations of the same type in different dynamic libraries or threads
typedef Type* (*CreatorFn)(Symbol name);
typedef void (*FillerFn)(Type* type);
NS_CORE_KERNEL_API Type* RegisterType(Symbol name, CreatorFn creator, FillerFn filler = 0);
NS_CORE_KERNEL_API Type* RegisterType(const char* name, CreatorFn creator, FillerFn filler = 0);

/// Registers the given type. Type will be internally 'deleted' at shutdown
NS_CORE_KERNEL_API void RegisterType(Type* type);

/// The fallback handler is invoked when requesting a type that is not registered
typedef void (*FallbackHandler)(Symbol name);
NS_CORE_KERNEL_API void SetFallbackHandler(FallbackHandler handler);

/// Removes a previously registered type
NS_CORE_KERNEL_API void Unregister(const Type* type);

/// Removes a previously registered type, but doesn't delete it
NS_CORE_KERNEL_API void UnregisterNoDelete(const Type* type);

/// Checks if a type is registered
NS_CORE_KERNEL_API bool IsTypeRegistered(Symbol name);

/// Clears the database and deallocates all internal memory
void Shutdown();

};

}

#endif
