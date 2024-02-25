BEGIN_CLS(Noesis::BaseObject)
P_FUN(TypeInfo, Noesis::ObjectHelpers::GetClassType)
P_FUN(Type, Noesis::ObjectHelpers::GetClassTypeName)
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
//P_FALLBACK(Noesis::DependencyObjectHelpers::FallbackGetProperty, Noesis::DependencyObjectHelpers::FallbackSetProperty)
END_CLS()


BEGIN_CLS(Noesis::DependencyData)
INHERIT(Noesis::TypeMetaData)
END_CLS()


BEGIN_CLS(Noesis::RoutedEvent)
INHERIT(Noesis::BaseComponent)
P_FREE_GETTER(Name, Noesis::RoutedEventHelpers::GetName)
P_FREE_GETTER(OwnerType, Noesis::RoutedEventHelpers::GetOwnerType)
END_CLS()


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
END_CLS()


BEGIN_CLS(Noesis::FrameworkElement)
INHERIT(Noesis::UIElement)
P_FREE_GETTER(Parent, Noesis::FrameworkElementHelpers::GetLogicalParent)
P_FREE_GETTER(ChildrenCount, Noesis::FrameworkElementHelpers::GetLogicalChildrenCount)
P_FUN(Child, Noesis::FrameworkElementHelpers::GetLogicalChild)
END_CLS()


BEGIN_CLS(Noesis::Panel)
INHERIT(Noesis::FrameworkElement)
END_CLS()
