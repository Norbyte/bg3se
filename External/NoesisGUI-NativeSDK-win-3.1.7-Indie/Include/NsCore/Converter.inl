////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Boxing.h>
#include <NsCore/String.h>


namespace Noesis
{
namespace Impl
{

typedef Int2Type<0> IsStruct;
typedef Int2Type<1> IsComponent;

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool TryConvertFromString(const char* str, IsStruct, Ptr<BaseComponent>& result)
{
    T obj;
    if (T::TryParse(str, obj))
    {
        result = Boxing::Box<T>(obj);
        return true;
    }
 
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool TryConvertFromString(const char* str, IsComponent, Ptr<BaseComponent>& result)
{
    Ptr<T> obj;
    if (T::TryParse(str, obj))
    {
        result = obj;
        return true;
    }

    return false;
}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool Converter<T>::CanConvertFrom(const Type* type) const
{
    return type == TypeOf<String>();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
bool Converter<T>::TryConvertFromString(const char* str, Ptr<BaseComponent>& result) const
{
    if (str == 0)
    {
        return false;
    }

    return Impl::TryConvertFromString<T>(str, Int2Type<IsRefCounted<T>::Result>(), result);
}

}