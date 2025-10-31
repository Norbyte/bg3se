////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CLOCK_H__
#define __GUI_CLOCK_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsCore/Delegate.h>
#include <NsCore/Vector.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/Duration.h>
#include <NsGui/Events.h>


namespace Noesis
{

class DependencyObject;
class ClockGroup;
class Timeline;
class TimeManager;

enum ClockState
{
    /// The current Clock time changes in direct relation to that of its parent. If the timeline 
    /// is an animation, it is actively affecting targeted properties, so their value may change 
    /// from tick (a sampling point in time) to tick. If the timeline has children, they may be 
    /// ClockState_Active, ClockState_Filling, or ClockState_Stopped
    ClockState_Active,
    /// The clock timing is paused. If the timeline is an animation, targeted properties are not
    /// updated and maintain its last value. If the timeline has children, they may be
    /// ClockState_Active, ClockState_Filling, or ClockState_Paused
    ClockState_Paused,
    /// The Clock timing continues, but does not change in relation to that of its parent. If the 
    /// timeline is an animation, it is actively affecting targeted properties, but its values 
    /// don't change from tick to tick. If the timeline has children, they may be ClockState_Active,
    /// ClockState_Filling, or ClockState_Stopped
    ClockState_Filling,
    /// The Clock timing is halted, making the clock's current time and progress values undefined.
    /// If this timeline is an animation, it no longer affects targeted properties. If this 
    /// timeline has children, they are also ClockState_Stopped
    ClockState_Stopped,
    /// To test if the clock finished completely and the TimeManaged can remove it. This can be done
    /// also launching a Finished event if the TimeManager registers against it
    ClockState_Finished
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Maintains run-time timing state for a Timeline.
///
/// A Timeline, by itself, doesn't actually do anything other than describe a segment of time. It's
/// the timeline's Clock object that does the real work: it maintains timing-related run-time state
/// for the timeline.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.clock.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Clock: public BaseComponent
{
public:
    Clock(Timeline* timeline, bool controllable);
    virtual ~Clock() = 0;

    /// Get the current iteration of this clock within its current active period, or -1 if this
    /// clock is stopped
    int GetCurrentIteration() const;

    /// Gets the current progress of Clock within its current iteration. This is a value between
    /// 0.0 and 1.0
    float GetCurrentProgress() const;

    /// Gets this clock's current time within its current iteration
    double GetCurrentTime() const;

    /// Gets a value indicating whether the clock is currently *Active*, *Filling* or *Stopped*
    ClockState GetCurrentState() const;
    
    /// Gets a value that indicates whether this Clock is part of a controllable clock tree
    bool HasControllableRoot() const;

    /// Gets or sets the clock that is the parent of this clock
    ClockGroup* GetParent() const;

    /// Gets the Timeline from which this Clock was created
    Timeline* GetTimeline() const;

    /// Occurs when this clock has completely finished playing
    DelegateEvent_<EventHandler> Completed();

protected:
    virtual bool Tick(double time, ClockState parentState);

    virtual void SetOwner(TimeManager* owner);
    virtual void Recycle(TimeManager* owner);
    void Construct(Timeline* timeline, bool controllable);

    /// From BaseRefCounted
    //@{
    int32_t OnDestroy() override;
    //@}

private:
    void RaiseCompleted(const EventArgs& args);

protected:
    TimeManager* mOwner;
    Ptr<Timeline> mTimeline;

private:
    friend class ClockGroup;
    friend class TimeManager;
    friend class ClockTest;

    ClockGroup* mParent;
    DependencyObject* mTarget;

    int mCurrentIteration;
    float mCurrentProgress;
    double mCurrentTime;

    ClockState mState;

    bool mHasControllableRoot;
    bool mNeedsUpdate;

    EventHandler mCompleted;

    NS_DECLARE_REFLECTION(Clock, BaseComponent)
};

NS_WARNING_POP

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_ANIMATION_API, Noesis::ClockState)


#endif
