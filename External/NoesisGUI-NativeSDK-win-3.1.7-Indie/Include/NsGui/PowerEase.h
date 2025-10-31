////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_POWEREASE_H__
#define __GUI_POWEREASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/EasingFunctionBase.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an easing function that creates an animation that accelerates and/or 
/// decelerates using the formula f(t) = t\ :sup:`p` where p is equal to the *Power* property.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.powerease.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API PowerEase: public EasingFunctionBase
{
public:
    PowerEase();
    ~PowerEase();

    /// Gets or sets the exponential power of the animation interpolation. For example, a value of 7
    /// will create an animation interpolation curve that follows the formula f(t) = t7.
    //@{
    float GetPower() const;
    void SetPower(float power);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<PowerEase> Clone() const;
    Ptr<PowerEase> CloneCurrentValue() const;
    //@}

public:
    static const DependencyProperty* PowerProperty;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From EasingFunctionBase
    //@{
    float EaseInCore(float normalizedTime) override;
    //@}

    NS_DECLARE_REFLECTION(PowerEase, EasingFunctionBase)
};

}


#endif
