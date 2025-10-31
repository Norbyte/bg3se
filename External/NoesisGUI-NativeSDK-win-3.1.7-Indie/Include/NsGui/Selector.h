////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SELECTOR_H__
#define __GUI_SELECTOR_H__


#include <NsCore/Noesis.h>
#include <NsGui/ItemsControl.h>
#include <NsGui/RoutedEvent.h>


namespace Noesis
{

class PropertyPath;
template<class T> class Nullable;
struct PathElement;

template<class T> class UICollection;
typedef UICollection<BaseComponent> SelectedItemsCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the Selector.SelectionChanged event.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct SelectionChangedEventArgs: public RoutedEventArgs
{
    typedef Vector<Ptr<BaseComponent>> ItemList;
    ItemList addedItems;
    ItemList removedItems;

    SelectionChangedEventArgs(BaseComponent* source);
};

typedef Delegate<void (BaseComponent*, const SelectionChangedEventArgs&)> SelectionChangedEventHandler;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that allows a user to select items from among its child elements.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.selector.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Selector: public ItemsControl
{
public:
    Selector();
    virtual ~Selector() = 0;

    /// Gets or sets a value that indicates whether an item is selected
    //@{
    static bool GetIsSelected(DependencyObject* element);
    static void SetIsSelected(DependencyObject* element, bool value);
    //@}

    /// Gets a value that indicates whether the keyboard focus is within the Selector
    static bool GetIsSelectionActive(UIElement* element);

    /// Gets or sets a value that indicates whether a Selector should keep the SelectedItem 
    /// synchronized with the current item in the Items property.
    /// Returns true if the *SelectedItem* is always synchronized with the current item in the 
    /// ItemCollection;  false if the *SelectedItem* is never synchronized with the current item;
    /// or null if the *SelectedItem* is synchronized with the current item only if the Selector 
    /// uses a CollectionView. The default value is null
    //@{
    const Nullable<bool>& GetIsSynchronizedWithCurrentItem() const;
    void SetIsSynchronizedWithCurrentItem(const Nullable<bool>& value);
    //@}

    /// Gets or sets the index of the first item in the current selection or returns negative one 
    /// (-1) if the selection is empty
    //@{
    int32_t GetSelectedIndex() const;
    void SetSelectedIndex(int32_t index);
    //@}

    /// Gets or sets the first item in the current selection or returns null if the selection is 
    /// empty
    //@{
    Noesis::BaseComponent* GetSelectedItem() const;
    void SetSelectedItem(Noesis::BaseComponent* item);
    //@}

    /// Gets or sets the value of the *SelectedItem*, obtained by using *SelectedValuePath*
    //@{
    Noesis::BaseComponent* GetSelectedValue() const;
    void SetSelectedValue(Noesis::BaseComponent* selectedValue);
    //@}

    /// Gets or sets the path that is used to get the *SelectedValue* from the *SelectedItem*
    //@{
    const char* GetSelectedValuePath() const;
    void SetSelectedValuePath(const char* selectedValuePath);
    //@}

    /// Occurs when the selection of a Selector changes
    UIElement::RoutedEvent_<SelectionChangedEventHandler> SelectionChanged();

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* IsSelectedProperty; // Attached
    static const DependencyProperty* IsSelectionActiveProperty; // Attached
    static const DependencyProperty* IsSynchronizedWithCurrentItemProperty;
    static const DependencyProperty* SelectedIndexProperty;
    static const DependencyProperty* SelectedItemProperty;
    static const DependencyProperty* SelectedValueProperty;
    static const DependencyProperty* SelectedValuePathProperty;
    //@}

    /// Routed events
    //@{
    static const RoutedEvent* SelectedEvent; // Attached
    static const RoutedEvent* SelectionChangedEvent;
    static const RoutedEvent* UnselectedEvent; // Attached
    //@}

protected:
    // Called when the selection changes
    virtual void OnSelectionChanged(const SelectionChangedEventArgs& args);

    /// From ItemsControl
    //@{
    void OnItemsSourceChanged(BaseComponent* oldSource, BaseComponent* newSource) override;
    void OnCurrentItemChanged() override;
    void OnItemsChanged(const NotifyCollectionChangedEventArgs& args) override;
    void OnContainersGenerated() override;
    void ClearContainerForItemOverride(DependencyObject* container,
        BaseComponent* item) const override;
    //@}

    /// From Control
    //@{
    void OnIsFocusEngagedChanged(bool engaged) override;
    //@}

    /// From UIElement
    //@{
    void OnGotFocus(const RoutedEventArgs& e) override;
    void OnIsKeyboardFocusWithinChanged(const DependencyPropertyChangedEventArgs& e) override;
    //@}

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    SelectedItemsCollection* GetInternalSelectedItems() const;

    typedef Vector<int32_t> IndicesVector;
    const IndicesVector& GetInternalSelectedIndices() const;

    void InternalSelectAll();
    void InternalUnselectAll();
    void InternalSelectRange(int32_t start, int32_t end);

    virtual bool IsMultiSelectEnabled() const;

protected:
    static const DependencyProperty* InternalSelectedItemsProperty;

    enum
    {
        IsUpdatingSelection = ItemsControl::LastFlag * 2,
        IsSynchronizingCurrent = IsUpdatingSelection * 2,
        IsSelectingItem = IsSynchronizingCurrent * 2,

        LastFlag = IsSelectingItem
    };

private:
    void SynchronizeWithCurrent(int32_t currentIndex);
    BaseComponent* SelectItem(BaseComponent* selectedValue);
    void UpdateSelectedValue(BaseComponent* selectedItem);

    void TryFocusSelected();

    void UpdateSingleSelectedList(SelectionChangedEventArgs& e,
        BaseComponent* selectedItem, int32_t selectedIndex);
    void UpdateSingleSelectedAndRaiseEvent(const SelectionChangedEventArgs& e,
        int32_t selectedIndex);
    void UpdateSingleSelectedAndRaiseEvent(const SelectionChangedEventArgs& e,
        BaseComponent* selectedItem, int32_t selectedIndex,
        bool raiseSelectionChanged = true);

    void OnItemSelected(SelectionChangedEventArgs& e, BaseComponent* item, int32_t index);
    void OnItemUnselected(SelectionChangedEventArgs& e, BaseComponent* item, int32_t index);

    void OnSelectedIndexChanged(int32_t oldIndex, int32_t newIndex);
    void OnSelectedItemChanged(BaseComponent* oldItem, BaseComponent* newItem);

    void OnSelectedItemsChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& e);

    void OnSelectedItemAdded(SelectionChangedEventArgs& args, BaseComponent* item,
        int32_t index);
    void OnSelectedItemRemoved(SelectionChangedEventArgs& args, BaseComponent* item,
        int32_t index, int32_t& selectedIndex);

    static bool CoerceSelectedValue(const DependencyObject* object,
        const void* value, void* coercedValue);

private:
    friend class ListBoxTest;

    // Used to resolve SelectedValuePath
    Ptr<PropertyPath> mPath;

    // Selected items indices
    IndicesVector mSelectedIndices;

    NS_DECLARE_REFLECTION(Selector, ItemsControl)
};

NS_WARNING_POP

}

#include <NsGui/Selector.inl>

#endif
