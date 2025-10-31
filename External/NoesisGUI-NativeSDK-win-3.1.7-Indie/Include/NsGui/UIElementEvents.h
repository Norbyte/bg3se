////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_UIELEMENTEVENTARGS_H__
#define __GUI_UIELEMENTEVENTARGS_H__


#include <NsCore/Noesis.h>
#include <NsCore/String.h>
#include <NsCore/ArrayRef.h>
#include <NsGui/RoutedEvent.h>
#include <NsGui/InputEnums.h>
#include <NsGui/Enums.h>
#include <NsGui/Cursor.h>
#include <NsDrawing/Point.h>


namespace Noesis
{

class UIElement;
class Visual;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for input related events.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct InputEventArgs: public RoutedEventArgs
{
    InputEventArgs(BaseComponent* source, const RoutedEvent* event);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for mouse related routed events that do not specifically involve mouse buttons or
/// the mouse wheel.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct MouseEventArgs: public InputEventArgs
{
    Point position;
    MouseButtonState leftButton;
    MouseButtonState middleButton;
    MouseButtonState rightButton;
    MouseButtonState xButton1Button;
    MouseButtonState xButton2Button;

    NS_GUI_CORE_API MouseEventArgs(BaseComponent* source, const RoutedEvent* event);
};

typedef Delegate<void (BaseComponent*, const MouseEventArgs&)> MouseEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for mouse button related events.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct MouseButtonEventArgs: public MouseEventArgs
{
    MouseButton changedButton;
    MouseButtonState buttonState;
    uint32_t clickCount;

    MouseButtonEventArgs(BaseComponent* source, const RoutedEvent* event, MouseButton button,
        MouseButtonState state, uint32_t clickCount);
};

typedef Delegate<void (BaseComponent*, const MouseButtonEventArgs&)> MouseButtonEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for various events that report changes to the mouse wheel delta value of a mouse.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct MouseWheelEventArgs: public MouseEventArgs
{
    int wheelRotation;
    Orientation orientation;

    MouseWheelEventArgs(BaseComponent* source, const RoutedEvent* event, int rotation,
        Orientation orientation = Orientation_Vertical);
};

typedef Delegate<void (BaseComponent*, const MouseWheelEventArgs&)> MouseWheelEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for touch related routed events.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TouchEventArgs: public InputEventArgs
{
    Point touchPoint;
    uint64_t touchDevice;

    /// Returns the current position of the touch device relative to the specified element
    Point GetTouchPoint(UIElement* relativeTo) const;

    TouchEventArgs(BaseComponent* source, const RoutedEvent* event, const Point& p, uint64_t device);
};

typedef Delegate<void (BaseComponent*, const TouchEventArgs&)> TouchEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a touch contact in a manipulation
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Manipulator
{
    uint64_t touchDevice;
    Point position;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the ManipulationStarting event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct ManipulationStartingEventArgs: public InputEventArgs
{
    /// The container that all manipulation events and calculations are relative to
    mutable Visual* manipulationContainer;

    /// Collection of touch contacts for the manipulation
    ArrayRef<Manipulator> manipulators;

    /// Which types of manipulations are possible
    mutable ManipulationModes mode = ManipulationModes_All;

    /// Cancels the manipulation and promotes touch to mouse events
    mutable bool cancel = false;

    ManipulationStartingEventArgs(BaseComponent* source, const RoutedEvent* event,
        Visual* container, ArrayRef<Manipulator> manipulators);
};

typedef Delegate<void (BaseComponent*, const ManipulationStartingEventArgs&)> ManipulationStartingEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the ManipulationStarted event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct ManipulationStartedEventArgs: public InputEventArgs
{
    /// The container that the manipulationOrigin member is relative to
    Visual* manipulationContainer;

    /// The point from which the manipulation originated
    Point manipulationOrigin;

    /// Collection of touch contacts for the manipulation
    ArrayRef<Manipulator> manipulators;

    /// Cancels the manipulation
    mutable bool cancel = false;

    /// Completes the manipulation without inertia
    mutable bool complete = false;

    ManipulationStartedEventArgs(BaseComponent* source, const RoutedEvent* event, Visual* container,
        const Point& origin, ArrayRef<Manipulator> manipulators);
};

typedef Delegate<void (BaseComponent*, const ManipulationStartedEventArgs&)> ManipulationStartedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Contains transformation data that is accumulated when manipulation events occur
////////////////////////////////////////////////////////////////////////////////////////////////////
struct ManipulationDelta
{
    /// The amount the manipulation has resized in pixels
    Point expansion;
    /// The rotation of the manipulation in degrees
    float rotation;
    /// The amount the manipulation has resized as a multiplier
    float scale;
    /// The linear motion of the manipulation in pixels
    Point translation;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes the speed at which manipulations occurs
////////////////////////////////////////////////////////////////////////////////////////////////////
struct ManipulationVelocities
{
    /// The speed of rotation in degrees per millisecond
    float angularVelocity;
    /// The rate at which the manipulation is resized in pixels per millisecond
    Point expansionVelocity;
    /// The speed of linear motion in pixels per millisecond
    Point linearVelocity;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the ManipulationDelta event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct ManipulationDeltaEventArgs: public InputEventArgs
{
    /// The container that defines the coordinates for the manipulation
    Visual* manipulationContainer;

    /// The point from which the manipulation originated
    Point manipulationOrigin;

    /// The most recent changes of the current manipulation
    ManipulationDelta deltaManipulation;

    /// The cumulated changes of the current manipulation
    ManipulationDelta cumulativeManipulation;

    /// The rates of the most recent changes to the manipulation
    ManipulationVelocities velocities;

    /// Indicates whether the ManipulationDelta event occurs during inertia
    bool isInertial;

    /// Collection of touch contacts for the manipulation
    ArrayRef<Manipulator> manipulators;

    /// Cancels the manipulation
    mutable bool cancel = false;

    /// Completes the manipulation without inertia
    mutable bool complete = false;

    ManipulationDeltaEventArgs(BaseComponent* source, const RoutedEvent* event, Visual* container,
        const Point& origin, const ManipulationDelta& delta, const ManipulationDelta& cumulative,
        const ManipulationVelocities& velocities, bool isInertial, ArrayRef<Manipulator> manipulators);
};

typedef Delegate<void (BaseComponent*, const ManipulationDeltaEventArgs&)> ManipulationDeltaEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Controls the deceleration of a resizing manipulation during inertia
////////////////////////////////////////////////////////////////////////////////////////////////////
struct InertiaExpansionBehavior
{
    /// The rate that resizing slows in pixels per square milliseconds
    float desiredDeceleration = FLT_NAN;

    /// The amount the element resizes at the end of inertia
    float desiredExpansion = FLT_NAN;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Controls the deceleration of a rotation manipulation during inertia
////////////////////////////////////////////////////////////////////////////////////////////////////
struct InertiaRotationBehavior
{
    /// The rate the rotation slows in degrees per squared millisecond
    float desiredDeceleration = FLT_NAN;

    /// The rotation, in degrees, at the end of the inertial movement
    float desiredRotation = FLT_NAN;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Controls deceleration on a translation manipulation during inertia
////////////////////////////////////////////////////////////////////////////////////////////////////
struct InertiaTranslationBehavior
{
    /// The rate the linear movement slows in pixels per squared millisecond
    float desiredDeceleration = FLT_NAN;

    /// The linear movement of the manipulation at the end of inertia
    float desiredDisplacement = FLT_NAN;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the ManipulationInertiaStarting event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct ManipulationInertiaStartingEventArgs: public InputEventArgs
{
    /// The container that the ManipulationOrigin property is relative to
    Visual* manipulationContainer;

    /// The point from which the manipulation originated
    Point manipulationOrigin;

    /// The rates of changes to the manipulation that occur before inertia starts
    mutable ManipulationVelocities initialVelocities;

    /// The rate of slowdown of rotational inertial movement
    mutable InertiaRotationBehavior rotationBehavior;

    /// The rate of slowdown of expansion inertial movement
    mutable InertiaExpansionBehavior expansionBehavior;

    /// The rate of slowdown of linear inertial movement
    mutable InertiaTranslationBehavior translationBehavior;

    /// Collection of touch contacts for the manipulation
    ArrayRef<Manipulator> manipulators;

    /// Cancels the manipulation
    mutable bool cancel = false;

    ManipulationInertiaStartingEventArgs(BaseComponent* source, const RoutedEvent* event,
        Visual* container, const Point& origin, const ManipulationVelocities& velocities,
        ArrayRef<Manipulator> manipulators);
};

typedef Delegate<void (BaseComponent*, const ManipulationInertiaStartingEventArgs&)>
    ManipulationInertiaStartingEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the ManipulationCompleted event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct ManipulationCompletedEventArgs: public InputEventArgs
{
    /// The container that defines the coordinates for the manipulation
    Visual* manipulationContainer;

    /// The point from which the manipulation originated
    Point manipulationOrigin;

    /// The velocities that are used for the manipulation
    ManipulationVelocities finalVelocities;

    /// The total transformation that occurs during the current manipulation
    ManipulationDelta totalManipulation; 

    /// Indicates whether the ManipulationDelta event occurs during inertia
    bool isInertial;

    /// Collection of touch contacts for the manipulation
    ArrayRef<Manipulator> manipulators;

    /// Cancels the manipulation
    mutable bool cancel = false;

    ManipulationCompletedEventArgs(BaseComponent* source, const RoutedEvent* event,
        Visual* container, const Point& origin, const ManipulationVelocities& velocities,
        const ManipulationDelta& totalManipulation, bool isInertial,
        ArrayRef<Manipulator> manipulators);
};

typedef Delegate<void (BaseComponent*, const ManipulationCompletedEventArgs&)>
    ManipulationCompletedEventHandler;


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides event data for the Tapped event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TappedEventArgs: public TouchEventArgs
{
    TappedEventArgs(BaseComponent* source, const RoutedEvent* event, const Point& p,
        uint64_t device);
};

typedef Delegate<void (BaseComponent*, const TappedEventArgs&)> TappedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides event data for the DoubleTapped event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct DoubleTappedEventArgs: public TouchEventArgs
{
    DoubleTappedEventArgs(BaseComponent* source, const RoutedEvent* event, const Point& p,
        uint64_t device);
};

typedef Delegate<void (BaseComponent*, const DoubleTappedEventArgs&)> DoubleTappedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies the state of the Holding event
////////////////////////////////////////////////////////////////////////////////////////////////////
enum HoldingState: int32_t
{
    HoldingState_Started,
    HoldingState_Completed,
    HoldingState_Canceled
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides event data for the Holding event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct HoldingEventArgs: public TouchEventArgs
{
    HoldingState holdingState;

    HoldingEventArgs(BaseComponent* source, const RoutedEvent* event, const Point& p,
        uint64_t device, HoldingState holdingState);
};

typedef Delegate<void (BaseComponent*, const HoldingEventArgs&)> HoldingEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides event data for the RightTapped event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct RightTappedEventArgs: public TouchEventArgs
{
    RightTappedEventArgs(BaseComponent* source, const RoutedEvent* event, const Point& p,
        uint64_t device);
};

typedef Delegate<void (BaseComponent*, const RightTappedEventArgs&)> RightTappedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides information for access keys events.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct AccessKeyEventArgs: public EventArgs
{
    /// Gets a value that indicates whether other elements are invoked by the key.
    bool isMultiple;

    /// Gets the access keys that was pressed.
    String key;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for keyboard-related events.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct KeyboardEventArgs: public InputEventArgs
{
    KeyboardEventArgs(BaseComponent* s, const RoutedEvent* e);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for LostKeyboardFocus and GotKeyboardFocus routed events, as well as related
/// attached and Preview events.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct KeyboardFocusChangedEventArgs: public KeyboardEventArgs
{
    /// Gets the element that previously had focus.
    UIElement* oldFocus;

    /// Gets the element that focus has moved to.
    UIElement* newFocus;

    KeyboardFocusChangedEventArgs(BaseComponent* source, const RoutedEvent* event,
        UIElement* oldFocus, UIElement* newFocus);
};

typedef Delegate<void (BaseComponent*, const KeyboardFocusChangedEventArgs&)>
    KeyboardFocusChangedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the KeyUp and KeyDown routed events, as well as related attached and Preview
/// events.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct KeyEventArgs: public KeyboardEventArgs
{
    /// Gets the keyboard key associated with the event.
    Key key;

    /// Gets the original key, unmapped input button associated with the event.
    Key originalKey;

    /// Gets the state of the keyboard key associated with this event.
    KeyStates keyStates;

    /// Gets a value that indicates whether the key referenced by the event is in the down state
    bool GetIsDown() const;

    /// Gets a value that indicates whether the keyboard key referenced by the event is a
    /// repeated key
    bool GetIsRepeat() const;

    /// Gets a value that indicates whether the key referenced by the event is in the toggled state
    bool GetIsToggled() const;

    /// Gets a value that indicates whether the key referenced by the event is in the up state
    bool GetIsUp() const;

    KeyEventArgs(BaseComponent* source, const RoutedEvent* event, Key key, KeyStates keyStates);
};

typedef Delegate<void (BaseComponent*, const KeyEventArgs&)> KeyEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Contains arguments associated with changes to a TextComposition.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TextCompositionEventArgs: public InputEventArgs
{
    /// Gets input text associated with a TextComposition event.
    uint32_t ch;

    TextCompositionEventArgs(BaseComponent* source, const RoutedEvent* event, uint32_t ch);
};

typedef Delegate<void (BaseComponent*, const TextCompositionEventArgs&)> TextCompositionEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides data for the QueryCursor event.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct QueryCursorEventArgs: public MouseEventArgs
{
    /// The cursor to set
    mutable Cursor* cursor;

    QueryCursorEventArgs(BaseComponent* source, const RoutedEvent* event);
};

typedef Delegate<void (BaseComponent*, const QueryCursorEventArgs&)> QueryCursorEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
struct QueryContinueDragEventArgs final: public RoutedEventArgs
{
    /// Indicates if Escape key was pressed
    bool escapePressed;

    /// Indicates current states for physical keyboard keys and mouse buttons (DragDropKeyStates)
    uint32_t keyStates;

    /// The action of drag operation
    mutable DragAction action = DragAction_Continue;

    QueryContinueDragEventArgs(BaseComponent* source, const RoutedEvent* event, bool escapePressed,
        uint32_t keyStates);
};

typedef Delegate<void (BaseComponent*, const QueryContinueDragEventArgs&)> QueryContinueDragEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
struct GiveFeedbackEventArgs final: public RoutedEventArgs
{
    /// The effects of drag operation (DragDropEffects)
    uint32_t effects;

    /// Indicates if default cursors should be used
    mutable bool useDefaultCursors;

    GiveFeedbackEventArgs(BaseComponent* source, const RoutedEvent* event, uint32_t effects,
        bool useDefaultCursors);
};

typedef Delegate<void (BaseComponent*, const GiveFeedbackEventArgs&)> GiveFeedbackEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
struct DragEventArgs final: public RoutedEventArgs
{
    /// The data object of drop operation
    BaseComponent* data;

    /// Indicates current states for physical keyboard keys and mouse buttons (DragDropKeyStates)
    uint32_t keyStates;

    /// The allowed effects of drag and drop operation (DragDropEffects)
    uint32_t allowedEffects;

    /// The effects of drag and drop operation (DragDropEffects)
    mutable uint32_t effects;

    /// Returns the point of drop operation that based on relativeTo
    Point GetPosition(UIElement* relativeTo) const;

    DragEventArgs(BaseComponent* source, const RoutedEvent* event, BaseComponent* data,
        uint32_t keyStates, uint32_t allowedEffects, UIElement* target, const Point& point);

private:
    UIElement* target;
    Point dropPoint;
};

typedef Delegate<void (BaseComponent*, const DragEventArgs&)> DragEventHandler;

}

#include <NsGui/UIElementEvents.inl>

#endif
