////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TOOLTIP_H__
#define __GUI_TOOLTIP_H__


#include <NsCore/Noesis.h>
#include <NsGui/ContentControl.h>


namespace Noesis
{

class Popup;
class PopupBinder;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that creates a pop-up window that displays information for an element in
/// the interface.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.tooltip
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ToolTip: public ContentControl
{
public:
    ToolTip();

    /// Gets or sets a value that indicates whether the control has a drop shadow
    //@{
    bool GetHasDropShadow() const;
    void SetHasDropShadow(bool hasShadow);
    //@}

    /// Get or sets the horizontal distance between the target origin and the popup alignment point
    //@{
    float GetHorizontalOffset() const;
    void SetHorizontalOffset(float offset);
    //@}

    /// Gets or sets a value that indicates whether a ToolTip is visible
    //@{
    bool GetIsOpen() const;
    void SetIsOpen(bool isOpen);
    //@}

    /// Gets or sets the orientation of the ToolTip control when it opens, and specifies how the
    /// ToolTip control behaves when it overlaps screen boundaries
    //@{
    PlacementMode GetPlacement() const;
    void SetPlacement(PlacementMode placement);
    //@}

    /// Gets or sets the rectangular area relative to which the ToolTip control is positioned when
    /// it opens
    //@{
    const Rect& GetPlacementRectangle() const;
    void SetPlacementRectangle(const Rect& rect);
    //@}

    /// Gets or sets the UIElement relative to which the ToolTip is positioned when it opens
    //@{
    UIElement* GetPlacementTarget() const;
    void SetPlacementTarget(UIElement* target);
    //@}

    /// Gets or sets a value that indicates whether an open ToolTip remains open until the user
    /// clicks the mouse when the mouse is not over the ToolTip
    //@{
    bool GetStaysOpen() const;
    void SetStaysOpen(bool staysOpen);
    //@}

    /// Get or sets the vertical distance between the target origin and the popup alignment point
    //@{
    float GetVerticalOffset() const;
    void SetVerticalOffset(float offset);
    //@}

    /// Occurs when a ToolTip is closed and is no longer visible
    UIElement::RoutedEvent_<RoutedEventHandler> Closed();

    /// Occurs when a ToolTip becomes visible
    UIElement::RoutedEvent_<RoutedEventHandler> Opened();

    /// Gets Popup where ToolTip is shown
    Popup* GetPopup() const;

    /// From IUITreeNode
    //@{
    void SetNodeParent(IUITreeNode* parent) override;
    //@}

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

    /// Dependency events
    //@{
    static const RoutedEvent* ClosedEvent;
    static const RoutedEvent* OpenedEvent;
    //@}

protected:
    virtual void OnClosed(const RoutedEventArgs& e);
    virtual void OnOpened(const RoutedEventArgs& e);

    /// From Visual
    //@{
    void OnVisualParentChanged(Visual* oldParent) override;
    //@}

    /// From FrameworkElement
    //@{
    void OnLogicalParentChanged(FrameworkElement* oldParent) override;
    //@}

    /// From Control
    //@{
    void UpdateVisualStates() override;
    //@}

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

private:
    void OnIsOpenChanged(bool isOpen);
    void OnOwnerDestroyed(DependencyObject* owner);
    void OnPopupClosed(BaseComponent* sender, const EventArgs& e);

    void Close();

private:
    PopupBinder* mPopupInternal;

    NS_DECLARE_REFLECTION(ToolTip, ContentControl)
};

}


#endif
