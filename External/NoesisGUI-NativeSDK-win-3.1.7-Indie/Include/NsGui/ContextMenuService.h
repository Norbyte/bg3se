////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CONTEXTMENUSERVICE_H__
#define __GUI_CONTEXTMENUSERVICE_H__


#include <NsCore/Noesis.h>
#include <NsCore/Ptr.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class BaseComponent;
struct Rect;
class DependencyObject;
class DependencyProperty;
class RoutedEvent;
class UIElement;
class ContextMenu;
struct EventArgs;
enum PlacementMode: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides the system implementation for displaying a ContextMenu.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.controls.contextmenuservice.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API ContextMenuService
{
    /// Dependency properties
    //@{
    static const DependencyProperty* ContextMenuProperty;
    static const DependencyProperty* HasDropShadowProperty;
    static const DependencyProperty* HorizontalOffsetProperty;
    static const DependencyProperty* IsEnabledProperty;
    static const DependencyProperty* PlacementProperty;
    static const DependencyProperty* PlacementRectangleProperty;
    static const DependencyProperty* PlacementTargetProperty;
    static const DependencyProperty* ShowOnDisabledProperty;
    static const DependencyProperty* VerticalOffsetProperty;
    //@}

    /// Routed events
    //@{
    static const RoutedEvent* ContextMenuClosingEvent;
    static const RoutedEvent* ContextMenuOpeningEvent;
    //@}

    /// Gets the value of the ContextMenu property of the specified object
    static ContextMenu* GetContextMenu(DependencyObject* obj);

    /// Sets the value of the ContextMenu property of the specified object
    static void SetContextMenu(DependencyObject* obj, ContextMenu* contextMenu);

    /// Gets a value that indicates whether the context menu appears with a dropped shadow
    static bool GetHasDropShadow(DependencyObject* obj);

    /// Sets a value that indicates whether the context menu appears with a dropped shadow
    static void SetHasDropShadow(DependencyObject* obj, bool hasDropShadow);

    /// Gets the horizontal distance between the target origin and the popup alignment point
    static float GetHorizontalOffset(DependencyObject* obj);

    /// Sets the horizontal distance between the target origin and the popup alignment point
    static void SetHorizontalOffset(DependencyObject* obj, float offset);

    /// Gets a value that indicates whether the ContextMenu can be shown
    static bool GetIsEnabled(DependencyObject* obj);

    /// Sets a value that indicates whether the ContextMenu can be shown
    static void SetIsEnabled(DependencyObject* obj, bool isEnabled);

    /// Gets a value that specifies the placement of the ContextMenu relative to the PlacementTarget
    /// or PlacementRectangle properties
    static PlacementMode GetPlacement(DependencyObject* obj);

    /// Sets a value that specifies the placement of the ContextMenu relative to the PlacementTarget
    /// or PlacementRectangle
    static void SetPlacement(DependencyObject* obj, PlacementMode mode);

    /// Gets the area relative to which the context menu is positioned when it opens
    static const Rect& GetPlacementRectangle(DependencyObject* obj);

    /// Sets the area relative to which the context menu is positioned when it opens
    static void SetPlacementRectangle(DependencyObject* obj, const Rect& rect);

    /// Gets the parent control relative to which the ContextMenu is positioned when it opens
    static UIElement* GetPlacementTarget(DependencyObject* obj);

    /// Sets the parent control relative to which the ContextMenu is positioned when it opens
    static void SetPlacementTarget(DependencyObject* obj, UIElement* target);

    /// Gets a value that indicates whether the ContextMenu should be shown when its parent
    /// is grayed out
    static bool GetShowOnDisabled(DependencyObject* obj);

    /// Sets a value that indicates whether the ContextMenu should be shown when its parent
    /// is grayed out
    static void SetShowOnDisabled(DependencyObject* obj, bool showOnDisable);

    /// Gets the vertical distance between the target origin and the popup alignment point
    static float GetVerticalOffset(DependencyObject* obj);

    /// Sets the vertical distance between the target origin and the popup alignment point
    static void SetVerticalOffset(DependencyObject* obj, float offset);

    NS_DECLARE_REFLECTION(ContextMenuService, NoParent)
};

}

#endif
