////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DISCRETETHICKNESSKEYFRAME_H__
#define __GUI_DISCRETETHICKNESSKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/ThicknessKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates from the *Thickness* value of the previous key frame to its own *Value* using discrete
/// interpolation.
///
/// his class is used as part of a ThicknessKeyFrameCollection in conjunction with a
/// ThicknessAnimationUsingKeyFrames to animate a Thickness property value along a set of key frames.
///
/// A key frame defines a segment of the ThicknessAnimationUsingKeyFrames to which it belongs. Each
/// key frame has a target Value and a KeyTime. The KeyTime specifies the time at which the key
/// frame's Value should be reached. A key frame animates from the target value of the previous key
/// frame to its own target value. It starts when the previous key frame ends and ends when its own
/// key time is reached.
///
/// Discrete key frames like DiscreteThicknessKeyFrame create sudden "jumps" between values (no
/// interpolation). In other words, the animated property does not change until the key frame's key
/// time is reached, at which point the animated property goes suddenly to the target value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discretethicknesskeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API DiscreteThicknessKeyFrame final: public ThicknessKeyFrame
{
public:
    // Hides Freezable methods for convenience
    //@{
    Ptr<DiscreteThicknessKeyFrame> Clone() const;
    Ptr<DiscreteThicknessKeyFrame> CloneCurrentValue() const;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From KeyFrame
    //@{
    Thickness InterpolateValueCore(const Thickness& baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(DiscreteThicknessKeyFrame, ThicknessKeyFrame)
};

}


#endif
