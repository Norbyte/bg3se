////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INT32ANIMATIONBASE_H__
#define __GUI_INT32ANIMATIONBASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/AnimationTimeline.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract class that, when implemented, animates a *Int32* value.
///
/// Derive from a Int32AnimationBase class and implement the *GetCurrentValueCore* method. The
/// *GetCurrentValueCore* method returns the current value of the animation. It takes three
/// parameters: a suggested starting value, a suggested ending value, and an AnimationClock, which
/// you use to determine the progress of the animation.
///
/// Because Int32AnimationBase class inherit from the Freezable class, you must also override
/// *CreateInstanceCore* core to return a new instance of your class.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.int32animationbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API Int32AnimationBase: public AnimationTimeline
{
public:
    /// Gets the current value of the animation
    int32_t GetCurrentValue(int32_t defaultOrigin, int32_t defaultDestination, AnimationClock* clock);

protected:
    /// Calculates a value that represents the current value of the property being animated,
    /// as determined by the host animation
    virtual int32_t GetCurrentValueCore(int32_t defaultOrigin, int32_t defaultDestination,
        AnimationClock* clock) = 0;

private:
    /// From AnimationTimeline
    //@{
    const Type* GetTargetPropertyType() const final override;
    Ptr<BaseComponent> GetCurrentValue(BaseComponent* defaultOrigin,
        BaseComponent* defaultDestination, AnimationClock* clock) final override;
    //@}

    NS_DECLARE_REFLECTION(Int32AnimationBase, AnimationTimeline)
};

}

#endif
