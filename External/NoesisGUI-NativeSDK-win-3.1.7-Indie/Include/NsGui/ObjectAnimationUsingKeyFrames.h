////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_OBJECTANIMATIONUSINGKEYFRAMES_H__
#define __GUI_OBJECTANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/ObjectAnimationBase.h>


namespace Noesis
{

class ObjectKeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of ObjectKeyFrame objects.
///
/// This collection is used as part of an ObjectAnimationUsingKeyFrames to animate an Object
/// property value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.objectkeyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<ObjectKeyFrame> ObjectKeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *BaseComponent* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of ObjectKeyFrame objects. Each ObjectKeyFrame defines a segment of the animation
/// with its own target Value and KeyTime. When the animation runs, it progresses from one key value
/// to the next at the specified key times.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.objectanimationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ObjectAnimationUsingKeyFrames final: public ObjectAnimationBase
{
public:
    ObjectAnimationUsingKeyFrames();
    ~ObjectAnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    ObjectKeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<ObjectAnimationUsingKeyFrames> Clone() const;
    Ptr<ObjectAnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From ObjectAnimationBase
    //@{
    Ptr<BaseComponent> GetCurrentValueCore(BaseComponent* defaultOrigin,
        BaseComponent* defaultDestination, AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<ObjectKeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<ObjectKeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(ObjectAnimationUsingKeyFrames, ObjectAnimationBase)
};

NS_WARNING_POP

}


#endif
