////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SLIDER_H__
#define __GUI_SLIDER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Vector.h>
#include <NsGui/CoreApi.h>
#include <NsGui/RangeBase.h>


namespace Noesis
{

class Track;
class RoutedCommand;
struct DragStartedEventArgs;
struct DragDeltaEventArgs;
struct CanExecuteRoutedEventArgs;
struct ExecutedRoutedEventArgs;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that lets the user select from a range of values by moving a Thumb
/// control along a Track.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.slider.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Slider: public RangeBase
{
public:
    Slider();
    ~Slider();

    /// Gets or sets the amount of time in milliseconds that a RepeatButton waits, while it is 
    /// pressed, before a command to move the Thumb executes, such as a *DecreaseLarge* command
    //@{
    int32_t GetDelay() const;
    void SetDelay(int32_t delay);
    //@}

    /// Gets or sets the amount of time in milliseconds between increase or decrease commands when
    /// a user clicks the RepeatButton of a Slider
    //@{
    int32_t GetInterval() const;
    void SetInterval(int32_t interval);
    //@}

    /// Gets or sets the direction of increasing value
    //@{
    bool GetIsDirectionReversed() const;
    void SetIsDirectionReversed(bool value);
    //@}

    /// Gets or sets a value that indicates whether the Thumb of a Slider moves immediately to the
    /// location of the mouse click that occurs while the mouse pointer pauses on the Slider track
    //@{
    bool GetIsMoveToPointEnabled() const;
    void SetIsMoveToPointEnabled(bool value);
    //@}

    /// Gets or sets a value that indicates whether the Slider displays a selection range along the
    /// Slider
    //@{
    bool GetIsSelectionRangeEnabled() const;
    void SetIsSelectionRangeEnabled(bool value);
    //@}

    /// Gets or sets a value that indicates whether the Slider automatically moves the Thumb to the
    /// closest tick mark
    //@{
    bool GetIsSnapToTickEnabled() const;
    void SetIsSnapToTickEnabled(bool value);
    //@}

    /// Gets or sets the orientation of a Slider
    //@{
    Orientation GetOrientation() const;
    void SetOrientation(Orientation orientation);
    //@}

    /// Gets or sets the largest value of a specified selection for a Slider
    //@{
    float GetSelectionEnd() const;
    void SetSelectionEnd(float value);
    //@}

    /// Gets or sets the smallest value of a specified selection for a Slider
    //@{
    float GetSelectionStart() const;
    void SetSelectionStart(float value);
    //@}

    /// Gets or sets the interval between tick marks
    //@{
    float GetTickFrequency() const;
    void SetTickFrequency(float value);
    //@}

    /// Gets or sets the position of tick marks with respect to the Track of the Slider
    //@{
    TickPlacement GetTickPlacement() const;
    void SetTickPlacement(TickPlacement placement);
    //@}

    /// Gets or sets the positions of the tick marks to display for a Slider
    //@{
    const char* GetTicks() const;
    void SetTicks(const char* value);
    //@}

    /// Gets the Track for a Slider control
    Track* GetTrack() const;

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* DelayProperty;
    static const DependencyProperty* IntervalProperty;
    static const DependencyProperty* IsDirectionReversedProperty;
    static const DependencyProperty* IsMoveToPointEnabledProperty;
    static const DependencyProperty* IsSelectionRangeEnabledProperty;
    static const DependencyProperty* IsSnapToTickEnabledProperty;
    static const DependencyProperty* OrientationProperty;
    static const DependencyProperty* SelectionEndProperty;
    static const DependencyProperty* SelectionStartProperty;
    static const DependencyProperty* TickFrequencyProperty;
    static const DependencyProperty* TickPlacementProperty;
    static const DependencyProperty* TicksProperty;
    //@}

    /// Routed Commands
    //@{
    static const RoutedCommand* DecreaseLargeCommand;
    static const RoutedCommand* DecreaseSmallCommand;
    static const RoutedCommand* IncreaseLargeCommand;
    static const RoutedCommand* IncreaseSmallCommand;
    static const RoutedCommand* MaximizeValueCommand;
    static const RoutedCommand* MinimizeValueCommand;
    //@}

protected:
    // Slider value changes
    //@{
    virtual void OnDecreaseLarge();
    virtual void OnIncreaseLarge();
    virtual void OnDecreaseSmall();
    virtual void OnIncreaseSmall();
    virtual void OnMinimizeValue();
    virtual void OnMaximizeValue();
    //@}

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From UIElement
    //@{
    void OnPreviewMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnPreviewTouchDown(const TouchEventArgs& e) override;
    void OnManipulationStarting(const ManipulationStartingEventArgs& e) override;
    //@}

    /// From Control
    //@{
    void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot) override;
    //@}

    /// From Slider
    //@{
    void OnValueChanged(float oldValue, float newValue) override;
    //@}

private:
    bool PreviewDown(const Point& p, bool isTouch, uint64_t touchDevice = 0);

    void OnThumbDragStarted(BaseComponent* sender, const DragStartedEventArgs& e);
    void OnThumbDragDelta(BaseComponent* sender, const DragDeltaEventArgs& e);

    void RegisterDelegates();
    void FreeDelegates();

    void InvalidateTicks();
    void EnsureTicks();

    void MoveToNearestTick(float newValue);
    void MoveToNextTick(float directionDelta);
    float FindNextTick(float currentValue);
    float FindPrevTick(float currentValue);

private:
    Ptr<Track> mTrackInternal;

    typedef Vector<float> Ticks;
    Ticks mTicksInternal;

    NS_DECLARE_REFLECTION(Slider, RangeBase)
};

NS_WARNING_POP

}


#endif
