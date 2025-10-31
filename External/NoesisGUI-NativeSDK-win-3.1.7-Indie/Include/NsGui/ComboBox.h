////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMBOBOX_H__
#define __GUI_COMBOBOX_H__


#include <NsCore/Noesis.h>
#include <NsGui/Selector.h>


namespace Noesis
{

class TextBox;
class ComboBoxItem;
class Popup;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a selection control with a drop-down list that can be shown or hidden by clicking
/// the arrow on the control.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.combobox.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ComboBox: public Selector
{
public:
    ComboBox();
    ~ComboBox();

    /// Gets or sets a value that indicates whether the drop-down for a combo box is currently open
    //@{
    bool GetIsDropDownOpen() const;
    void SetIsDropDownOpen(bool value);
    //@}

    /// Gets or sets a value that enables or disables editing of text in text box of the ComboBox
    //@{
    bool GetIsEditable() const;
    void SetIsEditable(bool value);
    //@}

    /// Gets or sets a value that enables selection-only mode, in which the contents of the combo
    /// box are selectable but not editable
    //@{
    bool GetIsReadOnly() const;
    void SetIsReadOnly(bool value);
    //@}

    /// Gets or sets the maximum height for a combo box drop-down
    //@{
    float GetMaxDropDownHeight() const;
    void SetMaxDropDownHeight(float height);
    //@}

    /// Gets or sets the placeholder text
    //@{
    const char* GetPlaceholder() const;
    void SetPlaceholder(const char* text);
    //@}

    /// Gets the item that is displayed in the selection box
    //@{
    BaseComponent* GetSelectionBoxItem() const;
    //@}

    /// Gets the item template of the selection box content
    //@{
    DataTemplate* GetSelectionBoxItemTemplate() const;
    //@}

    /// Gets or sets whether a ComboBox that is open and displays a drop-down control will remain
    /// open when a user clicks the TextBox.
    //@{
    bool GetStaysOpenOnEdit() const;
    void SetStaysOpenOnEdit(bool value);
    //@}

    /// Gets or sets the text of the currently selected item
    //@{
    const char* GetText() const;
    void SetText(const char* text);
    //@}

    /// Causes the item to scroll into view. If it is not visible, it is aligned either at the
    /// top or bottom of the viewport. Note this will only work if drop down is open.
    void ScrollIntoView(BaseComponent* item);

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* IsDropDownOpenProperty;
    static const DependencyProperty* IsEditableProperty;
    static const DependencyProperty* IsReadOnlyProperty;
    static const DependencyProperty* MaxDropDownHeightProperty;
    static const DependencyProperty* PlaceholderProperty;
    static const DependencyProperty* SelectionBoxItemProperty;
    static const DependencyProperty* SelectionBoxItemTemplateProperty;
    static const DependencyProperty* StaysOpenOnEditProperty;
    static const DependencyProperty* TextProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From UIElement
    //@{
    void OnPreviewKeyDown(const KeyEventArgs& e) override;
    void OnKeyDown(const KeyEventArgs& e) override;
    void OnPreviewMouseDown(const MouseButtonEventArgs& e) override;
    void OnMouseWheel(const MouseWheelEventArgs& e) override;
    void OnLostKeyboardFocus(const KeyboardFocusChangedEventArgs& e) override;
    //@}

    /// From FrameworkElement
    //@{
    void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot) override;
    Size MeasureOverride(const Size& availableSize) override;
    void UpdateVisualStates() override;
    //@}

    void OnIsFocusEngagedChanged(bool engaged) override;

    /// From ItemsControl
    //@{
    Ptr<DependencyObject> GetContainerForItemOverride() const override;
    bool IsItemItsOwnContainerOverride(BaseComponent* item) const override;
    void OnItemsChanged(const NotifyCollectionChangedEventArgs& args) override;
    void OnItemTemplateChanged(DataTemplate* oldTemplate, DataTemplate* newTemplate) override;
    void OnItemTemplateSelectorChanged(DataTemplateSelector* oldSelector,
        DataTemplateSelector* newSelector) override;
    //@}

    /// From Selector
    //@{
    void OnSelectionChanged(const SelectionChangedEventArgs& e) override;
    //@}

private:
    void Invalidate();
    void Update();

    void UpdateText(const char* text);
    void UpdateEditableText(const char* text);
    String GetSelectedItemText() const;
    void OnEditableTextChanged(BaseComponent* sender, const RoutedEventArgs& e);

    void UpdateSelectionBoxItem();
    void SetSelectionBoxItem(BaseComponent* item);
    void SetSelectionBoxItemTemplate(DataTemplate* itemTemplate);
    DataTemplate* FindItemTemplate(BaseComponent* item);

    void ManageKeyDown(const KeyEventArgs& e);
    void CloseDropDown(bool selectOnClose);
    void ToggleDropDown(bool selectOnClose);
    void ToggleDropDown(bool open, bool selectOnClose);
    void Close();

    void OnIsDropDownChanged(bool isDropDown);

    void SelectNext(int startIndex, int direction, bool allowCycle, bool focus);

    // Used by ComboBoxItem implementation
    friend class ComboBoxItem;
    void ItemSelected(ComboBoxItem* lbi);
    void ItemClicked(ComboBoxItem* cbi);
    void ItemEntered(ComboBoxItem* cbi);

private:
    Ptr<TextBox> mEditableTextBox;
    Ptr<ComboBoxItem> mHighlightedContainer;

    enum
    {
        NeedsUpdate = Selector::LastFlag * 2,
        UpdatingText = NeedsUpdate * 2,
        UpdatingEditableText = UpdatingText * 2,
        DropDownHasFocus = UpdatingEditableText * 2
    };

    NS_DECLARE_REFLECTION(ComboBox, Selector)
};

NS_WARNING_POP

}

#endif
