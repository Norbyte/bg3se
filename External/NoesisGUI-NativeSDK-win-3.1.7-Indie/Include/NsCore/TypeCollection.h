////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPECOLLECTION_H__
#define __CORE_TYPECOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/Type.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypeCollection. Types for STL containers.
////////////////////////////////////////////////////////////////////////////////////////////////////
//@{
class NS_CORE_KERNEL_API TypeCollection: public Type
{
public:
    /// Constructor
    TypeCollection(Symbol name);

    /// Gets the static type of collection elements
    const Type* GetElemType() const;

    /// Sets static type of collection elements
    void SetElemType(const Type* elemType);

    /// Gets number of elements in the collection
    /// \param ptr Pointer to the vector collection
    virtual uint32_t GetNumElems(const void* ptr) const = 0;

    /// Gets ith element
    virtual const void* Get(const void* ptr, uint32_t i) const = 0;

    /// Obtains a string describing the implementation of this Collection. For example:
    /// "vector", "list", ...
    virtual const char* GetCollectionId() const = 0;

protected:
    const Type* mElemType;

    NS_DECLARE_REFLECTION(TypeCollection, Type)
};

}

#endif
