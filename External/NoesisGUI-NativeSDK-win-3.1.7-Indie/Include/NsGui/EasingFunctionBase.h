////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EASINGFUNCTIONBASE_H__
#define __GUI_EASINGFUNCTIONBASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsGui/Animatable.h>
#include <NsGui/IEasingFunction.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies how the animation associated with an easing function interpolates.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum EasingMode
{
    /// Interpolation follows 100% interpolation minus the output of the formula associated with 
    /// the easing function
    EasingMode_EaseOut,
    /// Interpolation follows the mathematical formula associated with the easing function
    EasingMode_EaseIn,
    /// Interpolation uses EaseIn for the first half of the animation and EaseOut for the second 
    /// half
    EasingMode_EaseInOut
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides the base class for all the easing functions. You can create your own custom easing
/// functions by inheriting from this class.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.easingfunctionbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API EasingFunctionBase: public Animatable, public IEasingFunction
{
public:
    EasingFunctionBase();
    virtual ~EasingFunctionBase() = 0;

    /// Gets or sets a value that specifies how the animation interpolates
    //@{
    EasingMode GetEasingMode() const;
    void SetEasingMode(EasingMode mode);
    //@}

    /// From IEasingFunction
    //@{
    float Ease(float normalizedTime) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* EasingModeProperty;
    //@}

protected:
    /// Provides the logic portion of the easing function that you can override to produce the 
    /// EaseIn mode of the custom easing function
    virtual float EaseInCore(float normalizedTime) = 0;

    NS_DECLARE_REFLECTION(EasingFunctionBase, Animatable)
};

NS_WARNING_POP

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_ANIMATION_API, Noesis::EasingMode)


#endif
