////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EASINGDOUBLEKEYFRAME_H__
#define __GUI_EASINGDOUBLEKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/DoubleKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class EasingFunctionBase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A class that enables you to associate easing functions with a DoubleAnimationUsingKeyFrames
/// key frame animation.
///
/// The following example shows how to use key frames with easing functions associated with them to
/// create an animation of a rectangle that contracts upward, slows down, then expands downward (as
/// though falling), and then bounces to a stop.
///
/// .. code-block:: xml
///
///    <Rectangle Name="myRectangle" Width="200" Height="200" Fill="Blue">
///      <Rectangle.Triggers>
///        <EventTrigger RoutedEvent="Rectangle.MouseDown">
///          <BeginStoryboard>
///            <Storyboard>
///              <DoubleAnimationUsingKeyFrames
///                Storyboard.TargetProperty="Height"
///                Storyboard.TargetName="myRectangle">
///
///                <!-- This keyframe animates the ellipse up to the crest
///                                 where it slows down and stops. -->
///                <EasingDoubleKeyFrame Value="30" KeyTime="00:00:02">
///                  <EasingDoubleKeyFrame.EasingFunction>
///                    <CubicEase EasingMode="EaseOut"/>
///                  </EasingDoubleKeyFrame.EasingFunction>
///                </EasingDoubleKeyFrame>
///
///                <!-- This keyframe animates the ellipse back down and makes
///                                 it bounce. -->
///                <EasingDoubleKeyFrame Value="200" KeyTime="00:00:06">
///                  <EasingDoubleKeyFrame.EasingFunction>
///                    <BounceEase Bounces="5" EasingMode="EaseOut"/>
///                  </EasingDoubleKeyFrame.EasingFunction>
///                </EasingDoubleKeyFrame>
///
///              </DoubleAnimationUsingKeyFrames>
///            </Storyboard>
///          </BeginStoryboard>
///        </EventTrigger>
///      </Rectangle.Triggers>
///    </Rectangle>
///
/// This class is used as part of a DoubleKeyFrameCollection in conjunction with a
/// DoubleAnimationUsingKeyFrames to animate a Double property value along a set of key frames.
/// This DoubleKeyFrame interpolates between the Double value of the previous key frame and its own
/// value linearly with an easing function to produce its output value.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.easingdoublekeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API EasingDoubleKeyFrame final: public DoubleKeyFrame
{
public:
    /// Gets or sets the easing function applied to the key frame
    //@{
    EasingFunctionBase* GetEasingFunction() const;
    void SetEasingFunction(EasingFunctionBase* function);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<EasingDoubleKeyFrame> Clone() const;
    Ptr<EasingDoubleKeyFrame> CloneCurrentValue() const;
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
    float InterpolateValueCore(float baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(EasingDoubleKeyFrame, DoubleKeyFrame)
};

}


#endif
