////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_THICKNESSANIMATIONUSINGKEYFRAMES_H__
#define __GUI_THICKNESSANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/ThicknessAnimationBase.h>


namespace Noesis
{

struct Thickness;
class ThicknessKeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of ThicknessKeyFrame objects.
///
/// This collection is used as part of a ThicknessAnimationUsingKeyFrames to animate a Thickness
/// property value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.thicknesskeyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<ThicknessKeyFrame> ThicknessKeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Thickness* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of ThicknessKeyFrame objects. Each ThicknessKeyFrame defines a segment of the
/// animation with its own target Value and KeyTime. When the animation runs, it progresses from one
/// key value to the next at the specified key times.
///
/// There are three types of ThicknessKeyFrame classes, one for each supported interpolation method:
/// LinearThicknessKeyFrame, DiscreteThicknessKeyFrame, and SplineThicknessKeyFrame.
///
/// Unlike a ThicknessAnimation, a ThicknessAnimationUsingKeyFrames can have more than two of target
/// values. You can also control the interpolation method of individual ThicknessKeyFrame segments.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.thicknessanimationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ThicknessAnimationUsingKeyFrames final: public ThicknessAnimationBase
{
public:
    ThicknessAnimationUsingKeyFrames();
    ~ThicknessAnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    ThicknessKeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<ThicknessAnimationUsingKeyFrames> Clone() const;
    Ptr<ThicknessAnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From ThicknessAnimationBase
    //@{
    Thickness GetCurrentValueCore(const Thickness& defaultOrigin, const Thickness& defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<ThicknessKeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<ThicknessKeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(ThicknessAnimationUsingKeyFrames, ThicknessAnimationBase)
};

NS_WARNING_POP

}


#endif
