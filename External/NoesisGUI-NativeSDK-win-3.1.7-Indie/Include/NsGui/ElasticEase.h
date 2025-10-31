////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ELASTICEASE_H__
#define __GUI_ELASTICEASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/EasingFunctionBase.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an easing function that creates an animation that resembles a spring oscillating
/// back and forth until it comes to rest.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.elasticease.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ElasticEase: public EasingFunctionBase
{
public:
    ElasticEase();
    ~ElasticEase();

    /// Gets or sets the number of times the target slides back and forth over the animation 
    /// destination
    //@{
    int32_t GetOscillations() const;
    void SetOscillations(int32_t oscillations);
    //@}

    /// Gets or sets the stiffness of the spring. The smaller the Springiness value is, the stiffer 
    /// the spring and the faster the elasticity decreases in intensity over each oscillation
    //@{
    float GetSpringiness() const;
    void SetSpringiness(float value);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<ElasticEase> Clone() const;
    Ptr<ElasticEase> CloneCurrentValue() const;
    //@}

public:
    static const DependencyProperty* OscillationsProperty;
    static const DependencyProperty* SpringinessProperty;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From EasingFunctionBase
    //@{
    float EaseInCore(float normalizedTime) override;
    //@}

    NS_DECLARE_REFLECTION(ElasticEase, EasingFunctionBase)
};

}


#endif
