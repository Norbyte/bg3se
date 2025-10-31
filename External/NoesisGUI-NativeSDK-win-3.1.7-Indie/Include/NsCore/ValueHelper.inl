////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/CompilerTools.h>
#include <NsCore/TypeOf.h>


namespace Noesis
{
namespace Impl
{

NS_CORE_KERNEL_API String ToString(bool value);
NS_CORE_KERNEL_API String ToString(char value);
NS_CORE_KERNEL_API String ToString(uint8_t value);
NS_CORE_KERNEL_API String ToString(uint16_t value);
NS_CORE_KERNEL_API String ToString(uint32_t value);
NS_CORE_KERNEL_API String ToString(uint64_t value);
NS_CORE_KERNEL_API String ToString(int8_t value);
NS_CORE_KERNEL_API String ToString(int16_t value);
NS_CORE_KERNEL_API String ToString(int32_t value);
NS_CORE_KERNEL_API String ToString(int64_t value);
NS_CORE_KERNEL_API String ToString(float value);
NS_CORE_KERNEL_API String ToString(double value);
NS_CORE_KERNEL_API String ToString(const String& value);
NS_CORE_KERNEL_API String ToString(Symbol value);

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> String ToString(const T& value)
{
    return value.ToString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> String ToString(const T& value, Int2Type<0>)
{ 
    return Impl::ToString(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> String ToString(const T& value, Int2Type<1>)
{ 
    Symbol name;
    TypeOf<T>()->HasValue((int)value, name);
    return name.Str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> String ToString(const T& value, Int2Type<2>)
{
    return value != nullptr ? Impl::ToString(*value) : "";
}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> String ToString(const T& value)
{
    enum { ValueTypeId = IsPointer<T>::Result ? 2 : IsEnum<T>::Result ? 1 : 0 };
    return Impl::ToString(value, Int2Type<ValueTypeId>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> inline bool Equals(const T& left, const T& right)
{
    return left == right;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Equals(float left, float right)
{
    bool isLeftNaN = IsNaN(left);
    bool isRightNaN = IsNaN(right);
    return isLeftNaN ? isRightNaN : isRightNaN ? false : left == right;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Equals(double left, double right)
{
    bool isLeftNaN = IsNaN(left);
    bool isRightNaN = IsNaN(right);
    return isLeftNaN ? isRightNaN : isRightNaN ? false : left == right;
}

}
