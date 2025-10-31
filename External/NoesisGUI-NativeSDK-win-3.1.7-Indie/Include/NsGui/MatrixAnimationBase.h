////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MATRIXANIMATIONBASE_H__
#define __GUI_MATRIXANIMATIONBASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/AnimationTimeline.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class Transform2;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract class that, when implemented, animates a *Matrix* value.
///
/// Derive from a MatrixAnimationBase class and implement the *GetCurrentValueCore* method. The
/// *GetCurrentValueCore* method returns the current value of the animation. It takes three
/// parameters: a suggested starting value, a suggested ending value, and an AnimationClock, which
/// you use to determine the progress of the animation.
///
/// Because MatrixAnimationBase class inherit from the Freezable class, you must also override
/// *CreateInstanceCore* core to return a new instance of your class.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.matrixanimationbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API MatrixAnimationBase: public AnimationTimeline
{
public:
    /// Gets the current value of the animation
    Transform2 GetCurrentValue(const Transform2& defaultOrigin,
        const Transform2& defaultDestination, AnimationClock* clock);

protected:
    /// Calculates a value that represents the current value of the property being animated,
    /// as determined by the host animation
    virtual Transform2 GetCurrentValueCore(const Transform2& defaultOrigin,
        const Transform2& defaultDestination, AnimationClock* clock) = 0;

private:
    /// From AnimationTimeline
    //@{
    const Type* GetTargetPropertyType() const final override;
    Ptr<BaseComponent> GetCurrentValue(BaseComponent* defaultOrigin,
        BaseComponent* defaultDestination, AnimationClock* clock) final override;
    //@}

    NS_DECLARE_REFLECTION(MatrixAnimationBase, AnimationTimeline)
};

}

#endif
