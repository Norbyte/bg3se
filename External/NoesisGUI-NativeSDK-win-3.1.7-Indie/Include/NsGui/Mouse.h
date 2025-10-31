////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MOUSE_H__
#define __GUI_MOUSE_H__


#include <NsCore/Noesis.h>
#include <NsCore/Delegate.h>
#include <NsCore/Ptr.h>
#include <NsCore/Vector.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/BaseComponent.h>
#include <NsGui/CoreApi.h>
#include <NsDrawing/Point.h>


namespace Noesis
{

NS_INTERFACE IUITreeNode;
class View;
class DependencyObject;
class UIElement;
class FrameworkElement;
class ContextMenu;
class ToolTip;
class Cursor;
class RoutedEvent;
struct RoutedEventArgs;
struct DependencyPropertyChangedEventArgs;
enum MouseButtonState: int32_t;
enum MouseButton: int32_t;
enum Orientation: int32_t;

typedef Noesis::Delegate<void(UIElement*)> DirectlyOverChangedEvent;

////////////////////////////////////////////////////////////////////////////////////////////////////
enum CaptureMode
{
    /// No mouse capture. Mouse input goes to the element under the mouse
    CaptureMode_None,

    /// Mouse capture is applied to a single element. Mouse input goes to the captured element
    CaptureMode_Element,

    /// Mouse capture is applied to a subtree of elements. If the mouse is over a child of the
    /// element with capture, mouse input is sent to the child element. Otherwise, mouse input is
    /// sent to the element with mouse capture
    CaptureMode_SubTree
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the mouse device.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.input.mouse.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Mouse: public BaseComponent
{
public:
    Mouse(View* view);
    ~Mouse();

    /// Resets mouse state
    void ResetState();

    /// Gets last mouse position
    Pointi GetPosition() const;

    /// Gets the mouse position relative to the specified element
    static Point GetPosition(UIElement* relativeTo);

    /// Gets the state of any mouse button
    MouseButtonState GetButtonState(MouseButton button) const;

    /// Tries to capture the mouse for the specified element
    bool Capture(UIElement* element, CaptureMode mode = CaptureMode_Element);

    /// Gets the captured mouse element
    UIElement* GetCaptured() const;

    /// Occurs when the element returned in a hit test corresponding to the position of the mouse
    /// pointer changes
    DirectlyOverChangedEvent& DirectlyOverChanged();

public:
    /// Attached routed events
    //@{
    static const RoutedEvent* GotMouseCaptureEvent;
    static const RoutedEvent* LostMouseCaptureEvent;
    static const RoutedEvent* MouseDownEvent;
    static const RoutedEvent* MouseEnterEvent;
    static const RoutedEvent* MouseLeaveEvent;
    static const RoutedEvent* MouseMoveEvent;
    static const RoutedEvent* MouseUpEvent;
    static const RoutedEvent* MouseWheelEvent;
    static const RoutedEvent* PreviewMouseDownEvent;
    static const RoutedEvent* PreviewMouseMoveEvent;
    static const RoutedEvent* PreviewMouseUpEvent;
    static const RoutedEvent* PreviewMouseWheelEvent;
    static const RoutedEvent* QueryCursorEvent;
    //@}

private:
    friend class View;
    friend class ViewLayout;
    friend class TouchScreen;
    friend class UIElement;
    friend class FrameworkElement;

    bool Capture(UIElement* element, CaptureMode mode, bool updateOver);

    void Deactivate();

    void ResetDirectlyOver();
    void InvalidateHitTest();

    /// Indicates if mouse requires to refresh over/captured element state
    bool NeedsUpdate() const;
    void Update();

    bool UpdateCursor();

    bool RaiseButtonDownEvents(UIElement* element, MouseButton button, uint32_t clickCount);
    bool ButtonDown(int x, int y, MouseButton button);
    bool ButtonUp(int x, int y, MouseButton button);
    bool DoubleClick(int x, int y, MouseButton button);
    bool Move(int x, int y);
    bool Wheel(int x, int y, int wheelRotation);
    bool HWheel(int x, int y, int wheelRotation);
    bool Scroll(float value);
    bool Scroll(int x, int y, float value);
    bool HScroll(float value);
    bool HScroll(int x, int y, float value);

    bool RaiseWheel(int x, int y, int wheelRotation, Orientation orientation);
    bool RaiseWheel(UIElement* element, int wheelRotation, Orientation orientation);

    struct HitInfo
    {
        Ptr<UIElement> enabledElement;
        Ptr<UIElement> hitVisibleElement;
    };

    bool SetOver(const HitInfo& hit);

    void DirectlyOverModified(BaseComponent* source, 
        const DependencyPropertyChangedEventArgs& args);
    void UpdateOver();

    void UpdateCaptureWithin(UIElement* newCapture);
    void OnCapturedIsEnabledChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);
    void OnCapturedIsVisibleChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);
    void OnCapturedIsHitTestVisibleChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);
    void UpdateCaptured();

    void RegisterCaptured(UIElement* element);
    void UnregisterCaptured(UIElement* element);

    bool OpenContextMenu(UIElement* element, const Point& mousePosition);
    void OnContextMenuClosed(BaseComponent* sender, const RoutedEventArgs& args);

    bool OpenToolTip(UIElement* element);
    bool CloseToolTip();
    void TryOpenToolTip();
    bool TryCloseToolTip();
    void OnToolTipClosed(BaseComponent* sender, const RoutedEventArgs& args);
    void CancelToolTipTimer();
    void ResetToolTip();

    void RegisterToolTipOwner(UIElement* owner);
    void UnregisterToolTipOwner();
    void OnToolTipOwnerHidden(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);
    void OnToolTipOwnerDestroyed(DependencyObject* obj);

    uint32_t OnToolTipInitialTimeElapsed();
    uint32_t OnToolTipDurationTimeElapsed();
    uint32_t OnToolTipBetweenTimeElapsed();

    HitInfo HitTest(const Point& point) const;
    HitInfo HitTest(View* view, const Point& point) const;
    struct HitTestContext;

    void UpdatePosition(int x, int y);
    void UpdateButton(MouseButton button, MouseButtonState state);

private:
    View* mView;

    // Last mouse state
    Point mPosition;

    static const uint32_t NumButtons = 5;
    MouseButtonState mButtonStates[NumButtons];

    typedef Vector<Ptr<UIElement>> Elements;

    // Captured element
    Ptr<UIElement> mCaptured;
    Elements mCaptureWithinElements;
    CaptureMode mCaptureMode;

    // Last top element under the mouse
    Ptr<UIElement> mDirectlyOver;
    Elements mOverElements;
    DirectlyOverChangedEvent mDirectlyOverChangedEvent;

    // Current cursor icon
    Cursor* mCursor;

    // Active context menu and its owner
    ContextMenu* mContextMenu;
    IUITreeNode* mContextMenuOwner;

    // TODO: Move this management to ToolTipService
    //@{
    FrameworkElement* mLastToolTipOwner;
    Ptr<ToolTip> mToolTip;
    uint32_t mToolTipTimer;
    //@}

    union
    {
        struct
        {
            bool capturedRegistered : 1;
            bool contextMenuOpened : 1;
            bool toolTipOwn : 1;
            bool toolTipDelay : 1;
            bool toolTipOpened : 1;
            bool toolTipBetween : 1;
            bool toolTipTimerCreated : 1;
            bool updatingOver : 1;
            bool needsUpdateOver : 1;
            bool needsUpdateCaptured : 1;
        } mFlags;

        // To quickly set all flags to 0
        uint32_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(Mouse, BaseComponent)
};

NS_WARNING_POP

}


#endif
