////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TABITEM_H__
#define __GUI_TABITEM_H__


#include <NsCore/Noesis.h>
#include <NsGui/HeaderedContentControl.h>


namespace Noesis
{

class TabControl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a selectable item inside a TabControl.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.tabitem.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TabItem: public HeaderedContentControl
{
public:
    TabItem();

    /// Gets or sets a value that indicates whether the TabItem is selected
    //@{
    bool GetIsSelected() const;
    void SetIsSelected(bool isSelected);
    //@}

    /// Gets the tab strip placement
    Dock GetTabStripPlacement() const;

    // Used by TabControl to force focus on the Tab
    // Returns true if focus was set in the tab or in the content of the tab
    bool SetFocus();

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* IsSelectedProperty;
    static const DependencyProperty* TabStripPlacementProperty;
    //@}

protected:
    /// Called to indicate that the IsSelected property has changed to true
    virtual void OnSelected(const RoutedEventArgs& args);

    /// Called to indicate that the IsSelected property has changed to false
    virtual void OnUnselected(const RoutedEventArgs& args);

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From UIElement
    //@{
    void OnKeyDown(const KeyEventArgs& e) override;
    void OnPreviewGotKeyboardFocus(const KeyboardFocusChangedEventArgs& e) override;
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    //@}
    
    /// From Control
    //@{
    void UpdateVisualStates() override;
    //@}

private:
    TabControl* GetParentTabControl() const;

    NS_DECLARE_REFLECTION(TabItem, HeaderedContentControl)
};

}


#endif
