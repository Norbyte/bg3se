////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EASINGRECTKEYFRAME_H__
#define __GUI_EASINGRECTKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/RectKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class EasingFunctionBase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A class that enables you to associate easing functions with a RectAnimationUsingKeyFrames
/// key frame animation.
///
/// For an example of using an object similar to this one, see EasingDoubleKeyFrame.
///
/// This class is used as part of a RectKeyFrameCollection in conjunction with a
/// RectAnimationUsingKeyFrames to animate a Rect property value along a set of key frames. This
/// RectKeyFrame interpolates between the Rect value of the previous key frame and its own value
/// linearly with an easing function to produce its output value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.easingrectkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API EasingRectKeyFrame final: public RectKeyFrame
{
public:
    /// Gets or sets the easing function applied to the key frame
    //@{
    EasingFunctionBase* GetEasingFunction() const;
    void SetEasingFunction(EasingFunctionBase* function);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<EasingRectKeyFrame> Clone() const;
    Ptr<EasingRectKeyFrame> CloneCurrentValue() const;
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
    Rect InterpolateValueCore(const Rect& baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(EasingRectKeyFrame, RectKeyFrame)
};

}


#endif
