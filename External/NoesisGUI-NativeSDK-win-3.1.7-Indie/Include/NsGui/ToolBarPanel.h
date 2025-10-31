////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TOOLBARPANEL_H__
#define __GUI_TOOLBARPANEL_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/StackPanel.h>


namespace Noesis
{

class UIElementCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Arranges ToolBar items inside a ToolBar.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.toolbarpanel.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ToolBarPanel: public StackPanel
{
public:
    ToolBarPanel();
    ~ToolBarPanel();

protected:
    /// From Visual
    //@{
    uint32_t GetVisualChildrenCount() const;
    Visual* GetVisualChild(uint32_t index) const;
    //@}

    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize);
    Size ArrangeOverride(const Size& finalSize);
    //@}

    /// From DependencyObject
    //@{
    void OnInit();
    //@}

    /// From Panel
    //@{
    Ptr<UIElementCollection> CreateChildrenCollection(FrameworkElement* logicalParent);
    void GenerateChildren();
    void OnItemsChangedOverride(BaseComponent* sender, const ItemsChangedEventArgs& e);
    //@}

private:
    Ptr<UIElementCollection> mVisibleChildren;

    NS_DECLARE_REFLECTION(ToolBarPanel, StackPanel)
};

NS_WARNING_POP

}


#endif
