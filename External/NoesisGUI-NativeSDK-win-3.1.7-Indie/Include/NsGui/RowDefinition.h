////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ROWDEFINITION_H__
#define __GUI_ROWDEFINITION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BaseDefinition.h>


namespace Noesis
{

class GridLength;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines row-specific properties that apply to Grid elements.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.rowdefinition.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API RowDefinition: public BaseDefinition
{
public:
    RowDefinition();

    /// Gets or sets column height defined by a GridLength value. Default is 1*
    //@{
    const GridLength& GetHeight() const;
    void SetHeight(const GridLength& height);
    //@}

    /// Gets or sets column minimum height in pixels. Default value is 0
    //@{
    float GetMinHeight() const;
    void SetMinHeight(float minHeight);
    //@}

    /// Gets or sets column maximum height in pixels. Default value is positive Infinity
    //@{
    float GetMaxHeight() const;
    void SetMaxHeight(float maxHeight);
    //@}

    /// Gets a value that represents the calculated height of the RowDefinition
    float GetActualHeight() const;

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* HeightProperty;
    static const DependencyProperty* MinHeightProperty;
    static const DependencyProperty* MaxHeightProperty;
    //@}

private:
    NS_DECLARE_REFLECTION(RowDefinition, BaseDefinition)
};

}


#endif
