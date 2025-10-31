////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BOOLEANKEYFRAME_H__
#define __GUI_BOOLEANKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// BooleanAnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.booleankeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API BooleanKeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    bool GetValue() const;
    void SetValue(bool value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    bool InterpolateValue(bool baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<BooleanKeyFrame> Clone() const;
    Ptr<BooleanKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual bool InterpolateValueCore(bool baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(BooleanKeyFrame, BaseKeyFrame)

};

}

#endif
