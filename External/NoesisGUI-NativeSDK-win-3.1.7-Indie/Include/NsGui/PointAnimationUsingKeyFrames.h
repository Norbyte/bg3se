////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_POINTANIMATIONUSINGKEYFRAMES_H__
#define __GUI_POINTANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/PointAnimationBase.h>


namespace Noesis
{

struct Point;
class PointKeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of PointKeyFrame objects.
///
/// This collection is used as part of a PointAnimationUsingKeyFrames to animate a Point property
/// value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.pointkeyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<PointKeyFrame> PointKeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Point* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of PointKeyFrame objects. Each PointKeyFrame defines a segment of the animation with
/// its own target Value and KeyTime. When the animation runs, it progresses from one key value to
/// the next at the specified key times.
///
/// There are three types of PointKeyFrame classes, one for each supported interpolation method:
/// LinearPointKeyFrame, DiscretePointKeyFrame, and SplinePointKeyFrame.
///
/// Unlike a PointAnimation, a PointAnimationUsingKeyFrames can have more than two of target values.
/// You can also control the interpolation method of individual PointKeyFrame segments.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.pointanimationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API PointAnimationUsingKeyFrames final: public PointAnimationBase
{
public:
    PointAnimationUsingKeyFrames();
    ~PointAnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    PointKeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<PointAnimationUsingKeyFrames> Clone() const;
    Ptr<PointAnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From PointAnimationBase
    //@{
    Point GetCurrentValueCore(const Point& defaultOrigin, const Point& defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<PointKeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<PointKeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(PointAnimationUsingKeyFrames, PointAnimationBase)
};

NS_WARNING_POP

}


#endif
