////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VISUALSTATEMANAGER_H__
#define __GUI_VISUALSTATEMANAGER_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/DependencyObject.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class FrameworkElement;
class VisualStateGroup;
class VisualTransition;

template<class T> class UICollection;
typedef UICollection<VisualStateGroup> VisualStateGroupCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Manages states and the logic for transitioning between states for controls.
///
/// The VisualStateManager enables you to specify states for a control, the appearance of a control
/// when it is in a certain state, and when a control changes states.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.visualstatemanager.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API VisualStateManager: public DependencyObject
{
public:
    VisualStateManager();
    ~VisualStateManager();

    /// Gets the CustomVisualStateManager attached property.
    static VisualStateManager* GetCustomVisualStateManager(const DependencyObject* obj);
    
    /// Sets the CustomVisualStateManager attached property.
    static void SetCustomVisualStateManager(DependencyObject* obj, VisualStateManager* value);

    /// Gets the VisualStateGroupCollection attached property.
    static VisualStateGroupCollection* GetVisualStateGroups(const DependencyObject* obj);
    
    /// Sets the VisualStateGroupCollection attached property.
    static void SetVisualStateGroups(DependencyObject* obj, VisualStateGroupCollection* groups);
    
    /// Transitions the control between two states. Use this method to transition states on control
    /// that has a ControlTemplate.
    /// \param control The control to transition between states
    /// \param stateName The state to transition to
    /// \param useTransitions true to use a VisualTransition to transition between states, 
    ///     otherwise false.
    /// \return true if the control successfully transitioned to the new state, otherwise false.
    static bool GoToState(FrameworkElement* control, Symbol stateName, bool useTransitions);
    
    /// Transitions the element between two states. Use this method to transition states that are 
    /// defined by an application, rather than defined by a control.
    /// \param root The root element that contains the VisualStateManager
    /// \param stateName The state to transition to.
    /// \param useTransitions true to use a VisualTransition object to transition between states
    static bool GoToElementState(FrameworkElement* root, Symbol stateName,
        bool useTransitions);

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* CustomVisualStateManagerProperty; // Attached
    static const DependencyProperty* VisualStateGroupsProperty; // Attached
    //@}

    NS_DECLARE_REFLECTION(VisualStateManager, DependencyObject)
};

}


#endif
