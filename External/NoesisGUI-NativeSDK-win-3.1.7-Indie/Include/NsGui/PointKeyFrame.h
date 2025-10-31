////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_POINTKEYFRAME_H__
#define __GUI_POINTKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

struct Point;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// PointAnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.pointkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API PointKeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    const Point& GetValue() const;
    void SetValue(const Point& value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    Point InterpolateValue(const Point& baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<PointKeyFrame> Clone() const;
    Ptr<PointKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual Point InterpolateValueCore(const Point& baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(PointKeyFrame, BaseKeyFrame)

};

}

#endif
