////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INT32KEYFRAME_H__
#define __GUI_INT32KEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// Int32AnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.int32keyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Int32KeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    int32_t GetValue() const;
    void SetValue(int32_t value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    int32_t InterpolateValue(int32_t baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<Int32KeyFrame> Clone() const;
    Ptr<Int32KeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual int32_t InterpolateValueCore(int32_t baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(Int32KeyFrame, BaseKeyFrame)

};

}

#endif
