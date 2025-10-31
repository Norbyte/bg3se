////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EASINGINT32KEYFRAME_H__
#define __GUI_EASINGINT32KEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Int32KeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class EasingFunctionBase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A class that enables you to associate easing functions with a Int32AnimationUsingKeyFrames
/// key frame animation.
///
/// For an example of using an object similar to this one, see EasingDoubleKeyFrame.
///
/// This class is used as part of an Int32KeyFrameCollection in conjunction with an
/// Int32AnimationUsingKeyFrames to animate an Int32 property value along a set of key frames.
/// This Int32KeyFrame interpolates between the Int32 value of the previous key frame and its own
/// value linearly with an easing function to produce its output value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.easingint32keyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API EasingInt32KeyFrame final: public Int32KeyFrame
{
public:
    /// Gets or sets the easing function applied to the key frame
    //@{
    EasingFunctionBase* GetEasingFunction() const;
    void SetEasingFunction(EasingFunctionBase* function);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<EasingInt32KeyFrame> Clone() const;
    Ptr<EasingInt32KeyFrame> CloneCurrentValue() const;
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
    int32_t InterpolateValueCore(int32_t baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(EasingInt32KeyFrame, Int32KeyFrame)
};

}


#endif
