////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BEGINSTORYBOARD_H__
#define __GUI_BEGINSTORYBOARD_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/TriggerAction.h>
#include <NsGui/HandoffBehavior.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsCore/String.h>


namespace Noesis
{

class Storyboard;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A trigger action that begins a Storyboard and distributes its animations to their targeted 
/// objects and properties.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.beginstoryboard.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API BeginStoryboard final: public TriggerAction
{
public:
    BeginStoryboard();

    /// Gets or sets the proper hand-off behavior to start an animation clock in this storyboard
    //@{
    HandoffBehavior GetHandoffBehavior() const;
    void SetHandoffBehavior(HandoffBehavior behavior);
    //@}

    /// Gets or sets the name of the BeginStoryboard object. By naming the BeginStoryboard object,
    /// the Storyboard can be controlled after it is started
    //@{
    const char* GetName() const;
    void SetName(const char* name);
    //@}

    /// Gets or sets the Storyboard that this BeginStoryboard starts
    //@{
    Storyboard* GetStoryboard() const;
    void SetStoryboard(Storyboard* storyboard);
    //@}

    /// From TriggerAction
    //@{
    void Invoke(FrameworkElement* target, FrameworkElement* nameScope = 0) override;
    void Seal() override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* StoryboardProperty;
    //@}

private:
    HandoffBehavior mHandoffBehavior;
    String mName;
    bool mValidTargets;

    NS_DECLARE_REFLECTION(BeginStoryboard, TriggerAction)
};

NS_WARNING_POP

}


#endif
