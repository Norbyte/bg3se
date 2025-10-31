////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CLOCKGROUP_H__
#define __GUI_CLOCKGROUP_H__


#include <NsCore/Noesis.h>
#include <NsCore/Vector.h>
#include <NsGui/Clock.h>


namespace Noesis
{

class TimelineGroup;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// An assemblage of Clock types with behavior based off of a TimelineGroup.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.clockgroup.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ClockGroup: public Clock
{
public:
    ClockGroup(TimelineGroup* timelineGroup, bool controllable);
    ~ClockGroup();

    /// Gets the children collection of this ClockGroup
    void Add(Clock* clock);

    /// Gets the TimelineGroup object that dictates the behavior of this ClockGroup instance.
    TimelineGroup* GetTimeline() const;

    /// Get number of children
    uint32_t GetChildrenCount() const;

    /// Get child at index position
    Clock* GetChild(uint32_t index) const;

    /// From Clock
    //@{
    bool Tick(double time, ClockState parentState) override;
    //@}

private:
    /// From Clock
    //@{
    void SetOwner(TimeManager* owner) override;
    void Recycle(TimeManager* owner) override;
    //@}

private:
    Vector<Ptr<Clock>> mChildren;

    NS_DECLARE_REFLECTION(ClockGroup, Clock)
};

NS_WARNING_POP

}


#endif
