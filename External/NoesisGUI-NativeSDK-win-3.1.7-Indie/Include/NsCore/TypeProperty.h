////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPEPROPERTY_H__
#define __CORE_TYPEPROPERTY_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/Symbol.h>
#include <NsCore/CompilerTools.h>
#include <NsCore/Vector.h>


namespace Noesis
{

class Type;
class TypeClass;
class TypeMetaData;
class BaseComponent;
template<class T> class ArrayRef;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypeProperty. Defines a property of a reflection class type.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API TypeProperty
{
public:
    TypeProperty(Symbol name, const Type* type);
    TypeProperty(const TypeProperty&) = delete;
    TypeProperty& operator=(const TypeProperty&) = delete;
    virtual ~TypeProperty() = 0;

    /// Gets property's reflection name
    Symbol GetName() const;

    /// Gets property's reflection type
    const Type* GetContentType() const;

    /// Adds new metadata
    void AddMeta(TypeMetaData* metaData);

    /// Returns the first metadata found with the given type. May return null
    TypeMetaData* FindMeta(const TypeClass* metaDataType) const;

    /// Returns an array containing all metadata
    ArrayRef<TypeMetaData*> GetMeta() const;

    /// Gets property's content address
    virtual void* GetContent(const void* ptr) const = 0;

    /// Indicates if property is read-only
    virtual bool IsReadOnly() const;

    /// Gets property value using a boxed value or component
    virtual Ptr<BaseComponent> GetComponent(const void* ptr) const;

    /// Sets property value using a boxed value or component
    virtual void SetComponent(void* ptr, BaseComponent* value) const;

    /// Copies the value of the pointer to the given address
    virtual void GetCopy(const void* ptr, void* dest) const;

    /// Gets a pointer to the value of the property
    virtual const void* Get(const void* ptr) const;

    /// Sets the value of the property
    virtual void Set(void* ptr, const void* value) const;

public:
    Symbol mName;
    const Type* mType;

    typedef Vector<TypeMetaData*> MetaData;
    MetaData mMetaData;
};

NS_WARNING_POP

/// FindMeta helper function
template<class T> T* FindMeta(const TypeProperty* type);

/// Get property helper function
template<class T> typename Param<T>::Type PropGet(const TypeProperty* type, const void* ptr);

/// Set property helper function
template<class T> void PropSet(const TypeProperty* type, void* ptr, typename Param<T>::Type value);

}

#include <NsCore/TypeProperty.inl>

#endif
