////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RECTKEYFRAME_H__
#define __GUI_RECTKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

struct Rect;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// RectAnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.rectkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API RectKeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    const Rect& GetValue() const;
    void SetValue(const Rect& value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    Rect InterpolateValue(const Rect& baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<RectKeyFrame> Clone() const;
    Ptr<RectKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual Rect InterpolateValueCore(const Rect& baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(RectKeyFrame, BaseKeyFrame)

};

}

#endif
