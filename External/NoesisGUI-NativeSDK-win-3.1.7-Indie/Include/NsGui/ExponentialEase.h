////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EXPONENTIALEASE_H__
#define __GUI_EXPONENTIALEASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/EasingFunctionBase.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an easing function that creates an animation that accelerates and/or decelerates 
/// using an exponential formula.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.exponentialease.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ExponentialEase: public EasingFunctionBase
{
public:
    ExponentialEase();
    ~ExponentialEase();

    /// Gets or sets the exponent used to determine the interpolation of the animation.
    //@{
    float GetExponent() const;
    void SetExponent(float exponent);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<ExponentialEase> Clone() const;
    Ptr<ExponentialEase> CloneCurrentValue() const;
    //@}

public:
    static const DependencyProperty* ExponentProperty;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From EasingFunctionBase
    //@{
    float EaseInCore(float normalizedTime) override;
    //@}

    NS_DECLARE_REFLECTION(ExponentialEase, EasingFunctionBase)
};

}


#endif
