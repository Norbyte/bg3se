////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_LINEARPOINT3DKEYFRAME_H__
#define __GUI_LINEARPOINT3DKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Point3DKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates from the *Point3D* value of the previous key frame to its own *Value* using linear
/// interpolation.
///
/// This class is used as part of a Point3DKeyFrameCollection in conjunction with a
/// Point3DAnimationUsingKeyFrames to animate a Point3D property value along a set of key frames.
///
/// A key frame defines a segment of the Point3DAnimationUsingKeyFrames to which it belongs. Each
/// key frame has a target Value and a KeyTime. The KeyTime specifies the time at which the key
/// frame's Value should be reached. A key frame animates from the target value of the previous key
/// frame to its own target value. It starts when the previous key frame ends and ends when its own
/// key time is reached.
///
/// Linear key frames like LinearPoint3DKeyFrame create a smooth, linear interpolation between values.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.linearpoint3dkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API LinearPoint3DKeyFrame final: public Point3DKeyFrame
{
public:
    // Hides Freezable methods for convenience
    //@{
    Ptr<LinearPoint3DKeyFrame> Clone() const;
    Ptr<LinearPoint3DKeyFrame> CloneCurrentValue() const;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From KeyFrame
    //@{
    Point3D InterpolateValueCore(const Point3D& baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(LinearPoint3DKeyFrame, Point3DKeyFrame)
};

}


#endif
