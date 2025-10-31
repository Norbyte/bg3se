////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SPLINEPOIN3DTKEYFRAME_H__
#define __GUI_SPLINEPOIN3DTKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Point3DKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class KeySpline;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates from the *Point3D* value of the previous key frame to its own *Value* using splined 
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
/// Spline key frames like SplinePoint3DKeyFrame creates a variable transition between values which
/// is determined by the KeySpline property. Splined interpolation can be used to achieve more
/// realistic "real world" timing effects such as acceleration and deceleration.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.splinepoint3dkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API SplinePoint3DKeyFrame final: public Point3DKeyFrame
{
public:
    /// Gets or sets the two control Point3Ds that define animation progress for this key frame
    //@{
    KeySpline* GetKeySpline() const;
    void SetKeySpline(KeySpline* spline);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<SplinePoint3DKeyFrame> Clone() const;
    Ptr<SplinePoint3DKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* KeySplineProperty;
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

    NS_DECLARE_REFLECTION(SplinePoint3DKeyFrame, Point3DKeyFrame)
};

}


#endif
