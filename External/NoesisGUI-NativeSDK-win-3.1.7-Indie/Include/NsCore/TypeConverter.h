////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPECONVERTER_H__
#define __CORE_TYPECONVERTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/TypeConverterApi.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/StringFwd.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class Type;
class BoxedValue;
template<class T> class Ptr;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a unified way of converting types of values to other types
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_TYPECONVERTER_API TypeConverter: public BaseComponent
{
public:
    /// Returns the converter registered for the given type
    static TypeConverter* Get(const Type* type);

    /// Returns whether this converter can convert from the given type
    virtual bool CanConvertFrom(const Type* type) const;

    /// Returns whether this converter can convert to the given type
    virtual bool CanConvertTo(const Type* type) const;

    /// Converts the given value to the type of this converter
    virtual bool TryConvertFrom(BaseComponent* object, Ptr<BaseComponent>& result) const;

    /// Converts the specified text to an object
    virtual bool TryConvertFromString(const char* str, Ptr<BaseComponent>& result) const;

    /// Converts the given value object to the specified type
    virtual bool TryConvertTo(BaseComponent* object, const Type* type,
        Ptr<BaseComponent>& result) const;

    /// Converts the specified value to a string representation
    virtual bool TryConvertToString(BaseComponent* object, BaseString& result) const;

    NS_DECLARE_REFLECTION(TypeConverter, BaseComponent)
};

}

#endif
