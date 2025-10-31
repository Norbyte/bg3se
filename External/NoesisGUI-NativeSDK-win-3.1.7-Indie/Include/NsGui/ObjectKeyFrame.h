////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_OBJECTKEYFRAME_H__
#define __GUI_OBJECTKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// ObjectAnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.objectkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ObjectKeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    BaseComponent* GetValue() const;
    void SetValue(BaseComponent* value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    BaseComponent* InterpolateValue(BaseComponent* baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<ObjectKeyFrame> Clone() const;
    Ptr<ObjectKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual BaseComponent* InterpolateValueCore(BaseComponent*, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(ObjectKeyFrame, BaseKeyFrame)

};

}

#endif
