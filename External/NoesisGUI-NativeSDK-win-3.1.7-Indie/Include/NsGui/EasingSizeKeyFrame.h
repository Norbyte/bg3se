////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EASINGSIZEKEYFRAME_H__
#define __GUI_EASINGSIZEKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/SizeKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class EasingFunctionBase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A class that enables you to associate easing functions with a SizeAnimationUsingKeyFrames
/// key frame animation.
///
/// For an example of using an object similar to this one, see EasingDoubleKeyFrame.
///
/// This class is used as part of a SizeKeyFrameCollection in conjunction with a
/// SizeAnimationUsingKeyFrames to animate a Size property value along a set of key frames. This
/// SizeKeyFrame interpolates between the Size value of the previous key frame and its own value
/// linearly with an easing function to produce its output value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.easingsizekeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API EasingSizeKeyFrame final: public SizeKeyFrame
{
public:
    /// Gets or sets the easing function applied to the key frame
    //@{
    EasingFunctionBase* GetEasingFunction() const;
    void SetEasingFunction(EasingFunctionBase* function);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<EasingSizeKeyFrame> Clone() const;
    Ptr<EasingSizeKeyFrame> CloneCurrentValue() const;
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
    Size InterpolateValueCore(const Size& baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(EasingSizeKeyFrame, SizeKeyFrame)
};

}


#endif
