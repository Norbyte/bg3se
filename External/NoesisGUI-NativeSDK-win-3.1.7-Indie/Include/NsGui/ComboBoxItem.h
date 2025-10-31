////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMBOBOXITEM_H__
#define __GUI_COMBOBOXITEM_H__


#include <NsCore/Noesis.h>
#include <NsGui/ListBoxItem.h>


namespace Noesis
{

class ComboBox;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements a selectable item inside a ComboBox.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.comboboxitem.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ComboBoxItem: public ListBoxItem
{
public:
    ComboBoxItem();

    /// Gets a value that indicates whether the item is highlighted
    bool GetIsHighlighted() const;

    // Used internally by ComboBox
    void SetHighlighted(bool value);

public:
    static const DependencyProperty* IsHighlightedProperty;

protected:
    /// From UIElement
    //@{
    void OnMouseEnter(const MouseEventArgs& e) override;
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e) override;
    void OnKeyDown(const KeyEventArgs& e) override;
    void OnGotKeyboardFocus(const KeyboardFocusChangedEventArgs& e) override;
    //@}

    /// From ListBoxItem
    //@{
    void OnSelected(const RoutedEventArgs& e) override;
    //@}

private:
    ComboBox* FindParentComboBox() const;

    NS_DECLARE_REFLECTION(ComboBoxItem, ListBoxItem)
};

}

#endif
