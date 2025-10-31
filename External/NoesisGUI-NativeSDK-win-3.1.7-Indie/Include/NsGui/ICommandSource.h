////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ICOMMANDSOURCE_H__
#define __GUI_ICOMMANDSOURCE_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>


namespace Noesis
{

class BaseComponent;
class UIElement;
NS_INTERFACE ICommand;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an object that knows how to invoke a command.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE ICommandSource: public Interface
{
    /// Gets the command that will be executed when the command source is invoked
    virtual ICommand* GetCommand() const = 0;

    /// Represents a user defined data value that can be passed to the command when it is executed
    virtual BaseComponent* GetCommandParameter() const = 0;

    /// The object that the command is being executed on
    virtual UIElement* GetCommandTarget() const = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(ICommandSource, Interface)
};

}


#endif
