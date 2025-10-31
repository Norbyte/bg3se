////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IPROPERTYVALUECONVERTER_H__
#define __GUI_IPROPERTYVALUECONVERTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>


namespace Noesis
{

class DependencyProperty;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Converters that implement this interface use a DependencyProperty to specify the target type.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IPropertyValueConverter: public Interface
{
    /// Gets the property used to specify the target type
    virtual const DependencyProperty* GetProperty() const = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IPropertyValueConverter, Interface)
};

}


#endif
