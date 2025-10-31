////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPE_H__
#define __CORE_TYPE_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/BaseObject.h>
#include <NsCore/Symbol.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Root class for the type hierarchy. Type holds information about a run-time type
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API Type: public BaseObject
{
public:
    Type(Symbol name);
    Type(const Type&) = delete;
    Type& operator=(const Type&) = delete;

    /// Returns the name of the type
    const char* GetName() const;

    /// Returns the symbol name of the type
    Symbol GetTypeId() const;

    /// Determines if an instance of a specified type can be assigned to an instance of current type
    /// Returns true if any of the following conditions is true:
    ///  - 'type' and the current instance represent the same type
    ///  - 'type' is derived either directly or indirectly from the current instance
    ///  - The current instance is an interface that 'type' implements
    virtual bool IsAssignableFrom(const Type* type) const;

private:
    const Symbol mName;

    NS_DECLARE_REFLECTION(Type, NoParent)
};

NS_WARNING_POP

}

#endif
