////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CONTEXTMENU_H__
#define __GUI_CONTEXTMENU_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BaseMenu.h>


namespace Noesis
{

class Popup;
class PopupBinder;
class Menu;
struct Rect;
struct Point;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a pop-up menu that enables a control to expose functionality that is specific to
/// the context of the control.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.contextmenu.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ContextMenu: public BaseMenu
{
public:
    ContextMenu();

    /// Gets or sets a value that indicates whether the context menu appears with a dropped shadow
    //@{
    bool GetHasDropShadow() const;
    void SetHasDropShadow(bool value);
    //@}

    /// Get or sets the horizontal distance between the target origin and the popup alignment point
    //@{
    float GetHorizontalOffset() const;
    void SetHorizontalOffset(float value);
    //@}

    /// Gets or sets a value that indicates whether the ContextMenu is visible
    //@{
    bool GetIsOpen() const;
    void SetIsOpen(bool value);
    //@}

    /// Gets or sets the Placement property of a ContextMenu
    //@{
    PlacementMode GetPlacement() const;
    void SetPlacement(PlacementMode mode);
    //@}

    /// Gets or sets the area relative to which the context menu is positioned when it opens
    //@{
    const Rect& GetPlacementRectangle() const;
    void SetPlacementRectangle(const Rect& rect);
    //@}

    /// Gets or sets the UIElement relative to which the ContextMenu is positioned when it opens
    //@{
    UIElement* GetPlacementTarget() const;
    void SetPlacementTarget(UIElement* target);
    //@}

    /// Gets or sets a value that indicates whether the ContextMenu should close automatically
    //@{
    bool GetStaysOpen() const;
    void SetStaysOpen(bool value);
    //@}

    /// Get or sets the vertical distance between the target origin and the popup alignment point
    //@{
    float GetVerticalOffset() const;
    void SetVerticalOffset(float value);
    //@}

    /// Occurs when a particular instance of a ContextMenu closes
    UIElement::RoutedEvent_<RoutedEventHandler> Closed();

    /// Occurs when a particular instance of a context menu opens
    UIElement::RoutedEvent_<RoutedEventHandler> Opened();

    /// From IUITreeNode
    //@{
    void SetNodeParent(IUITreeNode* parent) override;
    //@}

    // Gets Popup where ContextMenu is shown
    Popup* GetPopup() const;

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* HasDropShadowProperty;
    static const DependencyProperty* HorizontalOffsetProperty;
    static const DependencyProperty* IsOpenProperty;
    static const DependencyProperty* PlacementProperty;
    static const DependencyProperty* PlacementRectangleProperty;
    static const DependencyProperty* PlacementTargetProperty;
    static const DependencyProperty* StaysOpenProperty;
    static const DependencyProperty* VerticalOffsetProperty;
    //@}

    /// Routed events
    //@{
    static const RoutedEvent* ClosedEvent;
    static const RoutedEvent* OpenedEvent;
    //@}

protected:
    virtual void OnOpened(const RoutedEventArgs& e);
    virtual void OnClosed(const RoutedEventArgs& e);

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From Visual
    //@{
    void OnDisconnectFromView() override;
    void OnVisualParentChanged(Visual* oldParent) override;
    //@}

    /// From UIElement
    //@{
    void OnKeyDown(const KeyEventArgs& e) override;
    void OnMouseUp(const MouseButtonEventArgs& e) override;
    void OnManipulationStarting(const ManipulationStartingEventArgs& e) override;
    void OnTapped(const TappedEventArgs& e) override;
    //@}

    /// From FrameworkElement
    //@{
    void OnLogicalParentChanged(FrameworkElement* oldParent) override;
    //@}

    /// From ItemsControl
    //@{
    Ptr<DependencyObject> GetContainerForItemOverride() const override;
    bool IsItemItsOwnContainerOverride(BaseComponent* item) const override;
    void ApplyItemContainerStyle(DependencyObject* container) const override;
    //@}

private:
    void OnIsOpenChanged(bool isOpen);
    void OnPopupClosed(BaseComponent* sender, const EventArgs& e);

    void ShowPopup();
    void HidePopup();

    void OnFocusedDestroyed(DependencyObject* d);

    template<class T>
    void CheckParent(Visual* parent) const;

private:
    PopupBinder* mPopupInternal;
    Menu* mMenuCapture;
    UIElement* mFocused;

    NS_DECLARE_REFLECTION(ContextMenu, BaseMenu)
};

}

#endif