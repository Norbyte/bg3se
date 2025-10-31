////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_OBJECTANIMATIONBASE_H__
#define __GUI_OBJECTANIMATIONBASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/AnimationTimeline.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract class that, when implemented, animates a *Object* value.
///
/// Derive from a ObjectAnimationBase class and implement the *GetCurrentValueCore* method. The
/// *GetCurrentValueCore* method returns the current value of the animation. It takes three
/// parameters: a suggested starting value, a suggested ending value, and an AnimationClock, which
/// you use to determine the progress of the animation.
///
/// Because ObjectAnimationBase class inherit from the Freezable class, you must also override
/// *CreateInstanceCore* core to return a new instance of your class.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.objectanimationbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ObjectAnimationBase: public AnimationTimeline
{
public:
    /// Gets the current value of the animation
    Ptr<BaseComponent> GetCurrentValue(BaseComponent* defaultOrigin,
        BaseComponent* defaultDestination, AnimationClock* clock) final override;

protected:
    /// Calculates a value that represents the current value of the property being animated,
    /// as determined by the host animation
    virtual Ptr<BaseComponent> GetCurrentValueCore(BaseComponent* defaultOrigin,
        BaseComponent* defaultDestination, AnimationClock* clock) = 0;

private:
    /// From AnimationTimeline
    //@{
    const Type* GetTargetPropertyType() const final override;
    //@}

    NS_DECLARE_REFLECTION(ObjectAnimationBase, AnimationTimeline)
};

}

#endif
