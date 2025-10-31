////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BOOLEANANIMATIONUSINGKEYFRAMES_H__
#define __GUI_BOOLEANANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/BooleanAnimationBase.h>


namespace Noesis
{

class BooleanKeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of BooleanKeyFrame objects.
///
/// This collection is used as part of a BooleanAnimationUsingKeyFrames to animate a Boolean
/// property value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.booleankeyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<BooleanKeyFrame> BooleanKeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Boolean* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of BooleanKeyFrame objects. Each BooleanKeyFrame defines a segment of the animation
/// with its own target Value and KeyTime. When the animation runs, it progresses from one key value
/// to the next at the specified key times.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.booleananimationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API BooleanAnimationUsingKeyFrames final: public BooleanAnimationBase
{
public:
    BooleanAnimationUsingKeyFrames();
    ~BooleanAnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    BooleanKeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<BooleanAnimationUsingKeyFrames> Clone() const;
    Ptr<BooleanAnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From BooleanAnimationBase
    //@{
    bool GetCurrentValueCore(bool defaultOrigin, bool defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<BooleanKeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<BooleanKeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(BooleanAnimationUsingKeyFrames, BooleanAnimationBase)
};

NS_WARNING_POP

}


#endif
