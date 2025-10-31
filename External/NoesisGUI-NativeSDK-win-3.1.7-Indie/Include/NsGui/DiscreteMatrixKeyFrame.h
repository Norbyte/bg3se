////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DISCRETEMATRIXKEYFRAME_H__
#define __GUI_DISCRETEMATRIXKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/MatrixKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates from the *Matrix* value of the previous key frame to its own *Value* using discrete
/// interpolation.
///
/// This class is used as part of a MatrixKeyFrameCollection in conjunction with a
/// MatrixAnimationUsingKeyFrames to animate a Matrix property value along a set of key frames.
///
/// A key frame defines a segment of the MatrixAnimationUsingKeyFrames to which it belongs. Each key
/// frame has a target Value and a KeyTime. The KeyTime specifies the time at which the key frame's
/// Value should be reached. A key frame animates from the target value of the previous key frame to
/// its own target value. It starts when the previous key frame ends and ends when its own key time
/// is reached.
///
/// Discrete key frames like DiscreteMatrixKeyFrame create sudden "jumps" between values (no
/// Interpolation). In other words, the animated property does not change until the key frame's key
/// time is reached at which point the animated property goes suddenly to the target value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discretematrixkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API DiscreteMatrixKeyFrame final: public MatrixKeyFrame
{
public:
    // Hides Freezable methods for convenience
    //@{
    Ptr<DiscreteMatrixKeyFrame> Clone() const;
    Ptr<DiscreteMatrixKeyFrame> CloneCurrentValue() const;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From KeyFrame
    //@{
    Transform2 InterpolateValueCore(const Transform2& baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(DiscreteMatrixKeyFrame, MatrixKeyFrame)
};

}


#endif
