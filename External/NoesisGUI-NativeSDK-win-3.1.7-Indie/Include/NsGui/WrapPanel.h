////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_WRAPPANEL_H__
#define __GUI_WRAPPANEL_H__


#include <NsCore/Noesis.h>
#include <NsGui/Panel.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Positions child elements in sequential position from left to right, breaking content to the
/// next line at the edge of the containing box.
///
/// Subsequent ordering happens sequentially from top to bottom or from right to left, depending on
/// the value of the Orientation property.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.wrappanel.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API WrapPanel: public Panel
{
public:
    WrapPanel();

    /// Gets or sets a value that specifies the width of all items contained in the panel
    //@{
    float GetItemWidth() const;
    void SetItemWidth(float itemWidth);
    //@}

    /// Gets or sets a value that specifies the height of all items contained in the panel
    //@{
    float GetItemHeight() const;
    void SetItemHeight(float itemHeight);
    //@}

    /// Gets or sets orientation of children elements
    //@{
    Orientation GetOrientation() const;
    void SetOrientation(Orientation orientation);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ItemWidthProperty;
    static const DependencyProperty* ItemHeightProperty;
    static const DependencyProperty* OrientationProperty;
    //@}

protected:
    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    Size MeasureHorizontal(const Size& availableSize);
    Size MeasureVertical(const Size& availableSize);

    void ArrangeHorizontal(const Size& finalSize);
    void ArrangeVertical(const Size& finalSize);

    void ArrangeLine(uint32_t start, uint32_t end, float y, float height, bool autoW, float itemW);
    void ArrangeColumn(uint32_t start, uint32_t end, float x, float width, bool autoH, float itemH);

    NS_DECLARE_REFLECTION(WrapPanel, Panel)
};

}


#endif
