////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_STRINGANIMATIONUSINGKEYFRAMES_H__
#define __GUI_STRINGANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/StringFwd.h>
#include <NsGui/StringAnimationBase.h>


namespace Noesis
{

class StringKeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of StringKeyFrame objects.
///
/// This collection is used as part of a StringAnimationUsingKeyFrames to animate a String property
/// value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.stringkeyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<StringKeyFrame> StringKeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *String* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of StringKeyFrame objects. Each StringKeyFrame defines a segment of the animation
/// with its own target Value and KeyTime. When the animation runs, it progresses from one key value
/// to the next at the specified key times.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.stringanimationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API StringAnimationUsingKeyFrames final: public StringAnimationBase
{
public:
    StringAnimationUsingKeyFrames();
    ~StringAnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    StringKeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<StringAnimationUsingKeyFrames> Clone() const;
    Ptr<StringAnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From StringAnimationBase
    //@{
    String GetCurrentValueCore(const String& defaultOrigin, const String& defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<StringKeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<StringKeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(StringAnimationUsingKeyFrames, StringAnimationBase)
};

NS_WARNING_POP

}


#endif
