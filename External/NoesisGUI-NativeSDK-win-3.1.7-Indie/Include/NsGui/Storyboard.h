////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_STORYBOARD_H__
#define __GUI_STORYBOARD_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/ParallelTimeline.h>
#include <NsGui/TimeSpan.h>
#include <NsGui/Clock.h>
#include <NsGui/HandoffBehavior.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsCore/Vector.h>
#include <NsCore/HashMap.h>


namespace Noesis
{

class PropertyPath;
class FrameworkElement;
class AnimationClock;
class AnimationTimeline;
struct AnimationTarget;
struct PathElement;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Indicates the origin of a seek operation. The offset is relative to this origin
////////////////////////////////////////////////////////////////////////////////////////////////////
enum TimeSeekOrigin : int8_t
{
    /// The offset is relative to the beginning of the activation period of the Timeline
    TimeSeekOrigin_BeginTime,
    /// The offset is relative to the Timeline's Duration, the length of a single iteration. This
    /// value has no meaning if the Timeline's Duration is not resolved
    TimeSeekOrigin_Duration
};

////////////////////////////////////////////////////////////////////////////////////////////////////
struct AnimationTarget
{
    inline bool operator==(const AnimationTarget& other) const
    {
        return object == other.object && dp == other.dp;
    }
    
    DependencyObject* object;
    const DependencyProperty* dp;
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A container timeline that provides object and property targeting information for its 
/// child animations. 
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.storyboard.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Storyboard: public ParallelTimeline
{
public:
    Storyboard();
    ~Storyboard();

    /// Retrieves the TargetName value of the specified Timeline.
    static const char* GetTargetName(const DependencyObject* element);

    /// Makes the specified Timeline target the dependency object with the specified name.
    static void SetTargetName(DependencyObject* element, const char* name);
    
    /// Retrieves the TargetProperty value of the specified Timeline.
    static PropertyPath* GetTargetProperty(const DependencyObject* element);
    
    /// Makes the specified Timeline target the specified dependency property.
    static void SetTargetProperty(DependencyObject* element, PropertyPath* path);

    /// Retrieves the Target value of the specified Timeline.
    static DependencyObject* GetTarget(const DependencyObject* element);
    
    /// Makes the specified Timeline target the dependency object.
    static void SetTarget(DependencyObject* element, DependencyObject* target);

    /// Applies the animations associated with this Storyboard to their targets and initiates them
    //@{
    void Begin();
    void Begin(FrameworkElement* target);
    void Begin(FrameworkElement* target, bool isControllable);
    void Begin(FrameworkElement* target, HandoffBehavior handoffBehavior);
    void Begin(FrameworkElement* target, HandoffBehavior handoffBehavior, bool isControllable);
    void Begin(FrameworkElement* target, FrameworkElement* nameScope);
    void Begin(FrameworkElement* target, FrameworkElement* nameScope, bool isControllable);
    void Begin(FrameworkElement* target, FrameworkElement* nameScope,
        HandoffBehavior handoffBehavior);
    void Begin(FrameworkElement* target, FrameworkElement* nameScope,
        HandoffBehavior handoffBehavior, bool isControllable);
    //@}

    /// Seeks this Storyboard to the specified position on the next clock tick. The offset param
    /// is a positive or negative value that describes the amount by which the timeline should
    /// move forward or backward
    //@{
    void Seek(const TimeSpan& offset);
    void Seek(const TimeSpan& offset, TimeSeekOrigin origin);
    void Seek(FrameworkElement* target, const TimeSpan& offset, TimeSeekOrigin origin);
    //@}

    /// Pauses the Clock of the specified FrameworkElement associated with this Storyboard
    //@{
    void Pause();
    void Pause(FrameworkElement* target);
    //@}

    /// Resumes the Clock that was created for this Storyboard
    //@{
    void Resume();
    void Resume(FrameworkElement* target);
    //@}

    /// Stops the Clock that was created for this Storyboard
    //@}
    void Stop();
    void Stop(FrameworkElement* target);
    //@}

    /// Removes the Clock objects that were created for this Storyboard. Animations that belong to
    /// this Storyboard no longer affect the properties they once animated, regardless of their
    /// FillBehavior setting
    //@{
    void Remove();
    void Remove(FrameworkElement* target);
    //@}

    /// Indicates if a controllable storyboard is playing
    //@{
    bool IsPlaying() const;
    bool IsPlaying(FrameworkElement* target) const;
    //@}

    /// Indicates if a controllable storyboard is paused
    //@{
    bool IsPaused() const;
    bool IsPaused(FrameworkElement* target) const;
    //@}

    /// From Freezable
    //@{
    Ptr<Storyboard> Clone() const;
    Ptr<Storyboard> CloneCurrentValue() const;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* TargetNameProperty; // Attached
    static const DependencyProperty* TargetProperty; // Attached
    static const DependencyProperty* TargetPropertyProperty; // Attached
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From Timeline
    //@{
    void OnClockDestroyed(const Clock* clock) override;
    //@}

private:
    void InternalBegin(FrameworkElement* target, FrameworkElement* nameScope,
        HandoffBehavior handoffBehavior, bool isControllable);
    void InternalSeek(FrameworkElement* target, const TimeSpan& offset, TimeSeekOrigin origin);
    void InternalPause(FrameworkElement* target);
    void InternalResume(FrameworkElement* target);
    void InternalRemove(FrameworkElement* target);

    bool InternalIsPlaying(FrameworkElement* target) const;
    bool InternalIsPaused(FrameworkElement* target) const;

    friend class TimeManager;
    void RegisterControllableClock(FrameworkElement* target, Clock* clock);
    void UnregisterControllableClock(FrameworkElement* target, bool removeClock, bool clearValue);

    typedef HashMap<FrameworkElement*, Clock*> Controllables;

    void UnregisterControllableClock(Controllables::Iterator it, bool removeClock, bool clearValue);

    void OnTargetDestroyed(DependencyObject* object);

    FrameworkElement* FindTarget() const;

    struct Animation
    {
        DependencyObject* target;
        const DependencyProperty* dp;
        AnimationClock* clock;
    };

    typedef Vector<Animation, 64> Animations;

    void ResolveTargets(FrameworkElement* fe, FrameworkElement* ns, Clock* clock,
        Animations& targets) const;

    AnimationTarget ResolveTarget(Timeline* timeline, FrameworkElement* fe,
        FrameworkElement* ns = 0) const;

    void AddPathElement(const PathElement& pathElement, void* context) const;

private:
    // NOTE: There is no CloneCommonCore because mControllables are not transferred to the
    //       cloned Storyboard

    Controllables mControllables;

    NS_DECLARE_REFLECTION(Storyboard, ParallelTimeline)
};

NS_WARNING_POP

}

#endif
