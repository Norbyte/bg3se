////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_NULLABLECONVERTER_H__
#define __CORE_NULLABLECONVERTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionImplementEmpty.h>
#include <NsCore/TypeConverter.h>
#include <NsCore/TypeConverterApi.h>
#include <NsCore/Nullable.h>
#include <NsCore/IdOf.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for nullable value converters
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API BaseNullableConverter: public TypeConverter
{
public:
    BaseNullableConverter(const Type* type, TypeConverter* converter);

    /// From TypeConverter
    //@{
    bool CanConvertFrom(const Type* type) const override;
    bool CanConvertTo(const Type* type) const override;
    bool TryConvertFrom(BaseComponent* object, Ptr<BaseComponent>& result) const override;
    bool TryConvertFromString(const char* str, Ptr<BaseComponent>& result) const override;
    bool TryConvertTo(BaseComponent* object, const Type* type, 
        Ptr<BaseComponent>& result) const override;
    bool TryConvertToString(BaseComponent* object, BaseString& result) const override;
    //@}

private:
    const Type* mInnerType;
    Ptr<TypeConverter> mInnerConverter;

    NS_DECLARE_REFLECTION(BaseNullableConverter, TypeConverter)
};

NS_WARNING_POP

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Converter for nullable values
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class NullableConverter: public BaseNullableConverter
{
public:
    inline NullableConverter();
    inline NullableConverter(TypeConverter* converter);

    NS_IMPLEMENT_INLINE_REFLECTION_(NullableConverter, BaseNullableConverter,
        IdOf("Converter<", IdOf<Nullable<T>>(), ">"))
};

}

#include "NullableConverter.inl"


#endif
