////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_REFLECTIONHELPER_H__
#define __CORE_REFLECTIONHELPER_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/Symbol.h>
#include <NsCore/Ptr.h>
#include <NsCore/CompilerTools.h>


namespace Noesis
{

class Type;
class TypeClass;
class TypeMetaData;
class TypeProperty;
class BaseComponent;
class MetaData;


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a metadata of a specific type class
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TypeClassMetaData
{
    const TypeClass* typeClass;
    const TypeMetaData* metaData;

    inline TypeClassMetaData(const TypeClass* tc, const TypeMetaData* m);
};

/// Gets first metadata found in the passed class hierarchy that matches the metadata type
/// If found, returns the pointer to metadata and the class where the metadata was found
NS_CORE_KERNEL_API TypeClassMetaData FindMetaData(const TypeClass* typeClass,
    const TypeClass* metadata);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a property of a specific type class
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TypeClassProperty
{
    const TypeClass* typeClass;
    const TypeProperty* property;

    inline TypeClassProperty(const TypeClass* tc, const TypeProperty* p);
};

/// Gets first property found in the passed class hierarchy that match the property name
/// If found, returns the typeProperty and the typeClass where the property was found
NS_CORE_KERNEL_API TypeClassProperty FindProperty(const TypeClass* typeClass, Symbol propId);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an event of a specific type class
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TypeClassEvent
{
    const TypeClass* typeClass;
    const TypeProperty* event;

    inline TypeClassEvent(const TypeClass* tc, const TypeProperty* e);
};

/// Gets first event found in the passed class hierarchy that matches the event name
/// If found, returns the event and the typeClass where the property was found
NS_CORE_KERNEL_API TypeClassEvent FindEvent(const TypeClass* typeClass, Symbol eventId);

}

#include <NsCore/ReflectionHelper.inl>

#endif
