////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_THICKNESSKEYFRAME_H__
#define __GUI_THICKNESSKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class KeyTime;
struct Thickness;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// BooleanAnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.booleankeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ThicknessKeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    const Thickness& GetValue() const;
    void SetValue(const Thickness& value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    Thickness InterpolateValue(const Thickness& baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<ThicknessKeyFrame> Clone() const;
    Ptr<ThicknessKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual Thickness InterpolateValueCore(const Thickness& baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(ThicknessKeyFrame, BaseKeyFrame)

};

}

#endif
