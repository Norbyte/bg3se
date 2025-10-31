////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INT16ANIMATIONUSINGKEYFRAMES_H__
#define __GUI_INT16ANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Int16AnimationBase.h>


namespace Noesis
{

class Int16KeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of Int16KeyFrame objects.
///
/// This collection is used as part of a Int16AnimationUsingKeyFrames to animate a Int16 property
/// value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.int16keyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<Int16KeyFrame> Int16KeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Int16* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of Int16KeyFrame objects. Each Int16KeyFrame defines a segment of the animation with
/// its own target Value and KeyTime. When the animation runs, it progresses from one key value to
/// the next at the specified key times.
///
/// There are three types of Int16KeyFrame classes, one for each supported interpolation method:
/// LinearInt16KeyFrame, DiscreteInt16KeyFrame, and SplineInt16KeyFrame.
///
/// Unlike a Int16Animation, a Int16AnimationUsingKeyFrames can have more than two of target values.
/// You can also control the interpolation method of individual Int16KeyFrame segments.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.int16animationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Int16AnimationUsingKeyFrames final: public Int16AnimationBase
{
public:
    Int16AnimationUsingKeyFrames();
    ~Int16AnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    Int16KeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<Int16AnimationUsingKeyFrames> Clone() const;
    Ptr<Int16AnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From Int16AnimationBase
    //@{
    int16_t GetCurrentValueCore(int16_t defaultOrigin, int16_t defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<Int16KeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<Int16KeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(Int16AnimationUsingKeyFrames, Int16AnimationBase)
};

NS_WARNING_POP

}


#endif
