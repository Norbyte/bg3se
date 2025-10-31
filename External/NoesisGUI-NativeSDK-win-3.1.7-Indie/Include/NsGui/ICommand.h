////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ICOMMAND_H__
#define __GUI_ICOMMAND_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsGui/Events.h>


namespace Noesis
{

class BaseComponent;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a command. Provide the commanding behavior for UI elements such as Button and MenuItem.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE ICommand: public Interface
{
    /// Occurs when something changes that affects whether or not the command should execute
    virtual EventHandler& CanExecuteChanged() = 0;

    /// Determines whether the command can execute in its current state
    virtual bool CanExecute(BaseComponent* param) const = 0;

    /// This method is called when command is invoked
    virtual void Execute(BaseComponent* param) const = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(ICommand, Interface)
};

}


#endif
