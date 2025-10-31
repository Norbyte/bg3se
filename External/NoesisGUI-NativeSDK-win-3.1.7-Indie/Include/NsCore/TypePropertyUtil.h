////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPEPROPERTYUTIL_H__
#define __CORE_TYPEPROPERTYUTIL_H__


#include <NsCore/Noesis.h>
#include <NsCore/TypeProperty.h>
#include <NsCore/CompilerTools.h>
#include <NsCore/String.h>
#include <NsCore/Boxing.h>


namespace Noesis
{

typedef Int2Type<0> NonRefCountedType;
typedef Int2Type<1> PtrRefCountedType;
typedef Int2Type<2> PointerRefCountedType;

typedef Int2Type<3> NonCharPointerType;
typedef Int2Type<4> CharPointerType;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Auxiliary functions to get values from a property using boxed values or Ptr objects
////////////////////////////////////////////////////////////////////////////////////////////////////
//@{
template<class T>
Ptr<BaseComponent> GetComponent(const TypeProperty* prop, const void* ptr, NonRefCountedType)
{
    return Boxing::Box(PropGet<T>(prop, ptr));
}

template<class T>
Ptr<BaseComponent> GetComponent(const TypeProperty* prop, const void* ptr, PtrRefCountedType)
{
    return DynamicPtrCast<BaseComponent>(PropGet<T>(prop, ptr));
}

template<class T>
Ptr<BaseComponent> GetComponent(const TypeProperty* prop, const void* ptr, PointerRefCountedType)
{
    return Ptr<BaseComponent>(const_cast<BaseComponent*>(
        DynamicCast<const BaseComponent*>(PropGet<T>(prop, ptr))));
}

template<class T>
Ptr<BaseComponent> GetComponent(const TypeProperty* prop, const void* ptr)
{
    typedef RemovePointer<T> PointedType;
    enum { IsRefCountedType = IsPtr<T>::Result ? 1 : IsRefCounted<PointedType>::Result ? 2 : 0 };
    return GetComponent<T>(prop, ptr, Int2Type<IsRefCountedType>());
}
//@}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Auxiliary functions to set values in a property using boxed values or Ptr objects
////////////////////////////////////////////////////////////////////////////////////////////////////
//@{
template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value, NonCharPointerType)
{
    PropSet<T>(prop, ptr, value != 0 ? Boxing::Unbox<T>(value) : T());
}

template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value, CharPointerType)
{
    PropSet<T>(prop, ptr, value != 0 ? Boxing::Unbox<String>(value).Str() : "");
}

template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value, NonRefCountedType)
{
    SetComponent<T>(prop, ptr, value, Int2Type<(int)IsSame<T, const char*>::Result + 3>());
}

template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value, PtrRefCountedType)
{
    PropSet<T>(prop, ptr, T(DynamicCast<typename T::Type*>(value)));
}

template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value, PointerRefCountedType)
{
    PropSet<T>(prop, ptr, DynamicCast<T>(value));
}

template<class T>
void SetComponent(const TypeProperty* prop, void* ptr, BaseComponent* value)
{
    typedef RemovePointer<T> PointedType;
    enum { IsRefCountedType = IsPtr<T>::Result ? 1 : IsRefCounted<PointedType>::Result ? 2 : 0 };
    SetComponent<T>(prop, ptr, value, Int2Type<IsRefCountedType>());
}
//@}

}

#endif
