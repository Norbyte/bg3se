////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_POINT3DANIMATIONBASE_H__
#define __GUI_POINT3DANIMATIONBASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/AnimationTimeline.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

struct Point3D;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract class that, when implemented, animates a *Point3D* value.
///
/// Derive from a Point3DAnimationBase class and implement the *GetCurrentValueCore* method. The
/// *GetCurrentValueCore* method returns the current value of the animation. It takes three
/// parameters: a suggested starting value, a suggested ending value, and an AnimationClock, which
/// you use to determine the progress of the animation.
///
/// Because Point3DAnimationBase class inherit from the Freezable class, you must also override
/// *CreateInstanceCore* core to return a new instance of your class.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.point3danimationbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Point3DAnimationBase: public AnimationTimeline
{
public:
    /// Gets the current value of the animation
    Point3D GetCurrentValue(const Point3D& defaultOrigin, const Point3D& defaultDestination,
        AnimationClock* clock);

protected:
    /// Calculates a value that represents the current value of the property being animated,
    /// as determined by the host animation
    virtual Point3D GetCurrentValueCore(const Point3D& defaultOrigin, const Point3D& defaultDestination,
        AnimationClock* clock) = 0;

private:
    /// From AnimationTimeline
    //@{
    const Type* GetTargetPropertyType() const final override;
    Ptr<BaseComponent> GetCurrentValue(BaseComponent* defaultOrigin,
        BaseComponent* defaultDestination, AnimationClock* clock) final override;
    //@}

    NS_DECLARE_REFLECTION(Point3DAnimationBase, AnimationTimeline)
};

}

#endif
