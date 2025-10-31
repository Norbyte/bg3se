////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASECOMMAND_H__
#define __GUI_BASECOMMAND_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ICommand.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Delegate.h>
#include <NsCore/Symbol.h>

namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for commands.
///
/// *Commands* allows you to define actions in one place and then refer to them from all your user
/// interface controls like menu items, toolbar or buttons. Examples of commands are the *Copy*,
/// *Cut*, and *Paste* operations found on many applications. Applications often expose these
/// actions through many mechanisms simultaneously: *MenuItems* in a Menu, *MenuItems* on a
/// ContextMenu, *Buttons* on a ToolBar, *keyboard shortcuts* and so on.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseCommand: public BaseComponent, public ICommand
{
public:
    BaseCommand();
    virtual ~BaseCommand() = 0;

    // From ICommand
    //@{
    EventHandler& CanExecuteChanged() override;
    bool CanExecute(BaseComponent* param) const override;
    void Execute(BaseComponent* param) const override;
    //@}

    void RaiseCanExecuteChanged();

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    EventHandler mCanExecuteChanged;

    NS_DECLARE_REFLECTION(BaseCommand, BaseComponent)
};

NS_WARNING_POP

}

#endif
