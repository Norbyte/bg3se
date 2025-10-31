////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DOUBLEKEYFRAME_H__
#define __GUI_DOUBLEKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// DoubleAnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.doublekeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API DoubleKeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    float GetValue() const;
    void SetValue(float value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    float InterpolateValue(float baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<DoubleKeyFrame> Clone() const;
    Ptr<DoubleKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual float InterpolateValueCore(float baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(DoubleKeyFrame, BaseKeyFrame)

};

}

#endif
