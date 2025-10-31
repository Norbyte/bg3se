////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TREEVIEW_H__
#define __GUI_TREEVIEW_H__


#include <NsCore/Noesis.h>
#include <NsGui/ItemsControl.h>


namespace Noesis
{

class TreeViewItem;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that displays hierarchical data in a tree structure that has items that
/// can expand and collapse.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.treeview.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TreeView: public ItemsControl
{
public:
    TreeView();
    ~TreeView();

    /// Gets the selected item in a TreeView
    BaseComponent* GetSelectedItem() const;

    /// Occurs when a new item is selected in the tree
    typedef RoutedPropertyChangedEventHandler<Ptr<BaseComponent>>::Handler SelectedItemChangedT;
    UIElement::RoutedEvent_<SelectedItemChangedT> SelectedItemChanged();

    // Used by TreeViewItem implementation
    //@{
    virtual void ItemClicked(TreeViewItem* tvi);
    void ResetSelectedItem();
    //@}

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* SelectedItemProperty;
    //@}

    // Routed Events
    //@{
    /// Occurs when the SelectedItem changes.
    static const RoutedEvent* SelectedItemChangedEvent;
    //@}

protected:
    // Notifies inheritors about SelectedItem property changes and raises SelectedItemChanged event
    virtual void OnSelectedItemChanged(const RoutedPropertyChangedEventArgs<Ptr<BaseComponent>>& e);

    /// From ItemsControl
    //@{
    Ptr<DependencyObject> GetContainerForItemOverride() const override;
    bool IsItemItsOwnContainerOverride(BaseComponent* item) const override;
    void OnItemsChanged(const NotifyCollectionChangedEventArgs& args) override;
    //@}

    // From Control
    //@{
    void OnIsFocusEngagedChanged(bool engaged) override;
    //@}

    /// From UIElement
    //@{
    void OnGotFocus(const RoutedEventArgs& e) override;
    //@}

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& e) override;
    //@}

private:
    void TryFocusSelected();

private:
    Ptr<TreeViewItem> mSelectedContainer;

    NS_DECLARE_REFLECTION(TreeView, ItemsControl)
};

NS_WARNING_POP

}


#endif
