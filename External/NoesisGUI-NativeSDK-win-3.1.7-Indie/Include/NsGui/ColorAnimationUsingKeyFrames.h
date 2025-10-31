////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COLORANIMATIONUSINGKEYFRAMES_H__
#define __GUI_COLORANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/ColorAnimationBase.h>


namespace Noesis
{

struct Color;
class ColorKeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of ColorKeyFrame objects.
///
/// This collection is used as part of a ColorAnimationUsingKeyFrames to animate a Color property
/// value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.colorkeyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<ColorKeyFrame> ColorKeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Color* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of ColorKeyFrame objects. Each ColorKeyFrame defines a segment of the animation with
/// its own target Value and KeyTime. When the animation runs, it progresses from one key value to
/// the next at the specified key times.
///
/// There are four types of ColorKeyFrame classes, one for each supported interpolation method:
/// LinearColorKeyFrame, DiscreteColorKeyFrame, EasingColorKeyFrame, and SplineColorKeyFrame.
///
/// Unlike a ColorAnimation, a ColorAnimationUsingKeyFrames can have more than two of target values.
/// You can also control the interpolation method of individual ColorKeyFrame segments.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.coloranimationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ColorAnimationUsingKeyFrames final: public ColorAnimationBase
{
public:
    ColorAnimationUsingKeyFrames();
    ~ColorAnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    ColorKeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<ColorAnimationUsingKeyFrames> Clone() const;
    Ptr<ColorAnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From ColorAnimationBase
    //@{
    Color GetCurrentValueCore(const Color& defaultOrigin, const Color& defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<ColorKeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<ColorKeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(ColorAnimationUsingKeyFrames, ColorAnimationBase)
};

NS_WARNING_POP

}


#endif
