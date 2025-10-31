////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_REPEATBUTTON_H__
#define __GUI_REPEATBUTTON_H__


#include <NsCore/Noesis.h>
#include <NsGui/BaseButton.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that raises its *Click* event repeatedly from the time it is pressed until
/// it is released.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.repeatbutton.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API RepeatButton: public BaseButton
{
public:
    RepeatButton();
    ~RepeatButton();

    /// Gets or sets the amount of time, in milliseconds, the RepeatButton waits while it is
    /// pressed before it starts repeating. The value must be non-negative
    //@{
    int32_t GetDelay() const;
    void SetDelay(int32_t delay);
    //@}

    /// Gets or sets the amount of time, in milliseconds, between repeats once repeating starts.
    /// The value must be non-negative
    //@{
    int32_t GetInterval() const;
    void SetInterval(int32_t interval);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* DelayProperty;
    static const DependencyProperty* IntervalProperty;
    //@}

protected:
    /// From BaseButton
    //@{
    void OnClick();
    //@}

    /// From UIElement
    //@{
    void OnKeyDown(const KeyEventArgs& e);
    void OnKeyUp(const KeyEventArgs& e);
    void OnLostMouseCapture(const MouseEventArgs& e);
    void OnMouseEnter(const MouseEventArgs& e);
    void OnMouseLeave(const MouseEventArgs& e);
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e);
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e);
    //@}

    /// From Visual
    //@{
    void OnDisconnectFromView();
    //@}

private:
    void StartTimer();
    void StopTimer();

    uint32_t OnTimerElapsed();

private:
    uint32_t mTimerId;

    NS_DECLARE_REFLECTION(RepeatButton, BaseButton)
};

}


#endif
