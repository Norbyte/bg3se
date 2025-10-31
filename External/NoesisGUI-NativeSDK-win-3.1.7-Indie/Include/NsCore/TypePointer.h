////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPEPOINTER_H__
#define __CORE_TYPEPOINTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/TypeMeta.h>
#include <NsCore/TypeCollection.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypeReference. Types for references.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API TypeReference: public Type
{
public:
    /// Constructor
    TypeReference(Symbol name);

    /// Sets reflection static type of reference content
    void SetContentType(const Type* contentType);

    /// Gets reflection type of runtime reference content
    /// \return Runtime reference content's reflection info
    inline const Type* GetContentType() const;

protected:
    const Type* mContentType;

    NS_DECLARE_REFLECTION(TypeReference, Type)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypeConst. Types for const objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API TypeConst: public Type
{
public:
    /// Constructor
    TypeConst(Symbol name);

    /// Sets reflection static type of const content
    void SetContentType(const Type* contentType);

    /// Gets reflection type of runtime const content
    /// \return Runtime const content's reflection info
    inline const Type* GetContentType() const;

protected:
    const Type* mContentType;

    NS_DECLARE_REFLECTION(TypeConst, Type)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypePointer. Types for pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API TypePointer: public TypeMeta
{
public:
    /// Constructor
    TypePointer(Symbol name);

    /// Sets reflection static type of pointer content
    void SetStaticContentType(const Type* contentType);

    /// Gets reflection type of pointer content static type
    /// \return Pointer content type's reflection info
    inline const Type* GetStaticContentType() const;

    /// Gets reflection type of runtime pointer content
    /// \param ptr Address of the pointer object
    /// \return Runtime pointer content's reflection info
    const Type* GetContentType(void* ptr) const;

    /// Gets the address of pointer content
    /// \return Pointer content's address
    void* GetContent(void* ptr) const;

protected:
    const Type* mContentType;

    NS_DECLARE_REFLECTION(TypePointer, TypeMeta)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypePtr. Types for Ptr objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API TypePtr: public TypeMeta
{
public:
    /// Constructor
    TypePtr(Symbol name);

    /// Sets reflection static type of Ptr content
    void SetStaticContentType(const Type* contentType);

    /// Gets reflection type of Ptr content static type
    /// \return Ptr content type's reflection info
    inline const Type* GetStaticContentType() const;

    /// Gets reflection type of runtime Ptr content
    /// \param ptr Address of the Ptr object
    /// \return Runtime Ptr content's reflection info
    const TypeClass* GetContentType(void* ptr) const;

    /// Gets the address of Ptr content
    /// \return Ptr content's address
    void* GetContent(void* ptr) const;

protected:
    const Type* mContentType;

    NS_DECLARE_REFLECTION(TypePtr, TypeMeta)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypeArray. Defines reflection info for a static array type.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API TypeArray: public TypeCollection
{
public:
    /// Constructor
    TypeArray(Symbol name);

    /// Sets number of elements in the array
    void SetElemCount(uint32_t elemCount);

    /// Sets size of the elements in the array
    void SetElemSize(uint32_t elemSize);

    /// Gets number of elements of the array
    inline uint32_t GetNumElems() const;

    /// From TypeCollection
    //@{
    uint32_t GetNumElems(const void* ptr) const;
    const void* Get(const void* ptr, uint32_t i) const;
    const char* GetCollectionId() const;
    //@}

protected:
    uint32_t mElemCount;
    uint32_t mElemSize;

    NS_DECLARE_REFLECTION(TypeArray, TypeCollection)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper function to extract the TypeClass of the component that could be referenced by 'type'
/// if it is a TypePtr or TypePointer.
///
/// For example:
///
///     ExtractComponentType(TypeOf<Ptr<BaseComponent>>()) == TypeOf<BaseComponent>();
///     ExtractComponentType(TypeOf<Ptr<const BaseComponent>>()) == TypeOf<BaseComponent>();
///     ExtractComponentType(TypeOf<Ptr<Interface>>()) == TypeOf<Interface>();
///     ExtractComponentType(TypeOf<Ptr<const Interface>>()) == TypeOf<Interface>();
///     ExtractComponentType(TypeOf<BaseComponent*>()) == TypeOf<BaseComponent>();
///     ExtractComponentType(TypeOf<const BaseComponent*>()) == TypeOf<BaseComponent>();
///     ExtractComponentType(TypeOf<Interface*>()) == TypeOf<Interface>();
///     ExtractComponentType(TypeOf<const Interface*>()) == TypeOf<Interface>();
///     ExtractComponentType(TypeOf<MyStruct*>()) == 0;
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_CORE_KERNEL_API
const TypeClass* ExtractComponentType(const Type* type);

/// Indicates if type is a BaseComponent
NS_CORE_KERNEL_API bool IsBaseComponentType(const TypeClass* type);

}

// Inline Include
#include <NsCore/TypePointer.inl>

#endif
