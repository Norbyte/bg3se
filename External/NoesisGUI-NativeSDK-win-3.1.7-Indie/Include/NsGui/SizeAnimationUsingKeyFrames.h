////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SIZEANIMATIONUSINGKEYFRAMES_H__
#define __GUI_SIZEANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/SizeAnimationBase.h>


namespace Noesis
{

struct Size;
class SizeKeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of SizeKeyFrame objects.
///
/// This collection is used as part of a SizeAnimationUsingKeyFrames to animate a Size property
/// value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.sizekeyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<SizeKeyFrame> SizeKeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Size* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of SizeKeyFrame objects. Each SizeKeyFrame defines a segment of the animation with
/// its own target Value and KeyTime. When the animation runs, it progresses from one key value to
/// the next at the specified key times.
///
/// There are three types of SizeKeyFrame classes, one for each supported interpolation method:
/// LinearSizeKeyFrame, DiscreteSizeKeyFrame, and SplineSizeKeyFrame.
///
/// Unlike a SizeAnimation, a SizeAnimationUsingKeyFrames can have more than two of target values.
/// You can also control the interpolation method of individual SizeKeyFrame segments.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.sizeanimationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API SizeAnimationUsingKeyFrames final: public SizeAnimationBase
{
public:
    SizeAnimationUsingKeyFrames();
    ~SizeAnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    SizeKeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<SizeAnimationUsingKeyFrames> Clone() const;
    Ptr<SizeAnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From SizeAnimationBase
    //@{
    Size GetCurrentValueCore(const Size& defaultOrigin, const Size& defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<SizeKeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<SizeKeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(SizeAnimationUsingKeyFrames, SizeAnimationBase)
};

NS_WARNING_POP

}


#endif
