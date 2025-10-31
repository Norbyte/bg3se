////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EASINGPOINTKEYFRAME_H__
#define __GUI_EASINGPOINTKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/PointKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class EasingFunctionBase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A class that enables you to associate easing functions with a PointAnimationUsingKeyFrames
/// key frame animation.
///
/// This class is used as part of a PointKeyFrameCollection in conjunction with a
/// PointAnimationUsingKeyFrames to animate a Point property value along a set of key frames. This
/// PointKeyFrame interpolates between the Point value of the previous key frame and its own value
/// linearly with an easing function to produce its output value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.easingpointkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API EasingPointKeyFrame final: public PointKeyFrame
{
public:
    /// Gets or sets the easing function applied to the key frame
    //@{
    EasingFunctionBase* GetEasingFunction() const;
    void SetEasingFunction(EasingFunctionBase* function);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<EasingPointKeyFrame> Clone() const;
    Ptr<EasingPointKeyFrame> CloneCurrentValue() const;
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
    Point InterpolateValueCore(const Point& baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(EasingPointKeyFrame, PointKeyFrame)
};

}


#endif
