////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TYPECONVERTERMETADATA_H__
#define __GUI_TYPECONVERTERMETADATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/TypeMetaData.h>
#include <NsCore/Symbol.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypeConverterMetaData. Stores information about the proper TypeConverter for an object
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TypeConverterMetaData: public TypeMetaData
{
public:
    /// Constructor
    TypeConverterMetaData(const char* typeId);

    /// Gets converter type Id
    Symbol GetTypeId() const;

private:
    Symbol mTypeId;

    NS_DECLARE_REFLECTION(TypeConverterMetaData, TypeMetaData)
};

NS_WARNING_POP

}

#endif
