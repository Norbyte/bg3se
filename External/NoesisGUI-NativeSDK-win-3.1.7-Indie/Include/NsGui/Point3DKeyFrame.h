////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_POINT3DKEYFRAME_H__
#define __GUI_POINT3DKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

struct Point3D;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// Point3DAnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.point3dkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Point3DKeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    const Point3D& GetValue() const;
    void SetValue(const Point3D& value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    Point3D InterpolateValue(const Point3D& baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<Point3DKeyFrame> Clone() const;
    Ptr<Point3DKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual Point3D InterpolateValueCore(const Point3D& baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(Point3DKeyFrame, BaseKeyFrame)

};

}

#endif
