////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FOCUSMANAGER_H__
#define __GUI_FOCUSMANAGER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/BaseComponent.h>


namespace Noesis
{

class DependencyObject;
class DependencyProperty;
class RoutedEvent;
class UIElement;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a set of static methods, attached properties, and events for determining and setting
/// focus scopes and for setting the focused element within the scope.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.input.focusmanager.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API FocusManager
{
public:
    /// Gets or sets the element with logical focus within the specified focus scope
    //@{
    static UIElement* GetFocusedElement(DependencyObject* element);
    static void SetFocusedElement(DependencyObject* element, UIElement* value);
    //@}

    /// Determines the closest ancestor of the specified element that has IsFocusScope set to true
    static DependencyObject* GetFocusScope(DependencyObject* element);

    /// Determines whether the specified DependencyObject is a focus scope
    //@{
    static bool GetIsFocusScope(DependencyObject* element);
    static void SetIsFocusScope(DependencyObject* element, bool value);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* FocusedElementProperty;
    static const DependencyProperty* IsFocusScopeProperty;
    //@}

    /// Routed events
    //@{
    static const RoutedEvent* LostFocusEvent;
    static const RoutedEvent* GotFocusEvent;
    //@}

    NS_DECLARE_REFLECTION(FocusManager, NoParent)
};

}

#endif
