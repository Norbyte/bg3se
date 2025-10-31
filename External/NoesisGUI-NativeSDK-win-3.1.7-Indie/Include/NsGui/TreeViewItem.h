////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TREEVIEWITEM_H__
#define __GUI_TREEVIEWITEM_H__


#include <NsCore/Noesis.h>
#include <NsGui/HeaderedItemsControl.h>


namespace Noesis
{

class TreeView;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements a selectable item in a TreeView control.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.treeviewitem.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TreeViewItem: public HeaderedItemsControl
{
public:
    TreeViewItem();
    ~TreeViewItem();

    /// Gets or sets whether the nested items in a TreeViewItem are expanded or collapsed
    //@{
    bool GetIsExpanded() const;
    void SetIsExpanded(bool value);
    //@}

    /// Gets or sets whether a TreeViewItem control is selected
    //@{
    bool GetIsSelected() const;
    void SetIsSelected(bool value);
    //@}

    /// Gets a value that indicates whether the containing TreeView has keyboard focus
    bool GetIsSelectionActive() const;

    /// Occurs when the IsExpanded property changes from true to false
    UIElement::RoutedEvent_<RoutedEventHandler> Collapsed();

    /// Occurs when the IsExpanded property changes from false to true
    UIElement::RoutedEvent_<RoutedEventHandler> Expanded();

    /// Occurs when the IsSelected property of a TreeViewItem changes from false to true
    UIElement::RoutedEvent_<RoutedEventHandler> Selected();

    /// Occurs when the IsSelected property of a TreeViewItem changes from true to false
    UIElement::RoutedEvent_<RoutedEventHandler> Unselected();

public:
    static const DependencyProperty* IsExpandedProperty;
    static const DependencyProperty* IsSelectedProperty;
    static const DependencyProperty* IsSelectionActiveProperty;
    
    static const RoutedEvent* CollapsedEvent;
    static const RoutedEvent* ExpandedEvent;
    static const RoutedEvent* SelectedEvent;
    static const RoutedEvent* UnselectedEvent;
    
protected:
    virtual void OnCollapsed(const RoutedEventArgs& e);
    virtual void OnExpanded(const RoutedEventArgs& e);
    virtual void OnSelected(const RoutedEventArgs& e);
    virtual void OnUnselected(const RoutedEventArgs& e);

    // From ItemsControl
    //@{
    Ptr<DependencyObject> GetContainerForItemOverride() const override;
    bool IsItemItsOwnContainerOverride(BaseComponent* item) const override;
    void OnItemsChanged(const NotifyCollectionChangedEventArgs& args) override;
    //@}

    // From Control
    //@{
    void UpdateVisualStates() override;
    //@}

    // From FrameworkElement
    //@{
    void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot) override;
    //@}

    // From UIElement
    //@{
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e) override;
    void OnGotFocus(const RoutedEventArgs& e) override;
    void OnPreviewKeyDown(const KeyEventArgs& e) override;
    void OnKeyDown(const KeyEventArgs& e) override;
    //@}

    // From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

private:
    TreeView* GetSourceTreeView() const;

    FrameworkElement* GetHeaderHost() const;

    bool BringHeaderIntoView();

private:
    Ptr<FrameworkElement> mHeaderHost;

    NS_DECLARE_REFLECTION(TreeViewItem, HeaderedItemsControl)
};

NS_WARNING_POP

}


#endif
