////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_POINT3DANIMATIONUSINGKEYFRAMES_H__
#define __GUI_POINT3DANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Point3DAnimationBase.h>


namespace Noesis
{

struct Point3D;
class Point3DKeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of Point3DKeyFrame objects.
///
/// This collection is used as part of a Point3DAnimationUsingKeyFrames to animate a Point3D
/// property value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.point3dkeyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<Point3DKeyFrame> Point3DKeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Point3D* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of Point3DKeyFrame objects. Each Point3DKeyFrame defines a segment of the animation
/// with its own target Value and KeyTime. When the animation runs, it progresses from one key value
/// to the next at the specified key times.
///
/// There are three types of Point3DKeyFrame classes, one for each supported interpolation method:
/// LinearPoint3DKeyFrame, DiscretePoint3DKeyFrame, and SplinePoint3DKeyFrame.
///
/// Unlike a Point3DAnimation, a Point3DAnimationUsingKeyFrames can have more than two of target
/// values. You can also control the interpolation method of individual Point3DKeyFrame segments.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.point3danimationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Point3DAnimationUsingKeyFrames final: public Point3DAnimationBase
{
public:
    Point3DAnimationUsingKeyFrames();
    ~Point3DAnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    Point3DKeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<Point3DAnimationUsingKeyFrames> Clone() const;
    Ptr<Point3DAnimationUsingKeyFrames> CloneCurrentValue() const;
    //@}

private:
    /// From DependencyObject
    //@{
    void OnInit() override;
    //@}

    /// From Freezable
    //@{
    void CloneCommonCore(const Freezable* source) override;
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From Timeline
    //@{
    Duration GetNaturalDuration(Clock* clock) const override;
    Duration GetEffectiveDuration() const override;
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
    mutable Ptr<Point3DKeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<Point3DKeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(Point3DAnimationUsingKeyFrames, Point3DAnimationBase)
};

NS_WARNING_POP

}


#endif
