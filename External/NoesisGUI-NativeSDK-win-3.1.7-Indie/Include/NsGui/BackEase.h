////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BACKEASE_H__
#define __GUI_BACKEASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/EasingFunctionBase.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an easing function that retracts the motion of an animation slightly 
/// before it begins to animate in the path indicated.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.backease.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API BackEase: public EasingFunctionBase
{
public:
    BackEase();
    ~BackEase();

    /// Gets or sets the amplitude of retraction associated with a BackEase animation.
    //@{
    float GetAmplitude() const;
    void SetAmplitude(float amplitude);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<BackEase> Clone() const;
    Ptr<BackEase> CloneCurrentValue() const;
    //@}

public:
    static const DependencyProperty* AmplitudeProperty;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From EasingFunctionBase
    //@{
    float EaseInCore(float normalizedTime) override;
    //@}
    
    NS_DECLARE_REFLECTION(BackEase, EasingFunctionBase)
};

}


#endif
