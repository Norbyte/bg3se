////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_NAMESCOPEPROPERTYMETADATA_H__
#define __GUI_NAMESCOPEPROPERTYMETADATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/TypeMetaData.h>
#include <NsCore/Symbol.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// The component that contains this metadata can be registered into a NameScope using the value of
/// the property specified by the metadata
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API NameScopePropertyMetaData: public TypeMetaData
{
public:
    NameScopePropertyMetaData(const char* nameScopeProperty);
    NameScopePropertyMetaData(Symbol nameScopeProperty);
    ~NameScopePropertyMetaData();

    Symbol GetNameScopeProperty() const;

private:
    Symbol mNameScopeProperty;

    NS_DECLARE_REFLECTION(NameScopePropertyMetaData, TypeMetaData)
};

NS_WARNING_POP

}


#endif
