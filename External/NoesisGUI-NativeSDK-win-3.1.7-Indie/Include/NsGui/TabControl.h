////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TABCONTROL_H__
#define __GUI_TABCONTROL_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Selector.h>


namespace Noesis
{

class TabItem;
class ContentPresenter;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that contains multiple items that share the same space on the screen.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.tabcontrol.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TabControl: public Selector
{
public:
    TabControl();
    ~TabControl();

    /// Gets or sets the DataTemplate to apply to any TabItem that does not have a ContentTemplate 
    // or ContentTemplateSelector property defined
    //@{
    DataTemplate* GetContentTemplate() const;
    void SetContentTemplate(DataTemplate* value);
    //@}

    /// Gets or sets a DataTemplateSelector that provides custom logic for choosing the template 
    /// that is used to display the content of the control
    //@{
    DataTemplateSelector* GetContentTemplateSelector() const;
    void SetContentTemplateSelector(DataTemplateSelector* selector);
    //@}

    /// Gets the content of the currently selected TabItem
    BaseComponent* GetSelectedContent() const;

    /// Gets the DataTemplate of the currently selected item
    DataTemplate* GetSelectedContentTemplate() const;

    /// Gets the DataTemplateSelector of the currently selected item
    DataTemplateSelector* GetSelectedContentTemplateSelector() const;

    /// Gets or sets how tab headers align relative to the tab content
    //@{
    Dock GetTabStripPlacement() const;
    void SetTabStripPlacement(Dock dock);
    //@}

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* ContentTemplateProperty;
    static const DependencyProperty* ContentTemplateSelectorProperty;
    static const DependencyProperty* SelectedContentProperty;
    static const DependencyProperty* SelectedContentTemplateProperty;
    static const DependencyProperty* SelectedContentTemplateSelectorProperty;
    static const DependencyProperty* TabStripPlacementProperty;
    //@}

protected:
    /// From UIElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    void OnKeyDown(const KeyEventArgs& args) override;
    void OnPreviewGotKeyboardFocus(const KeyboardFocusChangedEventArgs& args) override;
    //@}

    /// From Control
    //@{
    void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot) override;
    void UpdateVisualStates() override;
    //@}

    /// From ItemsControl
    //@{
    Ptr<DependencyObject> GetContainerForItemOverride() const override;
    bool IsItemItsOwnContainerOverride(BaseComponent* item) const override;
    //@}

    /// From Selector
    //@{
    void OnSelectionChanged(const SelectionChangedEventArgs& args) override;
    //@}

private:
    // Updates selected content host so it points to the new selected tab
    void UpdateSelectedContent(TabItem* selectedTab);

    TabItem* SelectedTab() const;
    TabItem* FindNextTabItem(int startIndex, int direction);

    friend class TabItem;
    void SelectTab(TabItem* tab);
    ContentPresenter* GetSelectedContentHost() const;

private:
    Ptr<ContentPresenter> mSelectedContentHost;

    NS_DECLARE_REFLECTION(TabControl, Selector)
};

NS_WARNING_POP

}


#endif
