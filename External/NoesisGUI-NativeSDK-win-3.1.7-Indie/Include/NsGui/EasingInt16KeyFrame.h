////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EASINGINT16KEYFRAME_H__
#define __GUI_EASINGINT16KEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Int16KeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class EasingFunctionBase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A class that enables you to associate easing functions with a Int16AnimationUsingKeyFrames
/// key frame animation.
///
/// For an example of using an object similar to this one, see EasingDoubleKeyFrame.
///
/// This class is used as part of an Int16KeyFrameCollection in conjunction with an
/// Int16AnimationUsingKeyFrames to animate an Int16 property value along a set of key frames.
/// This Int16KeyFrame interpolates between the Int16 value of the previous key frame and its own
/// value linearly with an easing function to produce its output value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.easingint16keyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API EasingInt16KeyFrame final: public Int16KeyFrame
{
public:
    /// Gets or sets the easing function applied to the key frame
    //@{
    EasingFunctionBase* GetEasingFunction() const;
    void SetEasingFunction(EasingFunctionBase* function);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<EasingInt16KeyFrame> Clone() const;
    Ptr<EasingInt16KeyFrame> CloneCurrentValue() const;
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
    int16_t InterpolateValueCore(int16_t baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(EasingInt16KeyFrame, Int16KeyFrame)
};

}


#endif
