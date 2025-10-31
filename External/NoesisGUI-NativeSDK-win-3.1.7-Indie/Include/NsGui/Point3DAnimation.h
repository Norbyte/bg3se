////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_POINT3DANIMATION_H__
#define __GUI_POINT3DANIMATION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Point3DAnimationBase.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

template<class T> class Nullable;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Point3D* property between two target values using linear interpolation
/// over a specified *Duration*.
///
/// It is common to specify a starting value (set it by using the *From* property) and an ending
/// value (set it by using the *To* property). However, you can also specify only a starting value,
/// a destination value, or an offset value. In these cases, the animation obtains the missing
/// target value from the property that is being animated.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.point3danimation.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Point3DAnimation final: public Point3DAnimationBase
{
public:
    /// Gets or sets the total amount by which the animation changes its starting value
    //@{
    const Nullable<Point3D>& GetBy() const;
    void SetBy(const Nullable<Point3D>& by);
    //@}

    /// Gets or sets the animation's starting value
    //@{
    const Nullable<Point3D>& GetFrom() const;
    void SetFrom(const Nullable<Point3D>& from);
    //@}

    /// Gets or sets the animation's ending value
    //@{
    const Nullable<Point3D>& GetTo() const;
    void SetTo(const Nullable<Point3D>& to);
    //@}

    /// Gets or sets the easing function applied to this animation
    //@{
    EasingFunctionBase* GetEasingFunction() const;
    void SetEasingFunction(EasingFunctionBase* function);
    //@}

    /// From Freezable
    //@{
    Ptr<Point3DAnimation> Clone() const;
    Ptr<Point3DAnimation> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ByProperty;
    static const DependencyProperty* FromProperty;
    static const DependencyProperty* ToProperty;
    static const DependencyProperty* EasingFunctionProperty;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From AnimationTimeline
    //@{
    Ptr<AnimationTimeline> CreateTransitionFrom(EasingFunctionBase* easing) const override;
    Ptr<AnimationTimeline> CreateTransitionTo(EasingFunctionBase* easing) const override;
    //@}

    /// From Point3DAnimationBase
    //@{
    Point3D GetCurrentValueCore(const Point3D& defaultOrigin, const Point3D& defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    NS_DECLARE_REFLECTION(Point3DAnimation, Point3DAnimationBase)
};

}

#endif
