////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TOOLBAROVERFLOWPANEL_H__
#define __GUI_TOOLBAROVERFLOWPANEL_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Panel.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Used to arrange overflow ToolBar items.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.toolbaroverflowpanel.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ToolBarOverflowPanel: public Panel
{
public:
    ToolBarOverflowPanel();

    /// Gets or sets the recommended width for this panel before items flow to the next line
    //@{
    float GetWrapWidth() const;
    void SetWrapWidth(float wrapWidth);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* WrapWidthProperty;
    //@}

protected:
    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize);
    Size ArrangeOverride(const Size& finalSize);
    //@}

    /// From Panel
    //@{
    Ptr<UIElementCollection> CreateChildrenCollection(FrameworkElement* logicalParent);
    //@}

private:
    void ArrangeLine(uint32_t start, uint32_t end, float y, float height);

private:
    float mCalculatedWrapWidth;

    NS_DECLARE_REFLECTION(ToolBarOverflowPanel, Panel)
};

}


#endif
