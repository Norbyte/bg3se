////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_GUI_CORE_API Point GetRelativePosition(const Point& p, UIElement* relativeTo);

////////////////////////////////////////////////////////////////////////////////////////////////////
inline InputEventArgs::InputEventArgs(BaseComponent* s, const RoutedEvent* e): RoutedEventArgs(s, e) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline MouseButtonEventArgs::MouseButtonEventArgs(BaseComponent* s, const RoutedEvent* e,
    MouseButton button, MouseButtonState state, uint32_t clicks): MouseEventArgs(s, e),
    changedButton(button), buttonState(state), clickCount(clicks) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline MouseWheelEventArgs::MouseWheelEventArgs(BaseComponent* s, const RoutedEvent* e, int delta,
    Orientation o) : MouseEventArgs(s, e), wheelRotation(delta), orientation(o) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Point TouchEventArgs::GetTouchPoint(UIElement* relativeTo) const
{
    NS_CHECK(relativeTo != nullptr, "GetTouchPoint relativeTo argument is null");
    return GetRelativePosition(touchPoint, relativeTo);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline TouchEventArgs::TouchEventArgs(BaseComponent* s, const RoutedEvent* e, const Point& touchPoint_,
    uint64_t touchDevice_) : InputEventArgs(s, e), touchPoint(touchPoint_), touchDevice(touchDevice_) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ManipulationStartingEventArgs::ManipulationStartingEventArgs(BaseComponent* s,
    const RoutedEvent* e, Visual* container, ArrayRef<Manipulator> manipulators_):
    InputEventArgs(s, e), manipulationContainer(container), manipulators(manipulators_) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ManipulationStartedEventArgs::ManipulationStartedEventArgs(BaseComponent* s,
    const RoutedEvent* e, Visual* container, const Point& origin,
    ArrayRef<Manipulator> manipulators_): InputEventArgs(s, e), manipulationContainer(container),
    manipulationOrigin(origin), manipulators(manipulators_) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ManipulationDeltaEventArgs::ManipulationDeltaEventArgs(BaseComponent* s, const RoutedEvent* e,
    Visual* container, const Point& origin, const ManipulationDelta& delta,
    const ManipulationDelta& cumulative, const ManipulationVelocities& velocities_,
    bool isInertial_, ArrayRef<Manipulator> manipulators_): InputEventArgs(s, e),
    manipulationContainer(container), manipulationOrigin(origin), deltaManipulation(delta),
    cumulativeManipulation(cumulative), velocities(velocities_), isInertial(isInertial_),
    manipulators(manipulators_) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ManipulationInertiaStartingEventArgs::ManipulationInertiaStartingEventArgs(BaseComponent* s,
    const RoutedEvent* e, Visual* container, const Point& origin,
    const ManipulationVelocities& velocities, ArrayRef<Manipulator> manipulators_):
    InputEventArgs(s, e), manipulationContainer(container), manipulationOrigin(origin),
    initialVelocities(velocities), manipulators(manipulators_) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline ManipulationCompletedEventArgs::ManipulationCompletedEventArgs(BaseComponent* s,
    const RoutedEvent* e, Visual* container, const Point& origin,
    const ManipulationVelocities& velocities, const ManipulationDelta& totalManipulation_,
    bool isInertial_, ArrayRef<Manipulator> manipulators_) : InputEventArgs(s, e),
    manipulationContainer(container), manipulationOrigin(origin), finalVelocities(velocities),
    totalManipulation(totalManipulation_), isInertial(isInertial_), manipulators(manipulators_) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline TappedEventArgs::TappedEventArgs(BaseComponent* source, const RoutedEvent* event,
    const Point& p, uint64_t device): TouchEventArgs(source, event, p, device) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline DoubleTappedEventArgs::DoubleTappedEventArgs(BaseComponent* source, const RoutedEvent* event,
    const Point& p, uint64_t device): TouchEventArgs(source, event, p, device) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline HoldingEventArgs::HoldingEventArgs(BaseComponent* source, const RoutedEvent* event,
    const Point& p, uint64_t device, HoldingState holdingState_):
    TouchEventArgs(source, event, p, device), holdingState(holdingState_) {};

////////////////////////////////////////////////////////////////////////////////////////////////////
inline RightTappedEventArgs::RightTappedEventArgs(BaseComponent* source, const RoutedEvent* event,
    const Point& p, uint64_t device): TouchEventArgs(source, event, p, device) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline KeyboardEventArgs::KeyboardEventArgs(BaseComponent* s, const RoutedEvent* e):
    InputEventArgs(s, e) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline KeyboardFocusChangedEventArgs::KeyboardFocusChangedEventArgs(BaseComponent* s,
    const RoutedEvent* e, UIElement* o, UIElement* n) : KeyboardEventArgs(s, e), oldFocus(o), newFocus(n) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool KeyEventArgs::GetIsDown() const
{
    return (keyStates & KeyStates_Down) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool KeyEventArgs::GetIsRepeat() const
{
    return (keyStates & KeyStates_Down) != 0 && (keyStates & KeyStates_Toggled) == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool KeyEventArgs::GetIsToggled() const
{
    return (keyStates & KeyStates_Toggled) != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool KeyEventArgs::GetIsUp() const
{
    return (keyStates & KeyStates_Down) == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline KeyEventArgs::KeyEventArgs(BaseComponent* s, const RoutedEvent* e, Key k, KeyStates ks):
    KeyboardEventArgs(s, e), keyStates(ks)
{
    originalKey = k;

    switch (k)
    {
        case Key_GamepadLeft: key = Key_Left; break;
        case Key_GamepadUp: key = Key_Up; break;
        case Key_GamepadRight: key = Key_Right; break;
        case Key_GamepadDown: key = Key_Down; break;
        case Key_GamepadAccept: key = Key_Space; break;
        case Key_GamepadCancel: key = Key_Escape; break;
        case Key_GamepadPageUp: key = Key_PageUp; break;
        case Key_GamepadPageDown: key = Key_PageDown; break;
        case Key_GamepadPageLeft: key = Key_PageLeft; break;
        case Key_GamepadPageRight: key = Key_PageRight; break;
        default: key = k; break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline TextCompositionEventArgs::TextCompositionEventArgs(BaseComponent* s, const RoutedEvent* e,
    uint32_t ch_) : InputEventArgs(s, e), ch(ch_) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline QueryCursorEventArgs::QueryCursorEventArgs(BaseComponent* s, const RoutedEvent* e):
    MouseEventArgs(s, e) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline QueryContinueDragEventArgs::QueryContinueDragEventArgs(BaseComponent* source,
    const RoutedEvent* event, bool escapePressed_, uint32_t keyStates_):
    RoutedEventArgs(source, event), escapePressed(escapePressed_), keyStates(keyStates_) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline GiveFeedbackEventArgs::GiveFeedbackEventArgs(BaseComponent* source, const RoutedEvent* event,
    uint32_t effects_, bool useDefaultCursors_): RoutedEventArgs(source, event), effects(effects_),
    useDefaultCursors(useDefaultCursors_) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Point DragEventArgs::GetPosition(UIElement* relativeTo) const
{
    NS_CHECK(relativeTo != nullptr, "GetPosition relativeTo argument is null");
    return GetRelativePosition(dropPoint, relativeTo);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline DragEventArgs::DragEventArgs(BaseComponent* source, const RoutedEvent* event,
    BaseComponent* data_, uint32_t keyStates_, uint32_t allowedEffects_, UIElement* target_,
    const Point& point): RoutedEventArgs(source, event), data(data_), keyStates(keyStates_),
        allowedEffects(allowedEffects_), effects(allowedEffects_), target(target_), dropPoint(point)
{
    NS_ASSERT(target != nullptr);
}

}
