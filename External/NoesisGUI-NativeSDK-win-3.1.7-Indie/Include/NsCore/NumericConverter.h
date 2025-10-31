////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_NUMERICCONVERTER_H__
#define __CORE_NUMERICCONVERTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/TypeConverter.h>
#include <NsCore/TypeConverterApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for numeric converters
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API BaseNumericConverter: public TypeConverter
{
public:
    BaseNumericConverter(const Type* type);

    /// From TypeConverter
    //@{
    bool CanConvertFrom(const Type* type) const override;
    bool CanConvertTo(const Type* type) const override;
    bool TryConvertFrom(BaseComponent* object, Ptr<BaseComponent>& result) const override;
    bool TryConvertFromString(const char* str, Ptr<BaseComponent>& result) const override;
    bool TryConvertTo(BaseComponent* object, const Type* type, 
        Ptr<BaseComponent>& result) const override;
     //@}

private:
    virtual bool Parse(const char* str, Ptr<BaseComponent>& result) const = 0;

private:
    const Type* mType;

    NS_DECLARE_REFLECTION(BaseNumericConverter, TypeConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API BooleanConverter: public BaseNumericConverter
{
public:
    BooleanConverter();

private:
    bool Parse(const char* str, Ptr<BaseComponent>& result) const override;

    NS_DECLARE_REFLECTION(BooleanConverter, BaseNumericConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API Int8Converter: public BaseNumericConverter
{
public:
    Int8Converter();

private:
    bool Parse(const char* str, Ptr<BaseComponent>& result) const override;

    NS_DECLARE_REFLECTION(Int8Converter, BaseNumericConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API Int16Converter: public BaseNumericConverter
{
public:
    Int16Converter();

private:
    bool Parse(const char* str, Ptr<BaseComponent>& result) const override;

    NS_DECLARE_REFLECTION(Int16Converter, BaseNumericConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API Int32Converter: public BaseNumericConverter
{
public:
    Int32Converter();

private:
    bool Parse(const char* str, Ptr<BaseComponent>& result) const override;

    NS_DECLARE_REFLECTION(Int32Converter, BaseNumericConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API Int64Converter: public BaseNumericConverter
{
public:
    Int64Converter();

private:
    bool Parse(const char* str, Ptr<BaseComponent>& result) const override;

    NS_DECLARE_REFLECTION(Int64Converter, BaseNumericConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API UInt8Converter: public BaseNumericConverter
{
public:
    UInt8Converter();

private:
    bool Parse(const char* str, Ptr<BaseComponent>& result) const override;

    NS_DECLARE_REFLECTION(UInt8Converter, BaseNumericConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API UInt16Converter: public BaseNumericConverter
{
public:
    UInt16Converter();

private:
    bool Parse(const char* str, Ptr<BaseComponent>& result) const override;

    NS_DECLARE_REFLECTION(UInt16Converter, BaseNumericConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API UInt32Converter: public BaseNumericConverter
{
public:
    UInt32Converter();

private:
    bool Parse(const char* str, Ptr<BaseComponent>& result) const override;

    NS_DECLARE_REFLECTION(UInt32Converter, BaseNumericConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API UInt64Converter: public BaseNumericConverter
{
public:
    UInt64Converter();

private:
    bool Parse(const char* str, Ptr<BaseComponent>& result) const override;

    NS_DECLARE_REFLECTION(UInt64Converter, BaseNumericConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API SingleConverter: public BaseNumericConverter
{
public:
    SingleConverter();

private:
    bool Parse(const char* str, Ptr<BaseComponent>& result) const override;

    NS_DECLARE_REFLECTION(SingleConverter, BaseNumericConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API DoubleConverter: public BaseNumericConverter
{
public:
    DoubleConverter();

private:
    bool Parse(const char* str, Ptr<BaseComponent>& result) const override;

    NS_DECLARE_REFLECTION(DoubleConverter, BaseNumericConverter)
};

}

#endif
