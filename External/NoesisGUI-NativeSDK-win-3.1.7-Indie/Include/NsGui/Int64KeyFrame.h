////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INT64KEYFRAME_H__
#define __GUI_INT64KEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// Int64AnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.int64keyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Int64KeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    int64_t GetValue() const;
    void SetValue(int64_t value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    int64_t InterpolateValue(int64_t baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<Int64KeyFrame> Clone() const;
    Ptr<Int64KeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual int64_t InterpolateValueCore(int64_t baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(Int64KeyFrame, BaseKeyFrame)

};

}

#endif
