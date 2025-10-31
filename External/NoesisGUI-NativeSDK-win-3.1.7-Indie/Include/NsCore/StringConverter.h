////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_STRINGCONVERTER_H__
#define __CORE_STRINGCONVERTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/TypeConverterApi.h>
#include <NsCore/TypeConverter.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Converter for string
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API StringConverter: public TypeConverter
{
public:
    /// From TypeConverter
    //@{
    bool CanConvertFrom(const Type* type) const override;
    bool TryConvertFrom(BaseComponent* object, Ptr<BaseComponent>& result) const override;
    bool TryConvertFromString(const char* str, Ptr<BaseComponent>& result) const override;
    //@}

    NS_DECLARE_REFLECTION(StringConverter, TypeConverter)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Converter for const char*
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API StringCharConverter: public StringConverter
{
public:
    NS_DECLARE_REFLECTION(StringCharConverter, StringConverter)
};

}

#endif
