////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ANIMATIONTIMELINE_H__
#define __GUI_ANIMATIONTIMELINE_H__


#include <NsCore/Noesis.h>
#include <NsGui/Timeline.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/HandoffBehavior.h>


namespace Noesis
{

class Clock;
class AnimationClock;
class EasingFunctionBase;
NS_INTERFACE ITimeManager;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a segment of time over which output values are produced. These values are used to
/// animate a target property.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.animationtimeline.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API AnimationTimeline: public Timeline
{
public:
    AnimationTimeline();
    virtual ~AnimationTimeline() = 0;

    /// Calculates the current animated value depending on current clock state
    virtual Ptr<BaseComponent> GetCurrentValue(BaseComponent* defaultOrigin,
        BaseComponent* defaultDestination, AnimationClock* clock) = 0;

    /// Begins the animation timeline creating a compatible clock
    void Start(DependencyObject* target, const DependencyProperty* dp, ITimeManager* timeManager,
        HandoffBehavior handoff = HandoffBehavior_SnapshotAndReplace);

    /// Returns the type of property that can be animated by this animation
    /// \prop
    virtual const Type* GetTargetPropertyType() const = 0;

    /// Identifies the IsAdditive dependency property.
    //@{
    bool GetIsAdditive() const;
    void SetIsAdditive(bool value);
    //@}

    /// Identifies the IsCumulative dependency property. 
    //@{
    bool GetIsCumulative() const;
    void SetIsCumulative(bool value);
    //@}

    /// Returns a new timeline used as transition (used by VisualStateManager)
    //@{
    virtual Ptr<AnimationTimeline> CreateTransitionFrom(EasingFunctionBase* easing) const;
    virtual Ptr<AnimationTimeline> CreateTransitionTo(EasingFunctionBase* easing) const;
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<AnimationTimeline> Clone() const;
    Ptr<AnimationTimeline> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* IsAdditiveProperty;
    static const DependencyProperty* IsCumulativeProperty;
    //@}

protected:
    /// From Timeline
    //@{
    Ptr<Clock> CreateClockCore(TimeManager* timeManager, bool hasControllableRoot) override;
    Duration GetNaturalDuration(Clock* clock) const override;
    Duration GetEffectiveDuration() const override;
    //@}

    NS_DECLARE_REFLECTION(AnimationTimeline, Timeline)
};

}


#endif
