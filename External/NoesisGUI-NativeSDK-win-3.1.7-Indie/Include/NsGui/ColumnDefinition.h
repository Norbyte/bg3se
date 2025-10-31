////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COLUMNDEFINITION_H__
#define __GUI_COLUMNDEFINITION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BaseDefinition.h>


namespace Noesis
{

class GridLength;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines column-specific properties that apply to Grid elements.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.columndefinition.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ColumnDefinition: public BaseDefinition
{
public:
    ColumnDefinition();

    /// Gets or sets column width defined by a GridLength value. Default is 1*
    //@{
    const GridLength& GetWidth() const;
    void SetWidth(const GridLength& width);
    //@}

    /// Gets or sets column minimum width in pixels. Default value is 0
    //@{
    float GetMinWidth() const;
    void SetMinWidth(float minWidth);
    //@}

    /// Gets or sets column maximum width in pixels. Default value is positive Infinity
    //@{
    float GetMaxWidth() const;
    void SetMaxWidth(float maxWidth);
    //@}

    /// Gets a value that represents the actual calculated width of a ColumnDefinition
    float GetActualWidth() const;

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* WidthProperty;
    static const DependencyProperty* MinWidthProperty;
    static const DependencyProperty* MaxWidthProperty;
    //@}

private:
    NS_DECLARE_REFLECTION(ColumnDefinition, BaseDefinition)
};

}

#endif
