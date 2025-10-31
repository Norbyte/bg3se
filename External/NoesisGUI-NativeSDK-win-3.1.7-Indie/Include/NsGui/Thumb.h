////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_THUMB_H__
#define __GUI_THUMB_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Control.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides information about the *DragCompleted* event that occurs when a user completes a drag
/// operation with the mouse of a Thumb control.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct DragCompletedEventArgs: public RoutedEventArgs
{
    /// Gets whether the drag operation for a Thumb was canceled by a call to the CancelDrag method
    bool canceled;

    /// Gets horizontal change in position of the Thumb after the user drags the control with mouse
    float horizontalChange;

    /// Gets vertical change in position of the Thumb after the user drags the control with mouse
    float verticalChange;

    DragCompletedEventArgs(BaseComponent* source, bool canceled, float hChange, float vChange);
};

typedef Delegate<void (BaseComponent*, const DragCompletedEventArgs&)> DragCompletedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides information about the *DragStarted* event that occurs when a user drags a Thumb control
/// with the mouse.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct DragStartedEventArgs: public RoutedEventArgs
{
    /// Gets the horizontal offset of the mouse click relative to the screen coordinates of Thumb
    float horizontalOffset;

    /// Gets the vertical offset of the mouse click relative to the screen coordinates of the Thumb
    float verticalOffset;

    DragStartedEventArgs(BaseComponent* source, float hOffset, float voffset);
};

typedef Delegate<void (BaseComponent*, const DragStartedEventArgs&)> DragStartedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides information about the *DragDelta* event that occurs one or more times when a user drags
/// a Thumb control with the mouse.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct DragDeltaEventArgs: public RoutedEventArgs
{
    /// Gets horizontal change in position of the Thumb after the user drags the control with mouse
    float horizontalChange;

    /// Gets vertical change in position of the Thumb after the user drags the control with mouse
    float verticalChange;

    DragDeltaEventArgs(BaseComponent* source, float hChange, float vChange);
};

typedef Delegate<void (BaseComponent*, const DragDeltaEventArgs&)> DragDeltaEventHandler;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that can be dragged by the user.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.thumb.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Thumb: public Control
{
public:
    Thumb();

    /// Gets whether the Thumb control has logical focus and mouse capture and the left mouse
    /// button is pressed
    bool GetIsDragging() const;

    /// Cancels current dragging operation
    void CancelDrag();

    /// Occurs when the Thumb control loses mouse capture
    UIElement::RoutedEvent_<DragCompletedEventHandler> DragCompleted();

    /// Occurs one or more times as the mouse changes position when a Thumb control has logical
    /// focus and mouse capture
    UIElement::RoutedEvent_<DragDeltaEventHandler> DragDelta();

    /// Occurs when a Thumb control receives logical focus and mouse capture
    UIElement::RoutedEvent_<DragStartedEventHandler> DragStarted();

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* IsDraggingProperty;
    //@}

    /// Dependency events
    //@{
    static const RoutedEvent* DragCompletedEvent;
    static const RoutedEvent* DragDeltaEvent;
    static const RoutedEvent* DragStartedEvent;
    //@}
    
protected:
    // Responds to a change in the value of the IsDragging property
    virtual void OnDraggingChanged(const DependencyPropertyChangedEventArgs& e);

    // From Control
    //@{
    void UpdateVisualStates();
    //@}

    // From UIElement
    //@{
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e);
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e);
    void OnMouseMove(const MouseEventArgs& e);
    void OnTouchDown(const TouchEventArgs& e);
    void OnTouchUp(const TouchEventArgs& e);
    void OnTouchMove(const TouchEventArgs& e);
    void OnManipulationStarting(const ManipulationStartingEventArgs& e);
    //@}

    // From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

private:
    bool StartDrag(const Point& p);
    bool StopDrag(const Point& p);
    bool Drag(const Point& p);

private:
    friend class Slider;
    Point mStartThumbPosition;
    Point mStartScreenPosition;
    Point mCurrentScreenPosition;

    NS_DECLARE_REFLECTION(Thumb, Control)
};

NS_WARNING_POP

}

#include <NsGui/Thumb.inl>

#endif
