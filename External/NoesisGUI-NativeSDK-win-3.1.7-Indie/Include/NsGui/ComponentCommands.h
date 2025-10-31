////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMPONENTCOMMANDS_H__
#define __GUI_COMPONENTCOMMANDS_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class RoutedUICommand;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a standard set of component related commands.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.input.componentcommands.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API ComponentCommands
{
    /// Gets the value that represents the *Extend Selection Down* command
    /// \prop
    static const RoutedUICommand* ExtendSelectionDownCommand;

    /// Gets the value that represents the *Extend Selection Left* command
    /// \prop
    static const RoutedUICommand* ExtendSelectionLeftCommand;

    /// Gets the value that represents the *Extend Selection Right* command
    /// \prop
    static const RoutedUICommand* ExtendSelectionRightCommand;

    /// Gets the value that represents the *Extend Selection Up* command
    /// \prop
    static const RoutedUICommand* ExtendSelectionUpCommand;

    /// Gets the value that represents the *Move Down* command
    /// \prop
    static const RoutedUICommand* MoveDownCommand;

    /// Gets the value that represents the *Move Focus Back* command
    /// \prop
    static const RoutedUICommand* MoveFocusBackCommand;

    /// Gets the value that represents the *Move Focus Down* command
    /// \prop
    static const RoutedUICommand* MoveFocusDownCommand;

    /// Gets the value that represents the *Move Focus Forward* command
    /// \prop
    static const RoutedUICommand* MoveFocusForwardCommand;

    /// Gets the value that represents the *Move Focus Page Down* command
    /// \prop
    static const RoutedUICommand* MoveFocusPageDownCommand;

    /// Gets the value that represents the *Move Focus Page Up* command
    /// \prop
    static const RoutedUICommand* MoveFocusPageUpCommand;

    /// Gets the value that represents the *Move Focus Up* command
    /// \prop
    static const RoutedUICommand* MoveFocusUpCommand;

    /// Gets the value that represents the *Move Left* command
    /// \prop
    static const RoutedUICommand* MoveLeftCommand;

    /// Gets the value that represents the *Move Right* command
    /// \prop
    static const RoutedUICommand* MoveRightCommand;

    /// Gets the value that represents the *Move to End* command
    /// \prop
    static const RoutedUICommand* MoveToEndCommand;

    /// Gets the value that represents the *Move to Home* command
    /// \prop
    static const RoutedUICommand* MoveToHomeCommand;

    /// Gets the value that represents the *Move to Page Down* command
    /// \prop
    static const RoutedUICommand* MoveToPageDownCommand;

    /// Gets the value that represents the *Move to Page Up* command
    /// \prop
    static const RoutedUICommand* MoveToPageUpCommand;

    /// Gets the value that represents the *Move Up* command
    /// \prop
    static const RoutedUICommand* MoveUpCommand;

    /// Gets the value that represents the *Scroll by Line* command
    /// \prop
    static const RoutedUICommand* ScrollByLineCommand;

    /// Gets the value that represents the *Scroll Page Down* command
    /// \prop
    static const RoutedUICommand* ScrollPageDownCommand;

    /// Gets the value that represents the *Scroll Page Left* command
    /// \prop
    static const RoutedUICommand* ScrollPageLeftCommand;

    /// Gets the value that represents the *Scroll Page Right* command
    /// \prop
    static const RoutedUICommand* ScrollPageRightCommand;

    /// Gets the value that represents the *Scroll Page Up* command
    static const RoutedUICommand* ScrollPageUpCommand;
    /// \prop

    /// Gets the value that represents the *Select to End* command
    /// \prop
    static const RoutedUICommand* SelectToEndCommand;

    /// Gets the value that represents the *Select to Home* command
    /// \prop
    static const RoutedUICommand* SelectToHomeCommand;

    /// Gets the value that represents the *Select to Page Down* command
    /// \prop
    static const RoutedUICommand* SelectToPageDownCommand;

    /// Gets the value that represents the *Select to Page Up* command
    /// \prop
    static const RoutedUICommand* SelectToPageUp;

    NS_DECLARE_REFLECTION(ComponentCommands, NoParent)
};

}

#endif
