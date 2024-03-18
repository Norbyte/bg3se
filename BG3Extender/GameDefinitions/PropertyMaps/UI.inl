
////////////////////////////////////////////////////////////////////////////////////////////////////
/// BASE CLASSES

BEGIN_CLS(Noesis::BaseObject)
P_FUN(TypeInfo, Noesis::ObjectHelpers::GetClassType)
P_FREE_GETTER(Type, Noesis::ObjectHelpers::GetClassTypeName)
P_FUN(ToString, Noesis::ObjectHelpers::ToString)
END_CLS()


BEGIN_CLS(Noesis::BaseRefCounted)
INHERIT(Noesis::BaseObject)
P_FREE_GETTER(NumReferences, Noesis::ObjectHelpers::GetNumReferences)
END_CLS()


BEGIN_CLS(Noesis::BaseComponent)
INHERIT(Noesis::BaseRefCounted)
END_CLS()


BEGIN_CLS(Noesis::TypeMetaData)
INHERIT(Noesis::BaseObject)
END_CLS()


BEGIN_CLS(Noesis::Type)
INHERIT(Noesis::BaseObject)
P_FREE_GETTER(Name, Noesis::TypeHelpers::GetName)
END_CLS()


BEGIN_CLS(Noesis::TypeMeta)
INHERIT(Noesis::Type)
// FindMeta, GetMeta
END_CLS()

BEGIN_CLS(Noesis::TypeProperty)
P_FREE_GETTER(Name, Noesis::TypePropertyHelpers::GetName)
P_FREE_GETTER(ContentType, Noesis::TypePropertyHelpers::GetContentType)
P_FREE_GETTER(Meta, Noesis::TypePropertyHelpers::GetMeta)
P_FREE_GETTER(IsReadOnly, Noesis::TypePropertyHelpers::IsReadOnly)
END_CLS()


BEGIN_CLS(Noesis::TypeClass)
INHERIT(Noesis::TypeMeta)
P_FREE_GETTER(Base, Noesis::TypeHelpers::GetBase)
P_FREE_GETTER(IsInterface, Noesis::TypeHelpers::IsInterface)
P_FREE_GETTER(Interfaces, Noesis::TypeHelpers::GetInterfaces)
P_FREE_GETTER(Properties, Noesis::TypeHelpers::GetProperties)
P_FREE_GETTER(Events, Noesis::TypeHelpers::GetEvents)
P_FREE_GETTER(Dependency, Noesis::TypeHelpers::GetDependencyData)
P_FREE_GETTER(UIElement, Noesis::TypeHelpers::GetUIElementData)
P_FUN(GetDependencyProperties, Noesis::TypeHelpers::GetDependencyProperties)
P_FUN(GetRoutedEvents, Noesis::TypeHelpers::GetRoutedEvents)
// FindProperty
// FindEvent
END_CLS()

BEGIN_CLS(Noesis::TypeClass::AncestorInfo)
P_RO(type)
END_CLS()


BEGIN_CLS(Noesis::DependencyProperty)
INHERIT(Noesis::BaseComponent)
P_FREE_GETTER(Name, Noesis::DependencyPropertyHelpers::GetName)
P_FREE_GETTER(ValueType, Noesis::DependencyPropertyHelpers::GetType)
P_FREE_GETTER(OwnerType, Noesis::DependencyPropertyHelpers::GetOwnerType)
P_FREE_GETTER(IsReadOnly, Noesis::DependencyPropertyHelpers::IsReadOnly)
END_CLS()

BEGIN_CLS(Noesis::DispatcherObject)
INHERIT(Noesis::BaseComponent)
END_CLS()


BEGIN_CLS(Noesis::DependencyObject)
INHERIT(Noesis::DispatcherObject)
P_FUN(GetProperty, Noesis::DependencyObjectHelpers::GetProperty)
P_FUN(GetAllProperties, Noesis::DependencyObjectHelpers::GetAllProperties)
P_FUN(SetProperty, Noesis::DependencyObjectHelpers::SetProperty)
P_FALLBACK(Noesis::DependencyObjectHelpers::FallbackGetProperty, Noesis::DependencyObjectHelpers::FallbackSetProperty)
END_CLS()


BEGIN_CLS(Noesis::DependencyData)
INHERIT(Noesis::TypeMetaData)
END_CLS()


BEGIN_CLS(Noesis::RoutedEvent)
INHERIT(Noesis::BaseComponent)
P_FREE_GETTER(Name, Noesis::RoutedEventHelpers::GetName)
P_FREE_GETTER(OwnerType, Noesis::RoutedEventHelpers::GetOwnerType)
END_CLS()



////////////////////////////////////////////////////////////////////////////////////////////////////
/// UI OBJECTS

BEGIN_CLS(Noesis::Visual)
INHERIT(Noesis::DependencyObject)
P_FREE_GETTER(VisualParent, Noesis::VisualHelpers::GetVisualParent)
P_FREE_GETTER(VisualChildrenCount, Noesis::VisualHelpers::GetVisualChildrenCount)
P_FUN(VisualChild, Noesis::VisualHelpers::GetVisualChild)
END_CLS()


BEGIN_CLS(Noesis::UIElementData)
INHERIT(Noesis::DependencyData)
P_FUN(GetEvent, Noesis::UIElementDataHelpers::GetEvent)
P_FUN(GetAllEvents, Noesis::UIElementDataHelpers::GetAllEvents)
END_CLS()


BEGIN_CLS(Noesis::UIElement)
INHERIT(Noesis::Visual)
P_FUN(Subscribe, Noesis::UIElementHelpers::Subscribe)
P_FUN(Unsubscribe, Noesis::UIElementHelpers::Unsubscribe)
END_CLS()


BEGIN_CLS(Noesis::FrameworkElement)
INHERIT(Noesis::UIElement)
P_FREE_GETTER(Parent, Noesis::FrameworkElementHelpers::GetLogicalParent)
P_FREE_GETTER(ChildrenCount, Noesis::FrameworkElementHelpers::GetLogicalChildrenCount)
P_FUN(Child, Noesis::FrameworkElementHelpers::GetLogicalChild)
P_FUN(Find, Noesis::FrameworkElementHelpers::FindNodeName)
P_FUN(Resource, Noesis::FrameworkElementHelpers::GetResource)
P_FUN(TreeParent, Noesis::FrameworkElementHelpers::GetTreeParent)
PN_RO(TemplatedParent, mTemplatedParent)
PN_RO(InheritanceParent, mInheritanceParent)
END_CLS()


BEGIN_CLS(Noesis::Panel)
INHERIT(Noesis::FrameworkElement)
END_CLS()


BEGIN_CLS(Noesis::Point)
PN(X, x)
PN(Y, y)
END_CLS()



////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS

BEGIN_CLS(Noesis::EventArgs)
END_CLS()

BEGIN_CLS(Noesis::RoutedEventArgs)
INHERIT(Noesis::EventArgs)
PN(Source, source)
PN(Handled, handled)
END_CLS()

BEGIN_CLS(Noesis::KeyboardFocusChangedEventArgs)
INHERIT(Noesis::RoutedEventArgs)
PN(OldFocus, oldFocus)
PN(NewFocus, newFocus)
END_CLS()

BEGIN_CLS(Noesis::KeyEventArgs)
INHERIT(Noesis::RoutedEventArgs)
PN(Key, key)
PN(OriginalKey, originalKey)
PN(KeyStates, keyStates)
END_CLS()

BEGIN_CLS(Noesis::MouseEventArgs)
INHERIT(Noesis::RoutedEventArgs)
PN(Position, position)
PN(LeftButton, leftButton)
PN(MiddleButton, middleButton)
PN(RightButton, rightButton)
PN(XButton1Button, xButton1Button)
PN(XButton2Button, xButton2Button)
END_CLS()

BEGIN_CLS(Noesis::MouseButtonEventArgs)
INHERIT(Noesis::MouseEventArgs)
PN(ChangedButton, changedButton)
PN(ButtonState, buttonState)
PN(ClickCount, clickCount)
END_CLS()

BEGIN_CLS(Noesis::MouseWheelEventArgs)
INHERIT(Noesis::MouseEventArgs)
PN(WheelRotation, wheelRotation)
PN(Orientation, orientation)
END_CLS()

BEGIN_CLS(Noesis::TouchEventArgs)
INHERIT(Noesis::RoutedEventArgs)
PN(TouchPoint, touchPoint)
PN(TouchDevice, touchDevice)
END_CLS()

BEGIN_CLS(Noesis::TappedEventArgs)
INHERIT(Noesis::TouchEventArgs)
END_CLS()

BEGIN_CLS(Noesis::DoubleTappedEventArgs)
INHERIT(Noesis::TouchEventArgs)
END_CLS()

BEGIN_CLS(Noesis::RightTappedEventArgs)
INHERIT(Noesis::TouchEventArgs)
END_CLS()

BEGIN_CLS(Noesis::HoldingEventArgs)
INHERIT(Noesis::TouchEventArgs)
PN(HoldingState, holdingState)
END_CLS()

BEGIN_CLS(Noesis::ManipulationStartingEventArgs)
INHERIT(Noesis::RoutedEventArgs)
END_CLS()

BEGIN_CLS(Noesis::ManipulationStartedEventArgs)
INHERIT(Noesis::RoutedEventArgs)
END_CLS()

BEGIN_CLS(Noesis::ManipulationDeltaEventArgs)
INHERIT(Noesis::RoutedEventArgs)
END_CLS()

BEGIN_CLS(Noesis::ManipulationInertiaStartingEventArgs)
INHERIT(Noesis::RoutedEventArgs)
END_CLS()

BEGIN_CLS(Noesis::ManipulationCompletedEventArgs)
INHERIT(Noesis::RoutedEventArgs)
END_CLS()

BEGIN_CLS(Noesis::TextCompositionEventArgs)
INHERIT(Noesis::RoutedEventArgs)
PN(Ch, ch)
END_CLS()

BEGIN_CLS(Noesis::QueryCursorEventArgs)
INHERIT(Noesis::RoutedEventArgs)
// PN(Cursor, cursor)
END_CLS()

BEGIN_CLS(Noesis::QueryContinueDragEventArgs)
INHERIT(Noesis::RoutedEventArgs)
PN(EscapePressed, escapePressed)
PN(KeyStates, keyStates)
PN(Action, action)
END_CLS()

BEGIN_CLS(Noesis::GiveFeedbackEventArgs)
INHERIT(Noesis::RoutedEventArgs)
PN(Effects, effects)
PN(UseDefaultCursors, useDefaultCursors)
END_CLS()

BEGIN_CLS(Noesis::DragEventArgs)
INHERIT(Noesis::RoutedEventArgs)
PN(Data, data)
PN(KeyStates, keyStates)
PN(AllowedEffects, allowedEffects)
PN(Effects, effects)
END_CLS()
