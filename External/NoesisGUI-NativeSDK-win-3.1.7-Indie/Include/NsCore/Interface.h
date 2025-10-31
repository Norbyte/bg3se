////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_INTERFACE_H__
#define __CORE_INTERFACE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionImplementEmpty.h>


namespace Noesis
{

class TypeClass;
class BaseObject;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Interface. Used as base for all component interfaces.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SWIG
NS_INTERFACE Interface
{
    virtual ~Interface() = 0;

    /// Gets the class type information
    virtual const TypeClass* GetClassType() const = 0;

    /// Returns a pointer to the BaseObject implementing this interface
    virtual BaseObject* GetBaseObject() const = 0;

    // Increments reference count and returns the new value
    virtual int32_t AddReference() const = 0;

    /// Decrements reference count and returns the new value
    virtual int32_t Release() const = 0;

    /// Gets current reference count for the object
    virtual int32_t GetNumReferences() const = 0;

    NS_IMPLEMENT_INLINE_STATIC_REFLECTION_(Interface, NoParent, "Interface")
};

inline Interface::~Interface() {}

// Helper macro to be inserted in classes that implement one or more interfaces
#define NS_IMPLEMENT_INTERFACE_FIXUP \
    Noesis::BaseObject* GetBaseObject() const override { return (Noesis::BaseObject*)this; } \
    int32_t AddReference() const override { return Noesis::BaseRefCounted::AddReference(); } \
    int32_t Release() const override { return Noesis::BaseRefCounted::Release(); } \
    int32_t GetNumReferences() const override { return Noesis::BaseRefCounted::GetNumReferences(); }

#endif

}


#endif
