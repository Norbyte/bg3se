////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_LINEARCOLORKEYFRAME_H__
#define __GUI_LINEARCOLORKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/ColorKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates from the *Color* value of the previous key frame to its own *Value* using linear
/// interpolation.
///
/// This class is used as part of a ColorKeyFrameCollection in conjunction with a
/// ColorAnimationUsingKeyFrames to animate a Color property value along a set of key frames.
///
/// A key frame defines a segment of the ColorAnimationUsingKeyFrames to which it belongs. Each key
/// frame has a target Value and a KeyTime. The KeyTime specifies the time at which the key frame's
/// Value should be reached. A key frame animates from the target value of the previous key frame to
/// its own target value. It starts when the previous key frame ends and ends when its own key time
/// is reached.
///
/// Linear key frames like LinearColorKeyFrame create a smooth, linear interpolation between values.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.linearcolorkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API LinearColorKeyFrame final: public ColorKeyFrame
{
public:
    // Hides Freezable methods for convenience
    //@{
    Ptr<LinearColorKeyFrame> Clone() const;
    Ptr<LinearColorKeyFrame> CloneCurrentValue() const;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From KeyFrame
    //@{
    Color InterpolateValueCore(const Color& baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(LinearColorKeyFrame, ColorKeyFrame)
};

}


#endif
