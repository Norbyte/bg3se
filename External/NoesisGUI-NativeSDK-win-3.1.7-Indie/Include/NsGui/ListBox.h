////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_LISTBOX_H__
#define __GUI_LISTBOX_H__


#include <NsCore/Noesis.h>
#include <NsGui/Selector.h>
#include <NsCore/ReflectionDeclareEnum.h>


namespace Noesis
{

class ListBoxItem;
class ListBoxTest;
struct NotifyCollectionChangedEventArgs;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Contains a list of selectable items.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.listbox.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ListBox: public Selector
{
public:
    ListBox();

    /// Gets a list of the currently selected items
    SelectedItemsCollection* GetSelectedItems() const;

    /// Gets or sets the selection behavior for a ListBox
    //@{
    SelectionMode GetSelectionMode() const;
    void SetSelectionMode(SelectionMode mode);
    //@}

    /// Selects all the items
    void SelectAll();

    /// Clears all of the selected items
    void UnselectAll();

    /// Causes the item to scroll into view. If it is not visible, it is aligned either at the
    /// top or bottom of the viewport
    void ScrollIntoView(BaseComponent* item);

    // From ItemsControl
    //@{
    Ptr<Panel> GetDefaultItemsPanel() const override;
    //@}

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* SelectedItemsProperty;
    static const DependencyProperty* SelectionModeProperty;
    //@}
    
protected:
    // From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    // From Selector
    //@{
    void OnSelectionChanged(const SelectionChangedEventArgs& args) override;
    bool IsMultiSelectEnabled() const override;
    //@}

    // From ItemsControl
    //@{
    Ptr<DependencyObject> GetContainerForItemOverride() const override;
    bool IsItemItsOwnContainerOverride(BaseComponent* item) const override;
    void OnItemsChanged(const NotifyCollectionChangedEventArgs& args) override;
    //@}

private:
    void SingleSelection(ListBoxItem* item);
    void ToggleSelection(ListBoxItem* item, bool multiple);
    void RangeSelection(ListBoxItem* item);

    /// Used by ListBoxItem implementation
    friend class ListBoxItem;
    void ItemSelected(ListBoxItem* lbi);
    void ItemUnselected(ListBoxItem* lbi);
    void ItemClicked(ListBoxItem* lbi, bool toggleSelection);

    // Used by testing
    friend class ListBoxTest;
    void ItemClicked(ListBoxItem* lbi, bool toggleSelection, bool shiftKeyDown,
        bool ctrlKeyDown, bool keyboardFocused = false);

private:
    int32_t mFirstPressedItem;

    NS_DECLARE_REFLECTION(ListBox, Selector)
};

}


#endif
