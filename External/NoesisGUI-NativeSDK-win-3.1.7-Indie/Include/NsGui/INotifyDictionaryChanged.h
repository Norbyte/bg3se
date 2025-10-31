////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INOTIFYDICTIONARYCHANGED_H__
#define __GUI_INOTIFYDICTIONARYCHANGED_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsCore/ReflectionImplementEmpty.h>


namespace Noesis
{

class BaseComponent;
template<class T> class Delegate;

/// Describes the action that caused the DictionaryChanged event
enum NotifyDictionaryChangedAction
{
    /// One item was added to the dictionary
    NotifyDictionaryChangedAction_Add,

    /// One item was removed from the dictionary
    NotifyDictionaryChangedAction_Remove,

    /// One item was replaced in the dictionary
    NotifyDictionaryChangedAction_Replace,

    /// The content of the dictionary was cleared
    NotifyDictionaryChangedAction_Reset,

    /// The content of the dictionary is going to be cleared
    NotifyDictionaryChangedAction_PreReset
};

/// Provides data for the DictionaryChanged event
struct NotifyDictionaryChangedEventArgs
{
    NotifyDictionaryChangedAction action;
    const char* key;
    const BaseComponent* oldValue;
    const BaseComponent* newValue;
};

typedef Delegate<void (BaseComponent*, const NotifyDictionaryChangedEventArgs&)>
    NotifyDictionaryChangedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Notifies listeners of dynamic changes, such as when items get added and removed or the whole
/// dictionary is refreshed.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE INotifyDictionaryChanged: public Interface
{
    /// Occurs when the dictionary changes
    virtual NotifyDictionaryChangedEventHandler& DictionaryChanged() = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(INotifyDictionaryChanged, Interface)
};

}


#endif
