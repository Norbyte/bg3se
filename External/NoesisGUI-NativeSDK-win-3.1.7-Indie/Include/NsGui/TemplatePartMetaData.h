////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEMPLATEPARTMETADATA_H__
#define __GUI_TEMPLATEPARTMETADATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/TypeMetaData.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/String.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an attribute that is applied to the class definition to identify the types of the
/// named parts that are used for templating.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TemplatePartMetaData: public TypeMetaData
{
public:
    TemplatePartMetaData(const char* name, const TypeClass* type);

    /// Gets the pre-defined name of the part
    const char* GetPartName() const;

    /// Gets the type of the named part this attribute is identifying
    const TypeClass* GetPartType() const;

private:
    String mPartName;
    const TypeClass* mPartType;

    NS_DECLARE_REFLECTION(TemplatePartMetaData, TypeMetaData)
};

NS_WARNING_POP

}


#endif
