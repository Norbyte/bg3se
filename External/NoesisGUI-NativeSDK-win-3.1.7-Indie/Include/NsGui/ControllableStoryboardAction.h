////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CONTROLLABLESTORYBOARDACTION_H__
#define __GUI_CONTROLLABLESTORYBOARDACTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/TriggerAction.h>
#include <NsCore/String.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class Storyboard;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Manipulates a Storyboard that has been applied by a BeginStoryboard action.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.controllablestoryboardaction.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ControllableStoryboardAction: public TriggerAction
{
public:
    ControllableStoryboardAction();
    virtual ~ControllableStoryboardAction() = 0;

    /// Gets or sets the *Name* of the BeginStoryboard that began the Storyboard you want to
    /// interactively control
    //@{
    const char* GetBeginStoryboardName() const;
    void SetBeginStoryboardName(const char* name);
    //@}

    /// From TriggerAction
    //@{
    void Invoke(FrameworkElement* target, FrameworkElement* nameScope = 0) override;
    //@}

protected:
    virtual void Invoke(FrameworkElement* target, Storyboard* storyboard) = 0;

private:
    Storyboard* GetStoryboard(FrameworkElement* nameScope) const;

private:
    String mBeginStoryboardName;

    NS_DECLARE_REFLECTION(ControllableStoryboardAction, TriggerAction)
};

NS_WARNING_POP

}


#endif
