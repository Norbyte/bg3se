////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ANIMATIONCLOCK_H__
#define __GUI_ANIMATIONCLOCK_H__


#include <NsCore/Noesis.h>
#include <NsGui/Clock.h>
#include <NsGui/HandoffBehavior.h>


namespace Noesis
{

class AnimationTimeline;
class DependencyObject;
class DependencyProperty;
NS_INTERFACE ITimeManager;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Maintains the run-time state of an AnimationTimeline and processes its output values.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.animationclock.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API AnimationClock: public Clock
{
public:
    AnimationClock(AnimationTimeline* animation, bool controllable);

    /// Starts the animation for the specified object and property
    void Start(DependencyObject* object, const DependencyProperty* dp, ITimeManager* timeManager, 
        HandoffBehavior handoff = HandoffBehavior_SnapshotAndReplace);

private:
    // From Clock
    //@{
    void Recycle(TimeManager* owner) override;
    //@}

    NS_DECLARE_REFLECTION(AnimationClock, Clock)
};

}


#endif
