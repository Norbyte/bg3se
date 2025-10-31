////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_APPLICATIONCOMMANDS_H__
#define __GUI_APPLICATIONCOMMANDS_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class RoutedUICommand;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a standard set of application related commands.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.input.applicationcommands.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API ApplicationCommands
{
    /// Gets the value that represents the Cancel Print command.
    /// \prop
    static const RoutedUICommand* CancelPrintCommand;

    /// Gets the value that represents the Close command.
    /// \prop
    static const RoutedUICommand* CloseCommand;

    /// Gets the value that represents the Context Menu command.
    /// \prop
    static const RoutedUICommand* ContextMenuCommand;

    /// Gets the value that represents the Copy command.
    /// \prop
    static const RoutedUICommand* CopyCommand;

    /// Gets the value that represents the Correction List command.
    /// \prop
    static const RoutedUICommand* CorrectionListCommand;

    /// Gets the value that represents the Cut command.
    /// \prop
    static const RoutedUICommand* CutCommand;

    /// Gets the value that represents the Delete command.
    /// \prop
    static const RoutedUICommand* DeleteCommand;

    /// Gets the value that represents the Find command.
    /// \prop
    static const RoutedUICommand* FindCommand;

    /// Gets the value that represents the Help command.
    /// \prop
    static const RoutedUICommand* HelpCommand;

    /// Gets the value that represents the New command.
    /// \prop
    static const RoutedUICommand* NewCommand;

    /// Gets the value that represents the Open command.
    /// \prop
    static const RoutedUICommand* OpenCommand;

    /// Gets the value that represents the Paste command.
    /// \prop
    static const RoutedUICommand* PasteCommand;

    /// Gets the value that represents the Print command.
    /// \prop
    static const RoutedUICommand* PrintCommand;

    /// Gets the value that represents the Print Preview command.
    /// \prop
    static const RoutedUICommand* PrintPreviewCommand;

    /// Gets the value that represents the Properties command.
    /// \prop
    static const RoutedUICommand* PropertiesCommand;

    /// Gets the value that represents the Redo command.
    /// \prop
    static const RoutedUICommand* RedoCommand;

    /// Gets the value that represents the Replace command.
    /// \prop
    static const RoutedUICommand* ReplaceCommand;

    /// Gets the value that represents the Save command.
    /// \prop
    static const RoutedUICommand* SaveCommand;

    /// Gets the value that represents the SaveAs command.
    /// \prop
    static const RoutedUICommand* SaveAsCommand;

    /// Gets the value that represents the Select All command.
    /// \prop
    static const RoutedUICommand* SelectAllCommand;

    /// Gets the value that represents the Stop command.
    /// \prop
    static const RoutedUICommand* StopCommand;

    /// Gets the value that represents the Undo command.
    /// \prop
    static const RoutedUICommand* UndoCommand;

    NS_DECLARE_REFLECTION(ApplicationCommands, NoParent)
};

}

#endif
