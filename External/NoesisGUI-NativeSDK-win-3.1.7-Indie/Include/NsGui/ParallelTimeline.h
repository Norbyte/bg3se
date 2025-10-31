////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_PARALLELTIMELINE_H__
#define __GUI_PARALLELTIMELINE_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/TimelineGroup.h>
#include <NsCore/ReflectionDeclareEnum.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
enum SlipBehavior
{
    /// Indicates that a ParallelTimeline will not slip with the child Timeline, but will expand 
    /// to fit all slipping Timeline children. NOTE: This is only effective when the Duration of 
    /// the ParallelTimeline is not explicitly specified. 
    SlipBehavior_Grow,
    /// Indicates that a ParallelTimeline will slip along with its first child Timeline that can 
    /// slip whenever that child is delayed or accelerated.
    SlipBehavior_Slip
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a segment of time that may contain child Timeline objects.
///
/// These child timelines become active according to their respective BeginTime 
/// properties. Also, child timelines are able to overlap (run in parallel) with each other.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.paralleltimeline.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ParallelTimeline: public TimelineGroup
{
public:
    ParallelTimeline();
    ~ParallelTimeline();

    /// Gets or sets SlipBehavior property
    //@{
    SlipBehavior GetSlipBehavior() const;
    void SetSlipBehavior(SlipBehavior behavior); 
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<ParallelTimeline> Clone() const;
    Ptr<ParallelTimeline> CloneCurrentValue() const;
    //@}

    /// From Timeline
    //@{
    Duration GetNaturalDuration(Clock* clock) const override;
    Duration GetEffectiveDuration() const override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* SlipBehaviorProperty;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    NS_DECLARE_REFLECTION(ParallelTimeline, TimelineGroup)
};

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_ANIMATION_API, Noesis::SlipBehavior)


#endif
