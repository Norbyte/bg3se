////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TIMELINEGROUP_H__
#define __GUI_TIMELINEGROUP_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/Timeline.h>


namespace Noesis
{

template<class T> class FreezableCollection;
typedef Noesis::FreezableCollection<Noesis::Timeline> TimelineCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract class that, when implemented represents a Timeline that may contain a collection of
/// child Timeline objects.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.timelinegroup.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API TimelineGroup: public Timeline
{
public:
    TimelineGroup();
    virtual ~TimelineGroup() = 0;

    /// Gets or sets the collection of child Timeline objects
    //@{
    TimelineCollection* GetChildren() const;
    void SetChildren(TimelineCollection* timelines);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<TimelineGroup> Clone() const;
    Ptr<TimelineGroup> CloneCurrentValue() const;
    //@}

public:
    /// DependencyProperties
    //@{
    static const DependencyProperty* ChildrenProperty;
    //@}

protected:
    /// From Timeline
    //@{
    Ptr<Clock> CreateClockCore(TimeManager* timeManager, bool hasControllableRoot) override;
    //@}

    NS_DECLARE_REFLECTION(TimelineGroup, Timeline)
};

}


#endif
