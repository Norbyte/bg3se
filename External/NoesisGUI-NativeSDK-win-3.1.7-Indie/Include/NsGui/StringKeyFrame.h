////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_STRINGKEYFRAME_H__
#define __GUI_STRINGKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/StringFwd.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/BaseKeyFrame.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an animation segment with its own target value and interpolation method for a
/// StringAnimationUsingKeyFrames.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.stringkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API StringKeyFrame: public BaseKeyFrame
{
public:
    /// Gets or sets the key frame's target value
    //@{
    const char* GetValue() const;
    void SetValue(const char* value);
    //@}

    /// Returns the interpolated value of a specific key frame at the progress increment provided
    String InterpolateValue(const String& baseValue, float keyFrameProgress);

    // Hides Freezable methods for convenience
    //@{
    Ptr<StringKeyFrame> Clone() const;
    Ptr<StringKeyFrame> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ValueProperty;
    //@}

protected:
    virtual String InterpolateValueCore(const String& baseValue, float keyFrameProgress) = 0;

private:
    NS_DECLARE_REFLECTION(StringKeyFrame, BaseKeyFrame)

};

}

#endif
