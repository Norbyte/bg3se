////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_LISTBOXITEM_H__
#define __GUI_LISTBOXITEM_H__


#include <NsCore/Noesis.h>
#include <NsGui/ContentControl.h>


namespace Noesis
{

class ListBox;
struct MouseButtonEventArgs;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a selectable item in a ListBox. 
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.listboxitem.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ListBoxItem: public ContentControl
{
public:
    ListBoxItem();

    /// Gets or sets a value that indicates whether a ListBoxItem is selected
    //@{
    bool GetIsSelected() const;
    void SetIsSelected(bool value);
    //@}

    /// Occurs when a ListBoxItem is selected
    UIElement::RoutedEvent_<RoutedEventHandler> Selected();

    /// Occurs when a ListBoxItem is unselected
    UIElement::RoutedEvent_<RoutedEventHandler> Unselected();

public:
    static const DependencyProperty* IsSelectedProperty;
    
    static const RoutedEvent* SelectedEvent;
    static const RoutedEvent* UnselectedEvent;

protected:
    /// Called when the ListBoxItem is selected in a ListBox
    virtual void OnSelected(const RoutedEventArgs& e);

    /// Called when the ListBoxItem is unselected in a ListBox
    virtual void OnUnselected(const RoutedEventArgs& e);

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From UIElement
    //@{
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e) override;
    void OnMouseRightButtonDown(const MouseButtonEventArgs& e) override;
    void OnKeyDown(const KeyEventArgs& e) override;
    void OnPreviewKeyDown(const KeyEventArgs& e) override;
    //@}

    /// From Control
    //@{
    void UpdateVisualStates() override;
    //@}

protected:
    bool mIsMouseClicked;

private:
    void OnMouseButtonCommon(const MouseButtonEventArgs& e);

    ListBox* FindParentListBox() const;

    NS_DECLARE_REFLECTION(ListBoxItem, ContentControl)
};

}


#endif
