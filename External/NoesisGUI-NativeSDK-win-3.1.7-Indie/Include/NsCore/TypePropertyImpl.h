////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPEPROPERTYIMPL_H__
#define __CORE_TYPEPROPERTYIMPL_H__


#include <NsCore/Noesis.h>
#include <NsCore/TypeProperty.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Property defined by an offset from the instance pointer
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
class TypePropertyOffset: public TypeProperty
{
public:
    /// Constructor
    TypePropertyOffset(Symbol name, uint32_t offset);

    /// From TypeProperty
    //@{
    void* GetContent(const void* ptr) const override;
    bool IsReadOnly() const override;
    Ptr<BaseComponent> GetComponent(const void* ptr) const override;
    void SetComponent(void* ptr, BaseComponent* value) const override;
    const void* Get(const void* ptr) const override;
    void GetCopy(const void* ptr, void* dest) const override;
    void Set(void* ptr, const void* value) const override;
    //@}

private:
    typedef RemoveConst<T> TT;
    uint32_t mOffset;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Static array property in a class
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, uint32_t N>
class TypePropertyArray: public TypeProperty
{
public:
    /// Constructor
    TypePropertyArray(Symbol name, uint32_t offset);

    /// From TypeProperty
    //@{
    void* GetContent(const void* ptr) const override;
    bool IsReadOnly() const override;
    const void* Get(const void* ptr) const override;
    //@}

private:
    uint32_t mOffset;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Property defined by getter and setter functions of a class
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class C, class T>
class TypePropertyFunction: public TypeProperty
{
private:
    struct ByRef
    {
        typedef const T& (C::*GetterFn)() const;
        typedef void (C::*SetterFn)(const T&);

        static const void* GetRef(GetterFn getter, const void* ptr);
        static void GetCopy(GetterFn getter, const void* ptr, void* dest);
    };

    struct ByCopy
    {
        typedef T (C::*GetterFn)() const;
        typedef void (C::*SetterFn)(T);

        static const void* GetRef(GetterFn getter, const void* ptr);
        static void GetCopy(GetterFn getter, const void* ptr, void* dest);
    };

public:
    typedef If<IsBestByCopy<T>::Result, ByCopy, ByRef> Helper;
    typedef typename Helper::GetterFn GetterFn;
    typedef typename Helper::SetterFn SetterFn;

    /// Constructor
    TypePropertyFunction(Symbol name, GetterFn getter, SetterFn setter);

    /// From TypeProperty
    //@{
    void* GetContent(const void* ptr) const override;
    bool IsReadOnly() const override;
    Ptr<BaseComponent> GetComponent(const void* ptr) const override;
    void SetComponent(void* ptr, BaseComponent* value) const override;
    const void* Get(const void* ptr) const override;
    void Get(const void* ptr, void* dest) const override;
    void Set(void* ptr, const void* value) const override;
    //@}

private:
    GetterFn mGetter;
    SetterFn mSetter;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Property holding a member delegate
////////////////////////////////////////////////////////////////////////////////////////////////////
class TypePropertyOffsetEvent: public TypeProperty
{
public:
    /// Constructor
    TypePropertyOffsetEvent(Symbol name, uint32_t offset);

    /// From TypeProperty
    //@{
    void* GetContent(const void* ptr) const override;
    bool IsReadOnly() const override;
    const void* Get(const void* ptr) const override;
    //@}

private:
    uint32_t mOffset;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Property holding a getter delegate
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class C>
class TypePropertyFunctionEvent: public TypeProperty
{
public:
    /// Constructor
    typedef void* (C::*GetterFn)();
    TypePropertyFunctionEvent(Symbol name, GetterFn getter);

    /// From TypeProperty
    //@{
    void* GetContent(const void* ptr) const override;
    bool IsReadOnly() const override;
    const void* Get(const void* ptr) const override;
    //@}

private:
    GetterFn mGetter;
};

}

#include <NsCore/TypePropertyImpl.inl>

#endif
