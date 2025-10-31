////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_QUARTICEASE_H__
#define __GUI_QUARTICEASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/EasingFunctionBase.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an easing function that creates an animation that accelerates and/or
/// decelerates using the formula f(t) = t\ :sup:`4`.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.quarticease.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API QuarticEase: public EasingFunctionBase
{
public:
    QuarticEase();
    ~QuarticEase();

    // Hides Freezable methods for convenience
    //@{
    Ptr<QuarticEase> Clone() const;
    Ptr<QuarticEase> CloneCurrentValue() const;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From EasingFunctionBase
    //@{
    float EaseInCore(float normalizedTime) override;
    //@}

    NS_DECLARE_REFLECTION(QuarticEase, EasingFunctionBase)
};

}


#endif
