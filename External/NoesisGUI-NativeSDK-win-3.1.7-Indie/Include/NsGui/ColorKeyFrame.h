////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COLORKEYFRAME_H__
#define __GUI_COLORKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

struct Color;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// ColorAnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.colorkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ColorKeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    const Color& GetValue() const;
    void SetValue(const Color& value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    Color InterpolateValue(const Color& baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<ColorKeyFrame> Clone() const;
    Ptr<ColorKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual Color InterpolateValueCore(const Color& baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(ColorKeyFrame, BaseKeyFrame)

};

}

#endif
