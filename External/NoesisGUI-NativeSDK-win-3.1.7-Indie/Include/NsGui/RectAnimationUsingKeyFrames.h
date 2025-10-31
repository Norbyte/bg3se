////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RECTANIMATIONUSINGKEYFRAMES_H__
#define __GUI_RECTANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/RectAnimationBase.h>


namespace Noesis
{

struct Rect;
class RectKeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of RectKeyFrame objects.
///
/// This collection is used as part of a RectAnimationUsingKeyFrames to animate a Rect property
/// value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.rectkeyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<RectKeyFrame> RectKeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Rect* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of RectKeyFrame objects. Each RectKeyFrame defines a segment of the animation with
/// its own target Value and KeyTime. When the animation runs, it progresses from one key value to
/// the next at the specified key times.
///
/// There are three types of RectKeyFrame classes, one for each supported interpolation method:
/// LinearRectKeyFrame, DiscreteRectKeyFrame, and SplineRectKeyFrame.
///
/// Unlike a RectAnimation, a RectAnimationUsingKeyFrames can have more than two target values. You
/// can also control the interpolation method of individual RectKeyFrame segments.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.rectanimationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API RectAnimationUsingKeyFrames final: public RectAnimationBase
{
public:
    RectAnimationUsingKeyFrames();
    ~RectAnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    RectKeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<RectAnimationUsingKeyFrames> Clone() const;
    Ptr<RectAnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From RectAnimationBase
    //@{
    Rect GetCurrentValueCore(const Rect& defaultOrigin, const Rect& defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<RectKeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<RectKeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(RectAnimationUsingKeyFrames, RectAnimationBase)
};

NS_WARNING_POP

}


#endif
