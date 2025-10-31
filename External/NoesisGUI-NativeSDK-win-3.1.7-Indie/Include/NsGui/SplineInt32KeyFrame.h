////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SPLINEINT32KEYFRAME_H__
#define __GUI_SPLINEINT32KEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Int32KeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class KeySpline;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates from the *Int32* value of the previous key frame to its own *Value* using splined 
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
/// Spline key frames like SplineInt32KeyFrame creates a variable transition between values which
/// is determined by the KeySpline property. Splined interpolation can be used to achieve more
/// realistic "real world" timing effects such as acceleration and deceleration.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.splineint32keyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API SplineInt32KeyFrame final: public Int32KeyFrame
{
public:
    /// Gets or sets the two control points that define animation progress for this key frame
    //@{
    KeySpline* GetKeySpline() const;
    void SetKeySpline(KeySpline* spline);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<SplineInt32KeyFrame> Clone() const;
    Ptr<SplineInt32KeyFrame> CloneCurrentValue() const;
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
    int32_t InterpolateValueCore(int32_t baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(SplineInt32KeyFrame, Int32KeyFrame)
};

}


#endif
