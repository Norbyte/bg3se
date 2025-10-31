////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_ENUMCONVERTER_H__
#define __CORE_ENUMCONVERTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/CompilerTools.h>
#include <NsCore/TypeConverterApi.h>
#include <NsCore/ReflectionImplement.h>
#include <NsCore/TypeConverter.h>
#include <NsCore/IdOf.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for enum converters
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API BaseEnumConverter: public TypeConverter
{
public:
    BaseEnumConverter(const TypeEnum* type);

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
    virtual Ptr<BaseComponent> Box(uint32_t v) const = 0;
    virtual uint32_t Unbox(BaseComponent* value) const = 0;
    virtual bool CanUnbox(BaseComponent* value) const;

private:
    const TypeEnum* mType;

    NS_DECLARE_REFLECTION(BaseEnumConverter, TypeConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Converter for enums
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
class EnumConverter: public BaseEnumConverter
{
    static_assert(IsEnum<T>::Result, "type must be an enum");

public:
    EnumConverter();

private:
    Ptr<BaseComponent> Box(uint32_t v) const override;
    uint32_t Unbox(BaseComponent* value) const override;

    NS_IMPLEMENT_INLINE_REFLECTION_(EnumConverter, BaseEnumConverter, IdOf("Converter<", IdOf<T>(), ">"))
};

}


#include <NsCore/EnumConverter.inl>


#endif
