////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EASINGPOINT3DKEYFRAME_H__
#define __GUI_EASINGPOINT3DKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Point3DKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class EasingFunctionBase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A class that enables you to associate easing functions with a Point3DAnimationUsingKeyFrames
/// key frame animation.
///
/// This class is used as part of a Point3DKeyFrameCollection in conjunction with a
/// Point3DAnimationUsingKeyFrames to animate a Point3D property value along a set of key frames.
/// This Point3DKeyFrame interpolates between the Point3D value of the previous key frame and its
/// own value linearly with an easing function to produce its output value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.easingpoint3dkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API EasingPoint3DKeyFrame final: public Point3DKeyFrame
{
public:
    /// Gets or sets the easing function applied to the key frame
    //@{
    EasingFunctionBase* GetEasingFunction() const;
    void SetEasingFunction(EasingFunctionBase* function);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<EasingPoint3DKeyFrame> Clone() const;
    Ptr<EasingPoint3DKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* EasingFunctionProperty;
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

    NS_DECLARE_REFLECTION(EasingPoint3DKeyFrame, Point3DKeyFrame)
};

}


#endif
