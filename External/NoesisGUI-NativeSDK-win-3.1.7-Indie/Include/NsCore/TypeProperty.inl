////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Error.h>


namespace Noesis
{

template<class T> struct TypeTag;
template<class T> struct TypeOfHelper;
template<class T> const typename TypeOfHelper<T>::ReturnType* TypeOf();
typedef Int2Type<0> ByRef;
typedef Int2Type<1> ByCopy;

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Symbol TypeProperty::GetName() const
{
    return mName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Type* TypeProperty::GetContentType() const
{
    return mType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> T* FindMeta(const TypeProperty* type)
{
    return (T*)(type->FindMeta(T::StaticGetClassType((TypeTag<T>*)nullptr)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> typename Param<T>::Type PropGet(const TypeProperty* type, const void* ptr, ByRef)
{
    return *reinterpret_cast<const T*>(type->Get(ptr));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T> typename Param<T>::Type PropGet(const TypeProperty* type, const void* ptr, ByCopy)
{
    T storage;
    type->GetCopy(ptr, &storage);
    return storage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> typename Param<T>::Type PropGet(const TypeProperty* type, const void* ptr)
{
    NS_ASSERT(TypeOf<T>() == type->GetContentType());
    return PropGet<T>(type, ptr, Int2Type<IsBestByCopy<T>::Result>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> void PropSet(const TypeProperty* type, void* ptr, typename Param<T>::Type value)
{
    NS_ASSERT(TypeOf<T>() == type->GetContentType());
    type->Set(ptr, &value);
}

}
