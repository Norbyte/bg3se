////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DRAGDROP_H__
#define __GUI_DRAGDROP_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

template<class T> class Delegate;
class RoutedEvent;
class BaseComponent;
class DependencyObject;
class UIElement;
struct Point;

typedef Delegate<void (DependencyObject* source, BaseComponent* data, UIElement* target,
    const Point& dropPoint, uint32_t effects)> DragDropCompletedCallback;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides helper methods and fields for initiating drag-and-drop operations.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.dragdrop.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API DragDrop
{
    /// Initiates a drag-and-drop operation
    static void DoDragDrop(DependencyObject* source, BaseComponent* data, uint32_t allowedEffects);
    static void DoDragDrop(DependencyObject* source, BaseComponent* data, uint32_t allowedEffects,
        const DragDropCompletedCallback& completedCallback);

    /// Identifies the *PreviewQueryContinueDrag* attached event
    /// \prop
    static const RoutedEvent* PreviewQueryContinueDragEvent;

    /// Identifies the *QueryContinueDrag* attached event
    /// \prop
    static const RoutedEvent* QueryContinueDragEvent;

    /// Identifies the *PreviewGiveFeedback* attached event
    /// \prop
    static const RoutedEvent* PreviewGiveFeedbackEvent;

    /// Identifies the *GiveFeedback* attached event
    /// \prop
    static const RoutedEvent* GiveFeedbackEvent;

    /// Identifies the *PreviewDragEnter* attached event
    /// \prop
    static const RoutedEvent* PreviewDragEnterEvent;

    /// Identifies the *DragEnter* attached event
    /// \prop
    static const RoutedEvent* DragEnterEvent;

    /// Identifies the *PreviewDragOver* attached event
    /// \prop
    static const RoutedEvent* PreviewDragOverEvent;

    /// Identifies the *DragOver* attached event
    /// \prop
    static const RoutedEvent* DragOverEvent;

    /// Identifies the *PreviewDragLeave* attached event
    /// \prop
    static const RoutedEvent* PreviewDragLeaveEvent;

    /// Identifies the *DragLeave* attached event
    /// \prop
    static const RoutedEvent* DragLeaveEvent;

    /// Identifies the *PreviewDrop* attached event
    /// \prop
    static const RoutedEvent* PreviewDropEvent;

    /// Identifies the *Drop* attached event
    /// \prop
    static const RoutedEvent* DropEvent;

    NS_DECLARE_REFLECTION(DragDrop, NoParent);
};

}

#endif
