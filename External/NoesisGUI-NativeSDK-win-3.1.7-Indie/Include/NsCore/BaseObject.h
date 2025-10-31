////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_BASEOBJECT_H__
#define __CORE_BASEOBJECT_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/StringFwd.h>


namespace Noesis
{

class TypeClass;
typedef decltype(sizeof(int)) SizeT;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for all Noesis polymorphic objects
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API BaseObject
{
public:
    BaseObject();
    BaseObject(const BaseObject&) = delete;
    BaseObject& operator=(const BaseObject&) = delete;
    virtual ~BaseObject() = 0;

    /// Redirect allocations to memory manager
    static void *operator new(SizeT size);
    static void operator delete(void* ptr);

    /// Determines whether the specified object instances are considered equal
    static bool Equals(const BaseObject* left, const BaseObject* right);

    /// Gets the class type information
    virtual const TypeClass* GetClassType() const;

    /// Returns a string that represents the current object
    virtual String ToString() const;

    /// Determines whether the specified object is equal to the current object
    virtual bool Equals(const BaseObject* object) const;

    NS_DECLARE_STATIC_REFLECTION(BaseObject, NoParent)
};

}

#endif
