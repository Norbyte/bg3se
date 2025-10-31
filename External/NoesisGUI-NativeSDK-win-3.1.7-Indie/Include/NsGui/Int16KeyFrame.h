////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INT16KEYFRAME_H__
#define __GUI_INT16KEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// Int16AnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.int16keyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Int16KeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    int16_t GetValue() const;
    void SetValue(int16_t value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    int16_t InterpolateValue(int16_t baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<Int16KeyFrame> Clone() const;
    Ptr<Int16KeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual int16_t InterpolateValueCore(int16_t baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(Int16KeyFrame, BaseKeyFrame)

};

}

#endif
