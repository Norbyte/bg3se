////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INT64ANIMATIONUSINGKEYFRAMES_H__
#define __GUI_INT64ANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Int64AnimationBase.h>


namespace Noesis
{

class Int64KeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of Int64KeyFrame objects.
///
/// This collection is used as part of a Int64AnimationUsingKeyFrames to animate a Int64 property
/// value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.int64keyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<Int64KeyFrame> Int64KeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Int64* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of Int64KeyFrame objects. Each Int64KeyFrame defines a segment of the animation with
/// its own target Value and KeyTime. When the animation runs, it progresses from one key value to
/// the next at the specified key times.
///
/// There are three types of Int64KeyFrame classes, one for each supported interpolation method:
/// LinearInt64KeyFrame, DiscreteInt64KeyFrame, and SplineInt64KeyFrame.
///
/// Unlike a Int64Animation, a Int64AnimationUsingKeyFrames can have more than two of target values.
/// You can also control the interpolation method of individual Int64KeyFrame segments.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.int64animationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Int64AnimationUsingKeyFrames final: public Int64AnimationBase
{
public:
    Int64AnimationUsingKeyFrames();
    ~Int64AnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    Int64KeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<Int64AnimationUsingKeyFrames> Clone() const;
    Ptr<Int64AnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From Int64AnimationBase
    //@{
    int64_t GetCurrentValueCore(int64_t defaultOrigin, int64_t defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<Int64KeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<Int64KeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(Int64AnimationUsingKeyFrames, Int64AnimationBase)
};

NS_WARNING_POP

}


#endif
