////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_POPUP_H__
#define __GUI_POPUP_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsCore/Delegate.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

struct Size;
struct Rect;
class PopupLayer;
class DependencyProperty;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a pop-up window that has content.
/// 
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.popup.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Popup: public FrameworkElement
{
public:
    Popup();
    ~Popup();

    /// Gets or sets a value that indicates whether a Popup control can contain transparent content
    //@{
    bool GetAllowsTransparency() const;
    void SetAllowsTransparency(bool allowsTransparency);
    //@}

    /// Gets or sets content of the Popup control
    //@{
    UIElement* GetChild() const;
    void SetChild(UIElement* child);
    //@}

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

    /// Gets or sets a value that indicates whether is visible or not
    //@{
    bool GetIsOpen() const;
    void SetIsOpen(bool isOpen);
    //@}

    /// Gets or sets the orientation of the control when it opens, and specifies how the control
    /// behaves when it overlaps screen boundaries
    //@{
    PlacementMode GetPlacement() const;
    void SetPlacement(PlacementMode placement);
    //@}

    /// Gets or sets the rectangular area relative to which the control is positioned when
    /// it opens
    //@{
    const Rect& GetPlacementRectangle() const;
    void SetPlacementRectangle(const Rect& rect);
    //@}

    /// Gets or sets the UIElement relative to which the control is positioned when it opens
    //@{
    UIElement* GetPlacementTarget() const;
    void SetPlacementTarget(UIElement* target);
    //@}

    /// Gets or sets an animation for the opening and closing of a Popup control
    //@{
    PopupAnimation GetPopupAnimation() const;
    void SetPopupAnimation(PopupAnimation animation);
    //@}

    /// Gets or sets a value that indicates whether the Popup control closes when the control is no
    /// longer in focus. True if the Popup control closes when IsOpen property is set to false;
    /// False if the Popup control closes when a mouse or keyboard event occurs outside the Popup
    /// control. The default is true.
    //@{
    bool GetStaysOpen() const;
    void SetStaysOpen(bool staysOpen);
    //@}

    /// Get or sets the vertical distance between the target origin and the popup alignment point
    //@{
    float GetVerticalOffset() const;
    void SetVerticalOffset(float offset);
    //@}

    /// Determines if an element is descendant of another in a tree with Popups
    static bool IsDescendantOf(Visual* ancestor, Visual* child);

    /// Occurs when the IsOpen property changes to false
    DelegateEvent_<EventHandler> Closed();

    /// Occurs when the IsOpen property changes to true
    DelegateEvent_<EventHandler> Opened();

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* AllowsTransparencyProperty;
    static const DependencyProperty* ChildProperty;
    static const DependencyProperty* HasDropShadowProperty;
    static const DependencyProperty* HorizontalOffsetProperty;
    static const DependencyProperty* IsOpenProperty;
    static const DependencyProperty* PlacementProperty;
    static const DependencyProperty* PlacementRectangleProperty;
    static const DependencyProperty* PlacementTargetProperty;
    static const DependencyProperty* PopupAnimationProperty;
    static const DependencyProperty* StaysOpenProperty;
    static const DependencyProperty* VerticalOffsetProperty;
    //@}

protected:
    // From DependencyObject
    //@{
    void OnInit() override;
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    // From Visual
    //@{
    void OnConnectToView(IView* view) override;
    void OnDisconnectFromView() override;
    //@}

    // From UIElement
    //@{
    void OnPreviewMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnPreviewMouseLeftButtonUp(const MouseButtonEventArgs& e) override;
    void OnPreviewMouseRightButtonDown(const MouseButtonEventArgs& e) override;
    void OnPreviewMouseRightButtonUp(const MouseButtonEventArgs& e) override;
    void OnMouseWheel(const MouseWheelEventArgs& e) override;
    //@}

    // From FrameworkElement
    //@{
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const override;
    uint32_t GetLogicalChildrenCount() const override;
    Ptr<BaseComponent> GetLogicalChild(uint32_t index) const override;
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    void OnIsOpenChanged(bool isOpen);
    void OnStaysOpenChanged(bool staysOpen);
    void OnPreviewMouseButton(const MouseButtonEventArgs& e);

    bool IsPopupOpen() const;
    void ShowPopup();
    void HidePopup();

    bool CanFireAnimation();
    void FireShowAnimation();
    void FireHideAnimation();

    void MouseCapture();
    void MouseRelease();

private:
    Ptr<PopupLayer> mPopupLayer;

    EventHandler mClosedEvent;
    EventHandler mOpenedEvent;

    union
    {
        mutable struct
        {
            bool isOpen : 1;
            bool isClosing : 1;
        } mFlags;

        // To quickly set all flags to 0
        mutable uint8_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(Popup, FrameworkElement)
};

NS_WARNING_POP

}


#endif
