////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DISCRETEPOINT3DKEYFRAME_H__
#define __GUI_DISCRETEPOINT3DKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Point3DKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates from the *Point3D* value of the previous key frame to its own *Value* using discrete
/// interpolation.
///
/// This class is used as part of a Point3DKeyFrameCollection object in conjunction with a
/// Point3DAnimationUsingKeyFrames object to animate a Point3D property value along a set of key
/// frames.
///
/// A key frame defines a segment of the Point3DAnimationUsingKeyFrames to which it belongs. Each
/// key frame has a target Value and a KeyTime. The KeyTime specifies the time at which the key
/// frame's Value should be reached. A key frame animates from the target value of the previous key
/// frame to its own target value. It starts when the previous key frame ends and ends when its own
/// key time is reached.
///
/// Discrete key frames like DiscretePoint3DKeyFrame create sudden "jumps" between values (no
/// interpolation). In other words, the animated property does not change until the key frame's key
/// time is reached, at which point the animated property goes suddenly to the target value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discretepoint3dkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API DiscretePoint3DKeyFrame final: public Point3DKeyFrame
{
public:
    // Hides Freezable methods for convenience
    //@{
    Ptr<DiscretePoint3DKeyFrame> Clone() const;
    Ptr<DiscretePoint3DKeyFrame> CloneCurrentValue() const;
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

    NS_DECLARE_REFLECTION(DiscretePoint3DKeyFrame, Point3DKeyFrame)
};

}


#endif
