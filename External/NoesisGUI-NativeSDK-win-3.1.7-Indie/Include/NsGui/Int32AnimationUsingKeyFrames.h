////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INT32ANIMATIONUSINGKEYFRAMES_H__
#define __GUI_INT32ANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Int32AnimationBase.h>


namespace Noesis
{

class Int32KeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of Int32KeyFrame objects.
///
/// This collection is used as part of a Int32AnimationUsingKeyFrames to animate a Int32 property
/// value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.int32keyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<Int32KeyFrame> Int32KeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Int32* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of Int32KeyFrame objects. Each Int32KeyFrame defines a segment of the animation with
/// its own target Value and KeyTime. When the animation runs, it progresses from one key value to
/// the next at the specified key times.
///
/// There are three types of Int32KeyFrame classes, one for each supported interpolation method:
/// LinearInt32KeyFrame, DiscreteInt32KeyFrame, and SplineInt32KeyFrame.
///
/// Unlike a Int32Animation, a Int32AnimationUsingKeyFrames can have more than two of target values.
/// You can also control the interpolation method of individual Int32KeyFrame segments.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.int32animationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Int32AnimationUsingKeyFrames final: public Int32AnimationBase
{
public:
    Int32AnimationUsingKeyFrames();
    ~Int32AnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    Int32KeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<Int32AnimationUsingKeyFrames> Clone() const;
    Ptr<Int32AnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From AnimationTimeline
    //@{
    Ptr<AnimationTimeline> CreateTransitionFrom(EasingFunctionBase* easing) const override;
    Ptr<AnimationTimeline> CreateTransitionTo(EasingFunctionBase* easing) const override;
    //@}

    /// From Timeline
    //@{
    Duration GetNaturalDuration(Clock* clock) const override;
    Duration GetEffectiveDuration() const override;
    //@}

    /// From Int32AnimationBase
    //@{
    int32_t GetCurrentValueCore(int32_t defaultOrigin, int32_t defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<Int32KeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<Int32KeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(Int32AnimationUsingKeyFrames, Int32AnimationBase)
};

NS_WARNING_POP

}


#endif
