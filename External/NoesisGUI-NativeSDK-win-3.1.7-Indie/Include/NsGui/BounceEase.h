////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BOUNCEEASE_H__
#define __GUI_BOUNCEEASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/EasingFunctionBase.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an easing function that creates an animated bouncing effect.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.bounceease.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API BounceEase: public EasingFunctionBase
{
public:
    BounceEase();
    ~BounceEase();

    /// Gets or sets the number of bounces
    //@{
    int32_t GetBounces() const;
    void SetBounces(int32_t bounces);
    //@}

    /// Gets or sets a value that specifies how bouncy the bounce animation is. Low values of this 
    /// property result in bounces with little lose of height between bounces (more bouncy) while 
    /// high values result in dampened bounces (less bouncy)
    //@{
    float GetBounciness() const;
    void SetBounciness(float bounciness);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<BounceEase> Clone() const;
    Ptr<BounceEase> CloneCurrentValue() const;
    //@}

public:
    static const DependencyProperty* BouncesProperty;
    static const DependencyProperty* BouncinessProperty;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From EasingFunctionBase
    //@{
    float EaseInCore(float normalizedTime) override;
    //@}
    
    NS_DECLARE_REFLECTION(BounceEase, EasingFunctionBase)
};

}


#endif
