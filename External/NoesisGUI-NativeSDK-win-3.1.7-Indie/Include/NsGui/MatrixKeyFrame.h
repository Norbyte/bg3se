////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MATRIXKEYFRAME_H__
#define __GUI_MATRIXKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class Transform2;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// MatrixAnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.matrixkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API MatrixKeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    const Transform2& GetValue() const;
    void SetValue(const Transform2& value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    Transform2 InterpolateValue(const Transform2& baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<MatrixKeyFrame> Clone() const;
    Ptr<MatrixKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual Transform2 InterpolateValueCore(const Transform2& baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(MatrixKeyFrame, BaseKeyFrame)

};

}

#endif
