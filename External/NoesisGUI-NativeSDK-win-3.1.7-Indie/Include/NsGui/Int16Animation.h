////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INT16ANIMATION_H__
#define __GUI_INT16ANIMATION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Int16AnimationBase.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

template<class T> class Nullable;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates the value of a *Int16* property between two 'target values' using linear interpolation
/// over a specified *Duration*.
///
/// It is common to specify a starting value (set it by using the *From* property) and an ending
/// value (set it by using the *To* property). However, you can also specify only a starting value,
/// a destination value, or an offset value. In these cases, the animation obtains the missing
/// target value from the property that is being animated.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.int16animation.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Int16Animation: public Int16AnimationBase
{
public:
    /// Gets or sets the total amount by which the animation changes its starting value
    //@{
    const Nullable<int16_t>& GetBy() const;
    void SetBy(const Nullable<int16_t>& by);
    //@}

    /// Gets or sets the animation's starting value
    //@{
    const Nullable<int16_t>& GetFrom() const;
    void SetFrom(const Nullable<int16_t>& from);
    //@}

    /// Gets or sets the animation's ending value
    //@{
    const Nullable<int16_t>& GetTo() const;
    void SetTo(const Nullable<int16_t>& to);
    //@}

    /// Gets or sets the easing function applied to this animation
    //@{
    EasingFunctionBase* GetEasingFunction() const;
    void SetEasingFunction(EasingFunctionBase* function);
    //@}

    /// From Freezable
    //@{
    Ptr<Int16Animation> Clone() const;
    Ptr<Int16Animation> CloneCurrentValue() const;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ByProperty;
    static const DependencyProperty* FromProperty;
    static const DependencyProperty* ToProperty;
    static const DependencyProperty* EasingFunctionProperty;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From AnimationTimeline
    //@{
    Ptr<AnimationTimeline> CreateTransitionFrom(EasingFunctionBase* easing) const override;
    Ptr<AnimationTimeline> CreateTransitionTo(EasingFunctionBase* easing) const override;
    //@}

    /// From Int16AnimationBase
    //@{
    int16_t GetCurrentValueCore(int16_t defaultOrigin, int16_t defaultDestination,
        AnimationClock* clock) override;
    //@}

private:
    NS_DECLARE_REFLECTION(Int16Animation, Int16AnimationBase)
};

}

#endif
