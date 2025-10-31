////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SIZEKEYFRAME_H__
#define __GUI_SIZEKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

struct Size;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// SizeAnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.sizekeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API SizeKeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    const Size& GetValue() const;
    void SetValue(const Size& value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    Size InterpolateValue(const Size& baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<SizeKeyFrame> Clone() const;
    Ptr<SizeKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual Size InterpolateValueCore(const Size& baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(SizeKeyFrame, BaseKeyFrame)

};

}

#endif
