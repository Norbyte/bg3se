////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MATRIXANIMATIONUSINGKEYFRAMES_H__
#define __GUI_MATRIXANIMATIONUSINGKEYFRAMES_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/MatrixAnimationBase.h>


namespace Noesis
{

class Transform2;
class MatrixKeyFrame;
template<class T> class FreezableCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of MatrixKeyFrame objects.
///
/// This collection is used as part of a MatrixAnimationUsingKeyFrames to animate a Matrix property
/// value along a set of key frames.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.animation.matrixkeyframecollection
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef FreezableCollection<MatrixKeyFrame> MatrixKeyFrameCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Matrix* property along a set of *KeyFrames*.
///
/// A key frame animation's target values are defined by its KeyFrames property, which contains a
/// collection of MatrixKeyFrame objects. Each MatrixKeyFrame defines a segment of the animation
/// with its own target Value and KeyTime. When the animation runs, it progresses from one key value
/// to the next at the specified key times.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.matrixanimationusingkeyframes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API MatrixAnimationUsingKeyFrames final: public MatrixAnimationBase
{
public:
    MatrixAnimationUsingKeyFrames();
    ~MatrixAnimationUsingKeyFrames();

    /// Gets the collection of KeyFrame objects that define the animation
    MatrixKeyFrameCollection* GetKeyFrames() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<MatrixAnimationUsingKeyFrames> Clone() const;
    Ptr<MatrixAnimationUsingKeyFrames> CloneCurrentValue() const;
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

    /// From MatrixAnimationBase
    //@{
    Transform2 GetCurrentValueCore(const Transform2& defaultOrigin,
        const Transform2& defaultDestination, AnimationClock* clock) override;
    //@}

private:
    mutable Ptr<MatrixKeyFrameCollection> mKeyFrames;
    Vector<Pair<Ptr<MatrixKeyFrame>, double>> mOrderedFrames;
    mutable Duration mNaturalDuration;

    NS_DECLARE_REFLECTION(MatrixAnimationUsingKeyFrames, MatrixAnimationBase)
};

NS_WARNING_POP

}


#endif
