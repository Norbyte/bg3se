////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPEMETA_H__
#define __CORE_TYPEMETA_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/Type.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Vector.h>


namespace Noesis
{

class Symbol;
class TypeMetaData;
template<class T> class ArrayRef;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Type with associated metadata
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API TypeMeta: public Type
{
public:
    TypeMeta(Symbol name);
    ~TypeMeta();

    /// Adds new metadata
    void AddMeta(TypeMetaData* metaData);

    /// Returns the first metadata found with the given type. May return null
    TypeMetaData* FindMeta(const TypeClass* metaDataType) const;

    /// Returns an array containing all metadata
    ArrayRef<TypeMetaData*> GetMeta() const;

public:
    typedef Vector<TypeMetaData*> MetaData;
    MetaData mMetaData;

    NS_DECLARE_REFLECTION(TypeMeta, Type)
};

NS_WARNING_POP

/// FindMeta helper function
template<class T> T* FindMeta(const TypeMeta* type);

}

#include <NsCore/TypeMeta.inl>

#endif
