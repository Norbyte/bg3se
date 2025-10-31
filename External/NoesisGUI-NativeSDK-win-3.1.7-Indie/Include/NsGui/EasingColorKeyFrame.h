////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EASINGCOLORKEYFRAME_H__
#define __GUI_EASINGCOLORKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/ColorKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class EasingFunctionBase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A class that enables you to associate easing functions with a ColorAnimationUsingKeyFrames
/// key frame animation.
///
/// For an example of using an object similar to this one, see EasingDoubleKeyFrame.
///
/// This class is used as part of a ColorKeyFrameCollection in conjunction with a
/// ColorAnimationUsingKeyFrames to animate a Color property value along a set of key frames.
/// This ColorKeyFrame interpolates between the Color value of the previous key frame and its own
/// value linearly with an easing function to produce its output value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.easingcolorkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API EasingColorKeyFrame final: public ColorKeyFrame
{
public:
    /// Gets or sets the easing function applied to the key frame
    //@{
    EasingFunctionBase* GetEasingFunction() const;
    void SetEasingFunction(EasingFunctionBase* function);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<EasingColorKeyFrame> Clone() const;
    Ptr<EasingColorKeyFrame> CloneCurrentValue() const;
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
    Color InterpolateValueCore(const Color& baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(EasingColorKeyFrame, ColorKeyFrame)
};

}


#endif
