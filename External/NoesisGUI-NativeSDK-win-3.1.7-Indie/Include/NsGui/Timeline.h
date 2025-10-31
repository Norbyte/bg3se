////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TIMELINE_H__
#define __GUI_TIMELINE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsGui/Animatable.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/Duration.h>
#include <NsGui/Events.h>


namespace Noesis
{

class TimeSpan;
class RepeatBehavior;
class Clock;
class FrameworkElement;
class TimeManager;
template<class T> class Nullable;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies how a Timeline behaves when it is outside its active period but its parent is 
/// inside its active or hold period
////////////////////////////////////////////////////////////////////////////////////////////////////
enum FillBehavior
{
    /// After it reaches the end of its active period, the timeline holds its progress until the 
    /// end of its parent's active and hold periods
    FillBehavior_HoldEnd,
    /// The timeline stops if it is outside its active period while its parent is inside its 
    /// active period
    FillBehavior_Stop
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Arguments to be passed in timeline Complete event
////////////////////////////////////////////////////////////////////////////////////////////////////
struct TimelineEventArgs: public EventArgs
{
    DependencyObject* target;

    TimelineEventArgs(DependencyObject* t = 0): target(t) { }
};

typedef Delegate<void (BaseComponent*, const TimelineEventArgs&)> TimelineEventHandler;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a segment of time.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.timeline.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Timeline: public Animatable
{
public:
    Timeline();
    Timeline(const Nullable<TimeSpan>& beginTime);
    Timeline(const Nullable<TimeSpan>& beginTime, const Duration& duration);
    Timeline(const Nullable<TimeSpan>& beginTime, const Duration& duration,
        const RepeatBehavior& repeatBehavior);
    virtual ~Timeline() = 0;

    /// Gets the desired frame rate for this timeline and its child timelines
    static int GetDesiredFrameRate(const DependencyObject* timeline);

    /// Sets the desired frame rate for this timeline and its child timelines
    static void SetDesiredFrameRate(DependencyObject* timeline, int rate);

    /// Gets or sets a value specifying the percentage of the timeline's Duration spent 
    /// accelerating the passage of time from zero to its maximum rate
    //@{
    float GetAccelerationRatio() const;
    void SetAccelerationRatio(float value);
    //@}

    /// Gets or sets a value that indicates whether the timeline plays in reverse after it 
    /// completes a forward iteration
    //@{
    bool GetAutoReverse() const;
    void SetAutoReverse(bool value);
    //@}
    
    /// Gets or sets the time at which this Timeline should begin.
    /// A timeline's own *SpeedRatio* setting does not affect its BeginTime. For example, a
    /// timeline with a *BeginTime* of 5 seconds, a *SpeedRatio* of 2, and a parent timeline
    /// with a *SpeedRatio* of 1 starts after 5 seconds, not 2.5.
    //@{
    const Nullable<TimeSpan>& GetBeginTime() const;
    void SetBeginTime(const Nullable<TimeSpan>& time);
    //@}

    /// Gets or sets a value specifying the percentage of the timeline's Duration spent 
    /// decelerating the passage of time from its maximum rate to zero
    //@{
    float GetDecelerationRatio() const;
    void SetDecelerationRatio(float value);
    //@}

    /// Gets or sets the length of time for which this timeline plays, not counting repetitions
    //@{
    const Duration& GetDuration() const;
    void SetDuration(const Duration& duration);
    //@}
    
    /// Gets or sets a value that specifies how the animation behaves after it reaches the end of 
    /// its active period
    //@{
    FillBehavior GetFillBehavior() const;
    void SetFillBehavior(FillBehavior behavior);
    //@}
    
    /// Gets or sets the name of this Timeline
    //@{
    const char* GetName() const;
    void SetName(const char* name);
    //@}
    
    /// Gets or sets the repeating behavior of this timeline
    //@{
    const RepeatBehavior& GetRepeatBehavior() const;
    void SetRepeatBehavior(const RepeatBehavior& behavior);
    //@}
    
    /// Gets or sets the rate, relative to its parent, at which time progresses for this Timeline
    //@{
    float GetSpeedRatio() const;
    void SetSpeedRatio(float value);
    //@}

    /// Occurs when the Storyboard object has completed playing
    DelegateEvent_<TimelineEventHandler> Completed();

    /// Creates a new Clock from this Timeline and specifies whether the new Clock is controllable.
    /// If this Timeline has children, a tree of clocks is created with this Timeline as the root.
    Ptr<Clock> CreateClock(TimeManager* timeManager, bool hasControllableRoot);

    /// Gets the duration of the timeline when duration is set to automatic
    virtual Duration GetNaturalDuration(Clock* clock) const;

    /// Gets duration of a single pass (without the reverse)
    const Duration& GetSinglePassDuration() const;

    /// Gets duration of a complete pass (with the reverse, if applicable)
    const Duration& GetIterationDuration() const;

    /// Gets total duration of the animation (with repetitions and autoreverse, but without
    /// BeginTime nor SpeedRatio)
    const Duration& GetTotalDuration() const;

    /// Indicates that the total duration is not affected by SpeedRatio
    bool IsTotalDurationAbsolute() const;

    const Duration& GetNormalizedDuration() const;

    /// Computes the effective durations of the timeline
    void CalculateEffectiveDurations();

    /// Calculated as TotalDuration / SpeedRatio + BeginTime
    virtual Duration GetEffectiveDuration() const = 0;

    // Called when an owner clock is being destroyed
    virtual void OnClockDestroyed(const Clock* clock);

    // Hides Freezable methods for convenience
    //@{
    Ptr<Timeline> Clone() const;
    Ptr<Timeline> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* AccelerationRatioProperty;
    static const DependencyProperty* AutoReverseProperty;
    static const DependencyProperty* BeginTimeProperty;
    static const DependencyProperty* DecelerationRatioProperty;
    static const DependencyProperty* DesiredFrameRateProperty;
    static const DependencyProperty* DurationProperty;
    static const DependencyProperty* FillBehaviorProperty;
    static const DependencyProperty* NameProperty;
    static const DependencyProperty* RepeatBehaviorProperty;
    static const DependencyProperty* SpeedRatioProperty;
    //@}

protected:
    /// Creates a Clock for this Timeline
    virtual Ptr<Clock> CreateClockCore(TimeManager* timeManager, bool hasControllableRoot) = 0;

    // From Freezable
    //@{
    void CloneCommonCore(const Freezable* source) override;
    //@}

private:
    void RaiseCompleted(const TimelineEventArgs& args);

private:
    friend class Storyboard;
    friend class TimeManager;

    Duration mSinglePassDurationInternal;
    Duration mIterationDurationInternal;
    Duration mTotalDurationInternal;
    bool mTotalDurationAbsolute;
    Duration mNormalizedDurationInternal;

    TimelineEventHandler mCompleted;

    NS_DECLARE_REFLECTION(Timeline, Animatable)
};

NS_WARNING_POP

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_ANIMATION_API, Noesis::FillBehavior)


#endif
