////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_LINEARINT32KEYFRAME_H__
#define __GUI_LINEARINT32KEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Int32KeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates from the *Int32* value of the previous key frame to its own *Value* using linear
/// interpolation.
///
/// This class is used as part of a Int32KeyFrameCollection in conjunction with a
/// Int32AnimationUsingKeyFrames to animate a Int32 property value along a set of key frames.
///
/// A key frame defines a segment of the Int32AnimationUsingKeyFrames to which it belongs. Each key
/// frame has a target Value and a KeyTime. The KeyTime specifies the time at which the key frame's
/// Value should be reached. A key frame animates from the target value of the previous key frame to
/// its own target value. It starts when the previous key frame ends and ends when its own key time
/// is reached.
///
/// Linear key frames like LinearInt32KeyFrame create a smooth, linear interpolation between values.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.linearint32keyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API LinearInt32KeyFrame final: public Int32KeyFrame
{
public:
    // Hides Freezable methods for convenience
    //@{
    Ptr<LinearInt32KeyFrame> Clone() const;
    Ptr<LinearInt32KeyFrame> CloneCurrentValue() const;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From KeyFrame
    //@{
    int32_t InterpolateValueCore(int32_t baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(LinearInt32KeyFrame, Int32KeyFrame)
};

}


#endif
