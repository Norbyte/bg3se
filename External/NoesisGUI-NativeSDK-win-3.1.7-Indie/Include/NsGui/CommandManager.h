////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMMANDMANAGER_H__
#define __GUI_COMMANDMANAGER_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

template<class T> class Delegate;
NS_INTERFACE ICommand;
class BaseComponent;
struct InputEventArgs;
class RoutedEvent;
struct EventArgs;
class UIElement;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides command related events and is responsible for managing routed commands execution.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API CommandManager
{
    /// Attached events
    //@{
    static const RoutedEvent* PreviewCanExecuteEvent;
    static const RoutedEvent* CanExecuteEvent;
    static const RoutedEvent* PreviewExecutedEvent;
    static const RoutedEvent* ExecutedEvent;
    //@}

    // Process input events (for internal use only)
    static void ProcessInput(UIElement* target, const InputEventArgs& args);

    NS_DECLARE_REFLECTION(CommandManager, NoParent)
};

}

#endif
