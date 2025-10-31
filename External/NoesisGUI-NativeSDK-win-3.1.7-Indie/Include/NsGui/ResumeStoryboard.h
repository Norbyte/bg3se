////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RESUMESTORYBOARD_H__
#define __GUI_RESUMESTORYBOARD_H__


#include <NsCore/Noesis.h>
#include <NsGui/AnimationApi.h>
#include <NsGui/ControllableStoryboardAction.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/String.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A trigger action that resumes a paused Storyboard.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.resumestoryboard.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API ResumeStoryboard final: public ControllableStoryboardAction
{
protected:
    /// ControllableStoryboardAction
    //@{
    void Invoke(FrameworkElement* target, Storyboard* storyboard) override;
    //@}

    NS_DECLARE_REFLECTION(ResumeStoryboard, ControllableStoryboardAction)
};

}


#endif
