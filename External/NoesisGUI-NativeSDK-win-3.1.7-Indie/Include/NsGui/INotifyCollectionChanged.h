////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INOTIFYCOLLECTIONCHANGED_H__
#define __GUI_INOTIFYCOLLECTIONCHANGED_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsCore/ReflectionImplementEmpty.h>


namespace Noesis
{

class BaseComponent;
template<class T> class Delegate;

/// Describes the action that caused a CollectionChanged even
enum NotifyCollectionChangedAction: int32_t
{
    /// An item was added to the collection
    NotifyCollectionChangedAction_Add,

    /// An item was removed from the collection
    NotifyCollectionChangedAction_Remove,

    /// An item was replaced in the collection
    NotifyCollectionChangedAction_Replace,

    /// An item was moved within the collection
    NotifyCollectionChangedAction_Move,

    /// The content of the collection was cleared
    NotifyCollectionChangedAction_Reset,

    /// The content of the collection is going to be cleared
    NotifyCollectionChangedAction_PreReset
};

/// Provides data for the CollectionChanged event
struct NotifyCollectionChangedEventArgs
{
    NotifyCollectionChangedAction action;
    int oldIndex;
    int newIndex;
    BaseComponent* oldValue;
    BaseComponent* newValue;
};

typedef Delegate<void (BaseComponent*, const NotifyCollectionChangedEventArgs&)>
    NotifyCollectionChangedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Notifies listeners of dynamic changes, such as when an item is added and removed or the whole
/// list is cleared
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE INotifyCollectionChanged: public Interface
{
    /// Occurs when the collection changes
    virtual NotifyCollectionChangedEventHandler& CollectionChanged() = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(INotifyCollectionChanged, Interface)
};

}


#endif
