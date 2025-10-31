////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TOGGLEBUTTON_H__
#define __GUI_TOGGLEBUTTON_H__


#include <NsCore/Noesis.h>
#include <NsGui/BaseButton.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

template<class T> class Nullable;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for button controls that can switch states.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.togglebutton.aspx#inheritanceContinued
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ToggleButton: public BaseButton
{
public:
    ToggleButton();
    ToggleButton(const ToggleButton&) = delete;
    ToggleButton& operator=(const ToggleButton&) = delete;

    /// Gets or sets whether the ToggleButton is checked.
    //@{
    const Nullable<bool>& GetIsChecked() const;
    void SetIsChecked(const Nullable<bool>& checked);
    //@}

    /// Determines whether the control supports two or three states
    //@{
    bool GetIsThreeState() const;
    void SetIsThreeState(bool state);
    //@}

    /// Occurs when a ToggleButton is checked.
    UIElement::RoutedEvent_<RoutedEventHandler> Checked();

    /// Occurs when the state of a ToggleButton is neither on nor off.
    UIElement::RoutedEvent_<RoutedEventHandler> Indeterminate();

    /// Occurs when a ToggleButton is unchecked.
    UIElement::RoutedEvent_<RoutedEventHandler> Unchecked();

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* IsCheckedProperty;
    static const DependencyProperty* IsThreeStateProperty;
    //@}

    /// Dependency events
    //@{
    static const RoutedEvent* CheckedEvent;
    static const RoutedEvent* IndeterminateEvent;
    static const RoutedEvent* UncheckedEvent;
    //@}

protected:
    /// Called when a ToggleButton raises a Checked event
    virtual void OnChecked(const RoutedEventArgs& e);

    /// Called when a ToggleButton raises an Indeterminate event
    virtual void OnIndeterminate(const RoutedEventArgs& e);

    /// Called by the OnClick() method to implement toggle behavior
    virtual void OnToggle();

    /// Called when a ToggleButton raises an Unchecked event
    virtual void OnUnchecked(const RoutedEventArgs& e);

    /// From BaseButton
    //@{
    void OnClick();
    //@}

    /// From Control
    //@{
    void UpdateVisualStates();
    //@}

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

private:
    NS_DECLARE_REFLECTION(ToggleButton, BaseButton)
};

}


#endif
