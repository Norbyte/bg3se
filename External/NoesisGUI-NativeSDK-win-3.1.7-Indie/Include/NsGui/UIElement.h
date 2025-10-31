////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_UIELEMENT_H__
#define __GUI_UIELEMENT_H__


#include <NsCore/Noesis.h>
#include <NsGui/Visual.h>
#include <NsGui/UIElementEvents.h>
#include <NsDrawing/Size.h>
#include <NsDrawing/Rect.h>
#include <NsCore/Vector.h>


namespace Noesis
{

class Effect;
class Brush;
class DrawingContext;
class Geometry;
class Transform;
class Transform3D;
class CommandBinding;
class InputBinding;
class Mouse;
class Keyboard;
struct SizeChangedInfo;

template<class T> class UICollection;
typedef UICollection<CommandBinding> CommandBindingCollection;
typedef UICollection<InputBinding> InputBindingCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies the direction within a user interface (UI) in which a desired focus change request is
/// attempted. The direction is either based on tab order or by relative direction in layout.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum FocusNavigationDirection: int32_t
{
    /// Move focus to the next focusable element in tab order. Not supported for PredictFocus.
    FocusNavigationDirection_Next,

    /// Move focus to the previous focusable element in tab order. Not supported for PredictFocus.
    FocusNavigationDirection_Previous,

    /// Move focus to the first focusable element in tab order. Not supported for PredictFocus.
    FocusNavigationDirection_First,

    /// Move focus to the last focusable element in tab order. Not supported for PredictFocus.
    FocusNavigationDirection_Last,

    /// Move focus to another focusable element to the left of the currently focused element.
    FocusNavigationDirection_Left,

    /// Move focus to another focusable element to the right of the currently focused element.
    FocusNavigationDirection_Right,

    /// Move focus to another focusable element upwards from the currently focused element.
    FocusNavigationDirection_Up,

    /// Move focus to another focusable element downwards from the currently focused element.
    FocusNavigationDirection_Down
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a request to move focus to another control. 
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TraversalRequest
{
    /// Gets the traversal direction
    FocusNavigationDirection focusNavigationDirection;

    /// Gets or sets a value that indicates whether focus traversal has reached the end of child
    /// elements that can have focus
    bool wrapped;
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a starting point for element layout characteristics, and also exposes virtual methods
/// that derived classes can override, which can influence the layout rendering behavior of the
/// element and its child elements.
///
/// Much of the input (keyboard, mouse and touch) and focusing behavior for elements in general is
/// also defined in the UIElement class.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.uielement.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API UIElement: public Visual
{
public:
    UIElement();
    UIElement(const UIElement&) = delete;
    UIElement& operator=(const UIElement&) = delete;
    ~UIElement();

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Dependency property accessors
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// Gets or sets a value indicating whether this element can be used as the target of a 
    /// drag-and-drop operation
    //@{
    bool GetAllowDrop() const;
    void SetAllowDrop(bool value);
    //@}

    /// Gets or sets the geometry used to define the outline of the contents of an element
    //@{
    Geometry* GetClip() const;
    void SetClip(Geometry* geometry);
    //@}

    /// Gets or sets a value indicating whether to clip the content of this element (or content
    /// coming from the child elements of this element) to fit into the size of the containing
    /// element
    //@{
    bool GetClipToBounds() const;
    void SetClipToBounds(bool clipToBounds);
    //@}

    /// Gets or sets the bitmap effect to apply to the element
    //@{
    Effect* GetEffect() const;
    void SetEffect(Effect* value);
    //@}

    /// Gets or sets a value that indicates whether the element can receive focus
    //@{
    bool GetFocusable() const;
    void SetFocusable(bool value);
    //@}

    /// Gets or sets a value indicating whether this element is enabled in the user interface
    //@{
    bool GetIsEnabled() const;
    void SetIsEnabled(bool enabled);
    //@}

    /// Indicates whether this element has the focus
    bool GetIsFocused() const;

    /// Gets or sets a value that declares whether this element can possibly be returned as a hit 
    /// test result from some portion of its rendered content
    //@{
    bool GetIsHitTestVisible() const;
    void SetIsHitTestVisible(bool value);
    //@}

    /// Gets a value indicating whether this element has keyboard focus
    bool GetIsKeyboardFocused() const;

    /// Gets a value indicating whether keyboard focus is anywhere within the element or its visual 
    /// tree child elements
    bool GetIsKeyboardFocusWithin() const;

    /// Gets a value indicating whether the mouse is captured to this element
    bool GetIsMouseCaptured() const;

    /// Gets a value that determines whether mouse capture is held by this element or by child 
    /// elements in its visual tree
    bool GetIsMouseCaptureWithin() const;

    /// Gets a value that indicates whether the position of the mouse pointer corresponds to hit 
    /// test results, which take element compositing into account.
    bool GetIsMouseDirectlyOver() const;

    /// Gets a value indicating whether the mouse pointer is located over this element (including 
    /// child elements in the visual tree)
    bool GetIsMouseOver() const;

    /// Indicates whether this element is visible in the user interface
    bool GetIsVisible() const;

    /// Gets or sets an opacity mask, as a Brush implementation that is applied to any 
    /// alpha-channel masking for the rendered content of this element
    //@{
    Brush* GetOpacityMask() const;
    void SetOpacityMask(Brush* mask);
    //@}

    /// Gets or sets the opacity factor applied to the entire element when it is rendered
    //@{
    float GetOpacity() const;
    void SetOpacity(float opacity);
    //@}

    /// Gets or sets the center point of any possible render transform declared by
    /// *RenderTransform*, relative to the bounds of the element
    //@{
    const Point& GetRenderTransformOrigin() const;
    void SetRenderTransformOrigin(const Point& origin);
    //@}

    /// Gets or sets transform information that affects the rendering position of this element
    //@{
    Transform* GetRenderTransform() const;
    void SetRenderTransform(Transform* transform);
    //@}

    /// Gets or sets the 3D effect to apply when rendering this element
    //@{
    Transform3D* GetTransform3D() const;
    void SetTransform3D(Transform3D* transform);
    //@}

    /// Gets or sets the user interface visibility of this element
    //@{
    Noesis::Visibility GetVisibility() const;
    void SetVisibility(Noesis::Visibility visibility);
    //@}

    /// Finds the View where this element is connected to traversing visual or logical ancestors
    IView* GetUIView() const;

    /// Gets the logical or visual parent for this element
    UIElement* GetUIParent() const;

    /// Gets a collection of CommandBinding objects associated with this element
    CommandBindingCollection* GetCommandBindings() const;

    /// Gets a collection of InputBinding objects associated with this element
    InputBindingCollection* GetInputBindings() const;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// Public methods in UIElement
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// Gets mouse input manager
    Mouse* GetMouse() const;

    /// Attempts to force capture of the mouse to this element
    bool CaptureMouse();

    /// Releases the mouse capture, if this element held the capture
    void ReleaseMouseCapture();

    /// Gets or sets a value that indicates whether manipulation events are enabled on this element
    //@{
    bool GetIsManipulationEnabled() const;
    void SetIsManipulationEnabled(bool enabled);
    //@}

    /// Gets or sets a value that determines whether this element can originate *Tapped* events
    //@{
    bool GetIsTapEnabled() const;
    void SetIsTapEnabled(bool enabled);
    //@}

    /// Gets or sets a value that determines whether this element can originate *DoubleTapped* events
    //@{
    bool GetIsDoubleTapEnabled() const;
    void SetIsDoubleTapEnabled(bool enabled);
    //@}

    /// Gets or sets a value that determines whether this element can originate *Holding* events
    //@{
    bool GetIsHoldingEnabled() const;
    void SetIsHoldingEnabled(bool enabled);
    //@}

    /// Gets or sets a value that determines whether this element can originate *RightTapped* events
    //@{
    bool GetIsRightTapEnabled() const;
    void SetIsRightTapEnabled(bool enabled);
    //@}

    /// Attempts to force capture of a touch to this element
    bool CaptureTouch(uint64_t touchDevice);

    /// Attempts to release the specified touch device from this element
    bool ReleaseTouchCapture(uint64_t touchDevice);
    
    /// Releases all captured touch devices from this element
    void ReleaseAllTouchCaptures();

    /// Gets element captured by the specified touch device
    UIElement* GetTouchCaptured(uint64_t touchDevice);

    /// Gets keyboard input manager
    Keyboard* GetKeyboard() const;

    /// Attempts to set focus to this element
    bool Focus();

    /// Attempts to set focus to this element indicating if control should be engaged or not
    bool Focus(bool engage);

    /// Gets control's desired size (calculated after *Measure* process)
    const Size& GetDesiredSize() const;
     
    /// Gets calculated (during *Arrange*) render size
    const Size& GetRenderSize() const;

    /// Invalidates the measurement state (layout) for the element
    void InvalidateMeasure();

    /// Indicates if measured size is valid
    bool IsMeasureValid() const;
    
    /// Updates the *DesiredSize* of a UIElement. Parent elements call this method from their own
    /// *MeasureCore* implementations to form a recursive layout update. Calling this method
    /// constitutes the first pass of a layout update in the layout system
    void Measure(const Size& availableSize);

    /// Invalidates the arrange state (layout) for the element. After the invalidation,
    /// the element will have its layout updated, which will occur asynchronously
    /// unless subsequently forced by *UpdateLayout*
    void InvalidateArrange();

    /// Indicates if arrange is valid
    bool IsArrangeValid() const;

    /// Positions child elements and determines a size for a UIElement. Parent elements call this
    /// method from their *ArrangeCore* implementation to form a recursive layout update. This
    /// method constitutes the second pass of a layout update
    void Arrange(const Rect& finalRect);

    /// Invalidates the rendering of the element, and forces a complete new layout pass. *OnRender*
    /// is called after the layout cycle is completed
    void InvalidateVisual();

    /// Ensures that all visual child elements of this element are properly updated for layout
    void UpdateLayout();

    /// Request to move the focus from this element to another element. Returns true if focus is
    /// moved successfully, returns false if there is no next element
    virtual bool MoveFocus(const TraversalRequest& request);

    /// When overridden in a derived class, returns the element that would receive focus for a 
    /// specified focus traversal direction, without actually moving focus to that element
    virtual DependencyObject* PredictFocus(FocusNavigationDirection direction);

    // Gets previous measure constraint (internally used by Layout manager)
    const Size& GetMeasureConstraint() const;

    // Gets previous arrange constraint (internally used by Layout manager)
    const Rect& GetArrangeConstraint() const;

    // RoutedEvents: A function to register handlers must be specified for each registered event
    // of a class. The function must have the same name as the event and return a 'RoutedEvent_'
    // object to allow users to Add or Remove a handler to the event by using operators += or -=
    template <class T>
    class RoutedEvent_
    {
    public:
        inline RoutedEvent_(UIElement* element, const RoutedEvent* ev):
            _element(element), _routedEvent(ev)
        {
            NS_ASSERT(element != 0);
            NS_ASSERT(ev != 0);
        }

        inline void operator+=(const T& handler)
        {
            _element->AddHandler(_routedEvent, *reinterpret_cast<const RoutedEventHandler*>(
                &handler));
        }

        inline void operator-=(const T& handler)
        {
            _element->RemoveHandler(_routedEvent, *reinterpret_cast<const RoutedEventHandler*>(
                &handler));
        }

    private:
        UIElement* _element;
        const RoutedEvent* _routedEvent;
    };

    /// Occurs when this element gets logical focus
    RoutedEvent_<RoutedEventHandler> GotFocus();
    /// Occurs when the keyboard is focused on this element
    RoutedEvent_<KeyboardFocusChangedEventHandler> GotKeyboardFocus();
    /// Occurs when this element captures the mouse
    RoutedEvent_<MouseEventHandler> GotMouseCapture();
    /// Occurs when a key is pressed while focus is on this element
    RoutedEvent_<KeyEventHandler> KeyDown();
    /// Occurs when a key is released while focus is on this element
    RoutedEvent_<KeyEventHandler> KeyUp();
    /// Occurs when this element loses logical focus
    RoutedEvent_<RoutedEventHandler> LostFocus();
    /// Occurs when the keyboard is no longer focused on this element
    RoutedEvent_<KeyboardFocusChangedEventHandler> LostKeyboardFocus();
    /// Occurs when this element loses mouse capture
    RoutedEvent_<MouseEventHandler> LostMouseCapture();
    /// Occurs when any mouse button is pressed while the pointer is over this element
    RoutedEvent_<MouseButtonEventHandler> MouseDown();
    /// Occurs when the mouse pointer enters the bounds of this element
    RoutedEvent_<MouseEventHandler> MouseEnter();
    /// Occurs when the mouse pointer leaves the bounds of this element
    RoutedEvent_<MouseEventHandler> MouseLeave();
    /// Occurs when the left mouse button is pressed while the mouse pointer is over this element
    RoutedEvent_<MouseButtonEventHandler> MouseLeftButtonDown();
    /// Occurs when the left mouse button is released while the mouse pointer is over this element
    RoutedEvent_<MouseButtonEventHandler> MouseLeftButtonUp();
    /// Occurs when the mouse pointer moves while over this element
    RoutedEvent_<MouseEventHandler> MouseMove();
    /// Occurs when the right mouse button is pressed while the mouse pointer is over this element
    RoutedEvent_<MouseButtonEventHandler> MouseRightButtonDown();
    /// Occurs when the right mouse button is released while the mouse pointer is over this element
    RoutedEvent_<MouseButtonEventHandler> MouseRightButtonUp();
    /// Occurs when any mouse button is released over this element
    RoutedEvent_<MouseButtonEventHandler> MouseUp();
    /// Occurs when the user rotates the mouse wheel while the mouse pointer is over this element
    RoutedEvent_<MouseWheelEventHandler> MouseWheel();
    /// Occurs when a finger touches the screen while the finger is over this element
    RoutedEvent_<TouchEventHandler> TouchDown();
    /// Occurs when a finger moves on the screen while the finger is over this element
    RoutedEvent_<TouchEventHandler> TouchMove();
    /// Occurs when a finger is raised off of the screen while the finger is over this element
    RoutedEvent_<TouchEventHandler> TouchUp();
    /// Occurs when a touch moves from outside to inside the bounds of this element
    RoutedEvent_<TouchEventHandler> TouchEnter();
    /// Occurs when a touch moves from inside to outside the bounds of this element
    RoutedEvent_<TouchEventHandler> TouchLeave();
    /// Occurs when a touch is captured to this element
    RoutedEvent_<TouchEventHandler> GotTouchCapture();
    /// Occurs when this element loses a touch capture
    RoutedEvent_<TouchEventHandler> LostTouchCapture();
    /// Occurs when a finger touches the screen while the finger is over this element
    RoutedEvent_<TouchEventHandler> PreviewTouchDown();
    /// Occurs when a finger moves on the screen while the finger is over this element
    RoutedEvent_<TouchEventHandler> PreviewTouchMove();
    /// Occurs when a finger is raised off of the screen while the finger is over this element
    RoutedEvent_<TouchEventHandler> PreviewTouchUp();
    /// Occurs when the manipulation processor is first created
    RoutedEvent_<ManipulationStartingEventHandler> ManipulationStarting();
    /// Occurs when an input device begins a manipulation on the UIElement object
    RoutedEvent_<ManipulationStartedEventHandler> ManipulationStarted();
    /// Occurs when the input device changes position during a manipulation
    RoutedEvent_<ManipulationDeltaEventHandler> ManipulationDelta();
    /// Occurs when the input device loses contact with the UIElement during a manipulation and
    /// inertia begins
    RoutedEvent_<ManipulationInertiaStartingEventHandler> ManipulationInertiaStarting();
    /// Occurs when a manipulation and inertia on the UIElement object is complete
    RoutedEvent_<ManipulationCompletedEventHandler> ManipulationCompleted();
    /// Occurs when a Tap interaction occurs on this element
    RoutedEvent_<TappedEventHandler> Tapped();
    /// Occurs when a DoubleTap interaction occurs on this element
    RoutedEvent_<DoubleTappedEventHandler> DoubleTapped();
    /// Occurs when a Hold interaction occurs on this element
    RoutedEvent_<HoldingEventHandler> Holding();
    /// Occurs when a DoubleTap interaction occurs on this element
    RoutedEvent_<RightTappedEventHandler> RightTapped();
    /// Occurs when the keyboard is focused on this element
    RoutedEvent_<KeyboardFocusChangedEventHandler> PreviewGotKeyboardFocus();
    /// Occurs when a key is pressed while focus is on this element
    RoutedEvent_<KeyEventHandler> PreviewKeyDown();
    /// Occurs when a key is released while focus is on this element
    RoutedEvent_<KeyEventHandler> PreviewKeyUp();
    /// Occurs when the keyboard is no longer focused on this element
    RoutedEvent_<KeyboardFocusChangedEventHandler> PreviewLostKeyboardFocus();
    /// Occurs when any mouse button is pressed while the pointer is over this element
    RoutedEvent_<MouseButtonEventHandler> PreviewMouseDown();
    /// Occurs when the left mouse button is pressed while the mouse pointer is over this element
    RoutedEvent_<MouseButtonEventHandler> PreviewMouseLeftButtonDown();
    /// Occurs when the left mouse button is released while the mouse pointer is over this element
    RoutedEvent_<MouseButtonEventHandler> PreviewMouseLeftButtonUp();
    /// Occurs when the mouse pointer moves while the mouse pointer is over this element
    RoutedEvent_<MouseEventHandler> PreviewMouseMove();
    /// Occurs when the right mouse button is pressed while the mouse pointer is over this element
    RoutedEvent_<MouseButtonEventHandler> PreviewMouseRightButtonDown();
    /// Occurs when the right mouse button is released while the mouse pointer is over this element
    RoutedEvent_<MouseButtonEventHandler> PreviewMouseRightButtonUp();
    /// Occurs when any mouse button is released while the mouse pointer is over this element
    RoutedEvent_<MouseButtonEventHandler> PreviewMouseUp();
    /// Occurs when the user rotates the mouse wheel while the mouse pointer is over this element
    RoutedEvent_<MouseWheelEventHandler> PreviewMouseWheel();
    /// Occurs when this element gets text in a device-independent manner
    RoutedEvent_<TextCompositionEventHandler> PreviewTextInput();
    /// Occurs when the cursor is requested to display
    RoutedEvent_<QueryCursorEventHandler> QueryCursor();
    /// Occurs when this element gets text in a device-independent manner
    RoutedEvent_<TextCompositionEventHandler> TextInput();
    /// Notifies source object of a dragging operation
    RoutedEvent_<QueryContinueDragEventHandler> PreviewQueryContinueDrag();
    /// Notifies source object of a dragging operation
    RoutedEvent_<QueryContinueDragEventHandler> QueryContinueDrag();
    /// Notifies source object of a dragging operation to provide feedback to the user
    RoutedEvent_<GiveFeedbackEventHandler> PreviewGiveFeedback();
    /// Notifies source object of a dragging operation to provide feedback to the user
    RoutedEvent_<GiveFeedbackEventHandler> GiveFeedback();
    /// Occurs when mouse enters the target of a dragging operation
    RoutedEvent_<DragEventHandler> PreviewDragEnter();
    /// Occurs when mouse enters the target of a dragging operation
    RoutedEvent_<DragEventHandler> DragEnter();
    /// Occurs when mouse is moved over the target of a dragging operation
    RoutedEvent_<DragEventHandler> PreviewDragOver();
    /// Occurs when mouse is moved over the target of a dragging operation
    RoutedEvent_<DragEventHandler> DragOver();
    /// Occurs when mouse leaves the target of a dragging operation
    RoutedEvent_<DragEventHandler> PreviewDragLeave();
    /// Occurs when mouse leaves the target of a dragging operation
    RoutedEvent_<DragEventHandler> DragLeave();
    /// Notifies target object of a dragging operation that data is being dropped
    RoutedEvent_<DragEventHandler> PreviewDrop();
    /// Notifies target object of a dragging operation that data is being dropped
    RoutedEvent_<DragEventHandler> Drop();

    /// Adds a handler to the specified routed event
    void AddHandler(const RoutedEvent* ev, const RoutedEventHandler& handler);

    /// Removes a handler from the specified routed event
    void RemoveHandler(const RoutedEvent* ev, const RoutedEventHandler& handler);

    /// Raises a specific routed event. The RoutedEvent to be raised is identified within the 
    /// RoutedEventArgs instance that is provided (as the RoutedEvent property of that event data)
    void RaiseEvent(const RoutedEventArgs& e);

    // Events: A function to register handlers must be specified for each event of a class. The
    // function must have the same name as the event and return an 'Event_' object to allow users
    // to Add or Remove a handler to the event by using operators += or -=
    template <class T>
    class Event_
    {
    public:
        inline Event_(UIElement* element, Symbol key):
            _element(element), _key(key)
        {
            NS_ASSERT(element != 0);
            NS_ASSERT(!key.IsNull());
        }

        inline void operator+=(const T& handler)
        {
            _element->AddEventHandler(_key, *reinterpret_cast<const EventHandler*>(&handler));
        }

        inline void operator-=(const T& handler)
        {
            _element->RemoveEventHandler(_key, *reinterpret_cast<const EventHandler*>(&handler));
        }

    private:
        UIElement* _element;
        Symbol _key;
    };

    /// Occurs when the value of the Focusable property changes
    Event_<DependencyPropertyChangedEventHandler> FocusableChanged();
    /// Occurs when the value of the IsEnabled property on this element changes
    Event_<DependencyPropertyChangedEventHandler> IsEnabledChanged();
    /// Occurs when the value of the IsHitTestVisible dependency property changes on this element
    Event_<DependencyPropertyChangedEventHandler> IsHitTestVisibleChanged();
    /// Occurs when the value of the IsVisible property changes on this element
    Event_<DependencyPropertyChangedEventHandler> IsVisibleChanged();
    /// Occurs when the value of the IsMouseCaptured property changes on this element
    Event_<DependencyPropertyChangedEventHandler> IsMouseCapturedChanged();
    /// Occurs when the value of the IsMouseCaptureWithinProperty changes on this element
    Event_<DependencyPropertyChangedEventHandler> IsMouseCaptureWithinChanged();
    /// Occurs when the value of the IsMouseDirectlyOver property changes on this element
    Event_<DependencyPropertyChangedEventHandler> IsMouseDirectlyOverChanged();
    /// Occurs when the value of the IsKeyboardFocused property changes on this element
    Event_<DependencyPropertyChangedEventHandler> IsKeyboardFocusedChanged();
    /// Occurs when the value of the IsKeyboardFocusWithinChanged property changes on this element
    Event_<DependencyPropertyChangedEventHandler> IsKeyboardFocusWithinChanged();
    /// Occurs when the layout of any elements associated with the current View changes
    Event_<EventHandler> LayoutUpdated();

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* AllowDropProperty;
    static const DependencyProperty* ClipProperty;
    static const DependencyProperty* ClipToBoundsProperty;
    static const DependencyProperty* EffectProperty;
    static const DependencyProperty* FocusableProperty;
    static const DependencyProperty* IsEnabledProperty;
    static const DependencyProperty* IsFocusedProperty;
    static const DependencyProperty* IsHitTestVisibleProperty;
    static const DependencyProperty* IsKeyboardFocusedProperty;
    static const DependencyProperty* IsKeyboardFocusWithinProperty;
    static const DependencyProperty* IsMouseCapturedProperty;
    static const DependencyProperty* IsMouseCaptureWithinProperty;
    static const DependencyProperty* IsMouseDirectlyOverProperty;
    static const DependencyProperty* IsMouseOverProperty;
    static const DependencyProperty* IsManipulationEnabledProperty;
    static const DependencyProperty* IsTapEnabledProperty;
    static const DependencyProperty* IsDoubleTapEnabledProperty;
    static const DependencyProperty* IsHoldingEnabledProperty;
    static const DependencyProperty* IsRightTapEnabledProperty;
    static const DependencyProperty* IsVisibleProperty;
    static const DependencyProperty* OpacityMaskProperty;
    static const DependencyProperty* OpacityProperty;
    static const DependencyProperty* RenderTransformOriginProperty;
    static const DependencyProperty* RenderTransformProperty;
    static const DependencyProperty* Transform3DProperty;
    static const DependencyProperty* VisibilityProperty;
    //@}

    /// Dependency events
    //@{
    static const RoutedEvent* GotFocusEvent;
    static const RoutedEvent* GotKeyboardFocusEvent;
    static const RoutedEvent* GotMouseCaptureEvent;
    static const RoutedEvent* KeyDownEvent;
    static const RoutedEvent* KeyUpEvent;
    static const RoutedEvent* LostFocusEvent;
    static const RoutedEvent* LostKeyboardFocusEvent;
    static const RoutedEvent* LostMouseCaptureEvent;
    static const RoutedEvent* MouseDownEvent;
    static const RoutedEvent* MouseEnterEvent;
    static const RoutedEvent* MouseLeaveEvent;
    static const RoutedEvent* MouseLeftButtonDownEvent;
    static const RoutedEvent* MouseLeftButtonUpEvent;
    static const RoutedEvent* MouseMoveEvent;
    static const RoutedEvent* MouseRightButtonDownEvent;
    static const RoutedEvent* MouseRightButtonUpEvent;
    static const RoutedEvent* MouseUpEvent;
    static const RoutedEvent* MouseWheelEvent;
    static const RoutedEvent* TouchDownEvent;
    static const RoutedEvent* TouchMoveEvent;
    static const RoutedEvent* TouchUpEvent;
    static const RoutedEvent* TouchEnterEvent;
    static const RoutedEvent* TouchLeaveEvent;
    static const RoutedEvent* GotTouchCaptureEvent;
    static const RoutedEvent* LostTouchCaptureEvent;
    static const RoutedEvent* PreviewTouchDownEvent;
    static const RoutedEvent* PreviewTouchMoveEvent;
    static const RoutedEvent* PreviewTouchUpEvent;
    static const RoutedEvent* ManipulationStartingEvent;
    static const RoutedEvent* ManipulationStartedEvent;
    static const RoutedEvent* ManipulationDeltaEvent;
    static const RoutedEvent* ManipulationInertiaStartingEvent;
    static const RoutedEvent* ManipulationCompletedEvent;
    static const RoutedEvent* TappedEvent;
    static const RoutedEvent* DoubleTappedEvent;
    static const RoutedEvent* HoldingEvent;
    static const RoutedEvent* RightTappedEvent;
    static const RoutedEvent* PreviewGotKeyboardFocusEvent;
    static const RoutedEvent* PreviewKeyDownEvent;
    static const RoutedEvent* PreviewKeyUpEvent;
    static const RoutedEvent* PreviewLostKeyboardFocusEvent;
    static const RoutedEvent* PreviewMouseDownEvent;
    static const RoutedEvent* PreviewMouseLeftButtonDownEvent;
    static const RoutedEvent* PreviewMouseLeftButtonUpEvent;
    static const RoutedEvent* PreviewMouseMoveEvent;
    static const RoutedEvent* PreviewMouseRightButtonDownEvent;
    static const RoutedEvent* PreviewMouseRightButtonUpEvent;
    static const RoutedEvent* PreviewMouseUpEvent;
    static const RoutedEvent* PreviewMouseWheelEvent;
    static const RoutedEvent* PreviewTextInputEvent;
    static const RoutedEvent* QueryCursorEvent;
    static const RoutedEvent* TextInputEvent;
    static const RoutedEvent* PreviewQueryContinueDragEvent;
    static const RoutedEvent* QueryContinueDragEvent;
    static const RoutedEvent* PreviewGiveFeedbackEvent;
    static const RoutedEvent* GiveFeedbackEvent;
    static const RoutedEvent* PreviewDragEnterEvent;
    static const RoutedEvent* DragEnterEvent;
    static const RoutedEvent* PreviewDragOverEvent;
    static const RoutedEvent* DragOverEvent;
    static const RoutedEvent* PreviewDragLeaveEvent;
    static const RoutedEvent* DragLeaveEvent;
    static const RoutedEvent* PreviewDropEvent;
    static const RoutedEvent* DropEvent;
    //@}

protected:
    /// Adds a handler to the specified non-routed event
    void AddEventHandler(Symbol key, const EventHandler& handler);

    /// Removes a handler from the specified non-routed event
    void RemoveEventHandler(Symbol key, const EventHandler& handler);

    /// Raises a specific non-routed event
    void RaiseEvent(Symbol key, const EventArgs& args);

    /// Returns the delegate corresponding to the given non-routed event
    EventHandler& GetEventHandler(Symbol key);

    /// Indicates if the element has ever been measured or arranged
    //@{
    bool NeverMeasured() const;
    bool NeverArranged() const;
    //@}

    /// Indicates if element is being measured or arranged
    //@{
    bool IsInsideMeasure() const;
    bool IsInsideArrange() const;
    //@}

    /// Enqueues element for size changed
    void InvalidateSize(const Size& oldSize, const Size& newSize);

    /// Invalidates element for render
    void InvalidateRender();

    /// Indicates if render is valid
    bool IsRenderValid() const;

    /// When set to true, layout properties are ignored during Measure and Arrange
    void IgnoreLayout(bool ignore);

    /// Updates a layout flag
    //@{
    void SetLayoutFlag(uint32_t flag);
    void ClearLayoutFlag(uint32_t flag);
    //@}

    /// Checks if a flag is active
    bool CheckLayoutFlag(uint32_t flag) const;

    /// Rounds layout values
    //@{
    float RoundValue(float val) const;
    Size RoundSize(const Size& size) const;
    Point RoundPoint(const Point& point) const;
    Rect RoundRect(const Rect& rect) const;
    //@}

    /// Returns an alternative clipping geometry that represents the region that would be clipped
    /// if ClipToBounds were set to true
    virtual Ptr<Geometry> GetLayoutClip(const Size& layoutSlotSize) const;

    /// When overridden in a derived class, returns an alternative user interface parent for this
    /// element if no visual parent exists
    virtual UIElement* GetUIParentCore() const;

    /// Provides measurement logic for sizing this element properly, with consideration of the size
    /// of any child element content
    virtual Size MeasureCore(const Size& availableSize);

    /// Provides arrangement logic for placing child elements within this element
    virtual void ArrangeCore(const Rect& finalRect);

    /// Gets a value that becomes the return value of IsEnabled in derived classes. 
    virtual bool IsEnabledCore() const;

    /// Property changed handlers
    //@{
    virtual void OnVisibilityChanged(Visibility oldVisibility, Visibility newVisibility);
    virtual void OnRenderTransformChanged(Transform* oldTransform, Transform* newTransform);
    //@}

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// Event handlers
    ////////////////////////////////////////////////////////////////////////////////////////////////
    virtual void OnChildDesiredSizeChanged(UIElement* child);
    virtual void OnGotFocus(const RoutedEventArgs& e);
    virtual void OnGotKeyboardFocus(const KeyboardFocusChangedEventArgs& e);
    virtual void OnGotMouseCapture(const MouseEventArgs& e);
    virtual void OnGotTouchCapture(const TouchEventArgs& e);
    virtual void OnIsKeyboardFocusedChanged(const DependencyPropertyChangedEventArgs& e);
    virtual void OnIsKeyboardFocusWithinChanged(const DependencyPropertyChangedEventArgs& e);
    virtual void OnIsMouseCapturedChanged(const DependencyPropertyChangedEventArgs& e);
    virtual void OnIsMouseCaptureWithinChanged(const DependencyPropertyChangedEventArgs& e);
    virtual void OnIsMouseDirectlyOverChanged(const DependencyPropertyChangedEventArgs& e);
    virtual void OnKeyDown(const KeyEventArgs& e);
    virtual void OnKeyUp(const KeyEventArgs& e);
    virtual void OnLostFocus(const RoutedEventArgs& e);
    virtual void OnLostKeyboardFocus(const KeyboardFocusChangedEventArgs& e);
    virtual void OnLostMouseCapture(const MouseEventArgs& e);
    virtual void OnLostTouchCapture(const TouchEventArgs& e);
    virtual void OnMouseDown(const MouseButtonEventArgs& e);
    virtual void OnMouseEnter(const MouseEventArgs& e);
    virtual void OnMouseLeave(const MouseEventArgs& e);
    virtual void OnMouseLeftButtonDown(const MouseButtonEventArgs& e);
    virtual void OnMouseLeftButtonUp(const MouseButtonEventArgs& e);
    virtual void OnMouseMove(const MouseEventArgs& e);
    virtual void OnMouseRightButtonDown(const MouseButtonEventArgs& e);
    virtual void OnMouseRightButtonUp(const MouseButtonEventArgs& e);
    virtual void OnMouseUp(const MouseButtonEventArgs& e);
    virtual void OnMouseWheel(const MouseWheelEventArgs& e);
    virtual void OnTouchDown(const TouchEventArgs& e);
    virtual void OnTouchMove(const TouchEventArgs& e);
    virtual void OnTouchUp(const TouchEventArgs& e);
    virtual void OnTouchEnter(const TouchEventArgs& e);
    virtual void OnTouchLeave(const TouchEventArgs& e);
    virtual void OnPreviewTouchDown(const TouchEventArgs& e);
    virtual void OnPreviewTouchMove(const TouchEventArgs& e);
    virtual void OnPreviewTouchUp(const TouchEventArgs& e);
    virtual void OnManipulationStarting(const ManipulationStartingEventArgs& e);
    virtual void OnManipulationStarted(const ManipulationStartedEventArgs& e);
    virtual void OnManipulationDelta(const ManipulationDeltaEventArgs& e);
    virtual void OnManipulationInertiaStarting(const ManipulationInertiaStartingEventArgs& e);
    virtual void OnManipulationCompleted(const ManipulationCompletedEventArgs& e);
    virtual void OnTapped(const TappedEventArgs& e);
    virtual void OnDoubleTapped(const DoubleTappedEventArgs& e);
    virtual void OnHolding(const HoldingEventArgs& e);
    virtual void OnRightTapped(const RightTappedEventArgs& e);
    virtual void OnPreviewGotKeyboardFocus(const KeyboardFocusChangedEventArgs& e);
    virtual void OnPreviewKeyDown(const KeyEventArgs& e);
    virtual void OnPreviewKeyUp(const KeyEventArgs& e);
    virtual void OnPreviewLostKeyboardFocus(const KeyboardFocusChangedEventArgs& e);
    virtual void OnPreviewMouseDown(const MouseButtonEventArgs& e);
    virtual void OnPreviewMouseLeftButtonDown(const MouseButtonEventArgs& e);
    virtual void OnPreviewMouseLeftButtonUp(const MouseButtonEventArgs& e);
    virtual void OnPreviewMouseMove(const MouseEventArgs& e);
    virtual void OnPreviewMouseRightButtonDown(const MouseButtonEventArgs& e);
    virtual void OnPreviewMouseRightButtonUp(const MouseButtonEventArgs& e);
    virtual void OnPreviewMouseUp(const MouseButtonEventArgs& e);
    virtual void OnPreviewMouseWheel(const MouseWheelEventArgs& e);
    virtual void OnPreviewTextInput(const TextCompositionEventArgs& e);
    virtual void OnTextInput(const TextCompositionEventArgs& e);
    virtual void OnQueryCursor(const QueryCursorEventArgs& e);
    virtual void OnPreviewQueryContinueDrag(const QueryContinueDragEventArgs& e);
    virtual void OnQueryContinueDrag(const QueryContinueDragEventArgs& e);
    virtual void OnPreviewGiveFeedback(const GiveFeedbackEventArgs& e);
    virtual void OnGiveFeedback(const GiveFeedbackEventArgs& e);
    virtual void OnPreviewDragEnter(const DragEventArgs& e);
    virtual void OnDragEnter(const DragEventArgs& e);
    virtual void OnPreviewDragOver(const DragEventArgs& e);
    virtual void OnDragOver(const DragEventArgs& e);
    virtual void OnPreviewDragLeave(const DragEventArgs& e);
    virtual void OnDragLeave(const DragEventArgs& e);
    virtual void OnPreviewDrop(const DragEventArgs& e);
    virtual void OnDrop(const DragEventArgs& e);

    /// When overridden in a derived class, participates in rendering operations that are directed
    /// by the layout system. The rendering instructions for this element are not used directly
    /// when this method is invoked, and are instead preserved for later asynchronous use by layout
    /// and drawing
    virtual void OnRender(DrawingContext* drawingContext);

    virtual void OnRenderSizeChanged(const SizeChangedInfo& info);

    /// From DependencyObject
    //@{
    void OnInit() override;
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From Visual
    //@{
    Rect GetContentBoundsCore() const override;
    DrawingCommands* GetDrawingCommands() const override;
    void OnConnectToView(IView* view) override;
    void OnDisconnectFromView() override;
    void OnVisualParentChanged(Visual* oldParent) override;
    HitTestResult HitTestCore(const Point& point) override;
    //@}

public:
    uint32_t mLayoutFlags;
    Size mRenderSize;

public:
    friend class FrameworkElement;
    friend class UIElementCollection;
    friend class UserControl;
    friend class View;
    friend class ViewLayout;
    friend class Mouse;
    friend class Keyboard;
    friend class UIElementTest;
    friend class SymEventBind;

    /// Notifies event handlers
    void NotifyHandlers(const RoutedEventArgs& args);

    /// Notifies type event handlers
    void NotifyTypeHandlers(const TypeClass* type, const RoutedEventArgs& args);

    /// Begins a tunneling route for an event
    void TunnelingEvent(const RoutedEventArgs& args);

    /// Begins a bubbling route for an event
    void BubblingEvent(const RoutedEventArgs& args);

    /// Builds the route for a tunneling or bubbling event
    //@{
    typedef Vector<Ptr<UIElement>, 128> EventRoute;
    void BuildEventRoute(EventRoute& route);
    //@}

    /// Invalidates parent measure
    void InvalidateParentMeasure();

    /// Invalidates children measure and arrange when element gets collapsed
    void InvalidateChildren();

    /// Indicates if element is being measured while inside its arrange pass
    bool IsMeasureInsideArrange() const;

    /// Calculates measure pass of the element while inside its arrange pass
    void MeasureInsideArrange(const Size& availableSize);

    /// Applies clip geometry to underlying visual
    void ApplyClip(const Size& layoutSlotSize);

    /// Raises render size changed events
    void RaiseSizeChanged(const SizeChangedInfo& sizeInfo);

    void OnIsEnabledChanged(const DependencyPropertyChangedEventArgs& e);
    void OnIsFocusedChanged(const DependencyPropertyChangedEventArgs& e);
    void OnIsHitTestVisibleChanged(const DependencyPropertyChangedEventArgs& e);
    void OnIsVisibleChanged(const DependencyPropertyChangedEventArgs& e);

    void UpdateIsVisible(Visibility visibility);

    void UpdateCachedProperties();
    void UpdateCachedProperties(const DependencyPropertyChangedEventArgs& e);
    bool GetIsEnabledCached() const;
    bool GetIsHitTestVisibleCached() const;
    bool GetIsKeyboardFocusedCached() const;
    bool GetIsKeyboardFocusWithinCached() const;
    bool GetClipToBoundsCached() const;
    Visibility GetVisibilityCached() const;

    void UpdateLayoutFlag(uint32_t flag, bool value);

    void EnsureCommandBindings();
    void EnsureInputBindings();

    void EnsureDrawingContext();

    void EnqueueSizeChanged(const Size& prevSize, const Size& newSize);

    void CancelMeasureRequest();
    void CancelArrangeRequest();
    void CancelSizeChangedRequest();

    EventHandler& GetFocusableChangedEvent();
    EventHandler& GetIsEnabledChangedEvent();
    EventHandler& GetIsHitTestVisibleChangedEvent();
    EventHandler& GetIsVisibleChangedEvent();
    EventHandler& GetIsMouseCapturedChangedEvent();
    EventHandler& GetIsMouseCaptureWithinChangedEvent();
    EventHandler& GetIsMouseDirectlyOverChangedEvent();
    EventHandler& GetIsKeyboardFocusedChangedEvent();
    EventHandler& GetIsKeyboardFocusWithinChangedEvent();

    /// Promotes MouseDown/Up events to the corresponding Left/Right mouse button events
    void PromoteMouseButtonEvent(const MouseButtonEventArgs& e,
        const RoutedEvent* leftEvent, const RoutedEvent* rightEvent);

public:
    Size mDesiredSize;
    Size mPreviousMeasureConstraint;
    Rect mPreviousArrangeConstraint;

    typedef HashMap<const RoutedEvent*, RoutedEventHandler> RoutedEventMap;
    RoutedEventMap mRoutedEventHandlers;

    typedef HashMap<Symbol, EventHandler> EventMap;
    EventMap mEventHandlers;

    Ptr<DrawingContext> mDrawingContext;

    Ptr<CommandBindingCollection> mCommandBindings;
    Ptr<InputBindingCollection> mInputBindings;

    void* mMeasureRequest;
    void* mArrangeRequest;
    void* mSizeChangedRequest;

    NS_DECLARE_REFLECTION(UIElement, Visual)
};

NS_WARNING_POP

}


#endif
