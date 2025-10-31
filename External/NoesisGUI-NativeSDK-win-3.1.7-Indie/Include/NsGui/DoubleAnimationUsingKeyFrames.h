////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DOUBLEANIMATIONUSINGKEYFRAMES_H__
#define __GUI_DOUBLEANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/DoubleAnimationBase.h>


namespace Noesis
{

class DoubleKeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of DoubleKeyFrame objects.
///
/// This collection is used as part of a DoubleAnimationUsingKeyFrames to animate a Double property
/// value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.doublekeyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<DoubleKeyFrame> DoubleKeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Double* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a 
/// collection of DoubleKeyFrame objects. Each DoubleKeyFrame defines a segment of the animation
/// with its own target Value and KeyTime. When the animation runs, it progresses from one key value
/// to the next at the specified key times.
///
/// There are three types of DoubleKeyFrame classes, one for each supported interpolation method:
/// LinearDoubleKeyFrame, DiscreteDoubleKeyFrame, and SplineDoubleKeyFrame.
///
/// Unlike a DoubleAnimation, a DoubleAnimationUsingKeyFrames can have more than two of target
/// values. You can also control the interpolation method of individual DoubleKeyFrame segments.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.doubleanimationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API DoubleAnimationUsingKeyFrames final: public DoubleAnimationBase
{
public:
    DoubleAnimationUsingKeyFrames();
    ~DoubleAnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    DoubleKeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<DoubleAnimationUsingKeyFrames> Clone() const;
    Ptr<DoubleAnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From DoubleAnimationBase
    //@{
    float GetCurrentValueCore(float defaultOrigin, float defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    mutable Duration mNaturalDuration;
    mutable Ptr<DoubleKeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<DoubleKeyFrame>, double>> mOrderedFrames;

    NS_DECLARE_REFLECTION(DoubleAnimationUsingKeyFrames, DoubleAnimationBase)
};

NS_WARNING_POP

}


#endif
