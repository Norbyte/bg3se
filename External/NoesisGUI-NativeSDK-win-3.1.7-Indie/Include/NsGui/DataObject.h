////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DATAOBJECT_H__
#define __GUI_DATAOBJECT_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsGui/CoreApi.h>
#include <NsGui/RoutedEvent.h>


namespace Noesis
{

class UIElement;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides an abstract base class for events associated with the DataObject class.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API DataObjectEventArgs: public RoutedEventArgs
{
    mutable bool commandCancelled;
    bool isDragDrop;

    DataObjectEventArgs(BaseComponent* source, const RoutedEvent* event, bool isDragDrop);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Arguments for the DataObject.Copying event.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API DataObjectCopyingEventArgs final: public DataObjectEventArgs
{
    Ptr<BaseComponent> dataObject;

    DataObjectCopyingEventArgs(BaseComponent* source, BaseComponent* dataObject, bool isDragDrop);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Contains arguments for the DataObject.Pasting event.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API DataObjectPastingEventArgs final: public DataObjectEventArgs
{
    Ptr<BaseComponent> sourceDataObject;
    mutable Ptr<BaseComponent> dataObject;

    DataObjectPastingEventArgs(BaseComponent* source, BaseComponent* dataObject, bool isDragDrop);
};

NS_WARNING_POP

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Noesis::Delegate<void (BaseComponent* sender, const DataObjectCopyingEventArgs& e)>
    DataObjectCopyingEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Noesis::Delegate<void (BaseComponent* sender, const DataObjectPastingEventArgs& e)>
    DataObjectPastingEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a format-independent mechanism for transferring data.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.dataobject
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API DataObject
{
    /// Adds a DataObject.Copying event handler to a specified element.
    static void AddCopyingHandler(UIElement* element, const DataObjectCopyingEventHandler& handler);

    /// Adds a DataObject.Pasting event handler to a specified element.
    static void AddPastingHandler(UIElement* element, const DataObjectPastingEventHandler& handler);

    /// Removes a DataObject.Copying event handler from a specified element.
    static void RemoveCopyingHandler(UIElement* element,
        const DataObjectCopyingEventHandler& handler);

    /// Removes a DataObject.Pasting event handler from a specified element.
    static void RemovePastingHandler(UIElement* element,
        const DataObjectPastingEventHandler& handler);

    /// Attached routed events
    //@{
    static const RoutedEvent* CopyingEvent;
    static const RoutedEvent* PastingEvent;
    //@}

    NS_DECLARE_REFLECTION(DataObject, NoParent)
};

}


#endif
