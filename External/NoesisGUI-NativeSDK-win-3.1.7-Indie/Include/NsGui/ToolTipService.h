////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TOOLTIPSERVICE_H__
#define __GUI_TOOLTIPSERVICE_H__


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
struct EventArgs;
enum PlacementMode: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a service that provides properties and events to control the display and behavior
/// of tooltips.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.tooltipservice.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API ToolTipService
{
    /// Dependency properties
    //@{
    static const DependencyProperty* BetweenShowDelayProperty;
    static const DependencyProperty* HasDropShadowProperty;
    static const DependencyProperty* HorizontalOffsetProperty;
    static const DependencyProperty* InitialShowDelayProperty;
    static const DependencyProperty* IsEnabledProperty;
    static const DependencyProperty* IsOpenProperty;
    static const DependencyProperty* PlacementProperty;
    static const DependencyProperty* PlacementRectangleProperty;
    static const DependencyProperty* PlacementTargetProperty;
    static const DependencyProperty* ShowDurationProperty;
    static const DependencyProperty* ShowOnDisabledProperty;
    static const DependencyProperty* ToolTipProperty;
    static const DependencyProperty* VerticalOffsetProperty;
    //@}

    /// Routed events
    //@{
    static const RoutedEvent* ToolTipClosingEvent;
    static const RoutedEvent* ToolTipOpeningEvent;
    //@}

    /// Gets the maximum time between the display of two tooltips where the second tooltip
    /// appears without a delay
    static int32_t GetBetweenShowDelay(const DependencyObject* obj);
    
    /// Sets the maximum time between the display of two tooltips where the second tooltip
    /// appears without a delay
    static void SetBetweenShowDelay(DependencyObject* obj, int32_t delay);

    /// Gets whether the tooltip displays a drop shadow effect
    static bool GetHasDropShadow(const DependencyObject* obj);
    
    /// Sets whether the tooltip displays a drop shadow effect
    static void SetHasDropShadow(DependencyObject* obj, bool shadow);

    /// Gets the offset from the left of the area that is specified for the tooltip by the
    /// *PlacementRectangle* and *PlacementTarget* properties
    static float GetHorizontalOffset(const DependencyObject* obj);
    
    /// Sets the offset from the left of the area that is specified for the tooltip by the
    /// *PlacementRectangle* and *PlacementTarget* properties
    static void SetHorizontalOffset(DependencyObject* obj, float offset);

    /// Gets the length of time before a tooltip opens
    static int32_t GetInitialShowDelay(const DependencyObject* obj);
    
    /// Sets the length of time before a tooltip opens
    static void SetInitialShowDelay(DependencyObject* obj, int32_t delay);

    /// Gets whether a tooltip appears
    static bool GetIsEnabled(const DependencyObject* obj);
    
    /// Sets whether a tooltip appears
    static void SetIsEnabled(DependencyObject* obj, bool isEnabled);

    /// Gets whether a tooltip is currently visible
    static bool GetIsOpen(const DependencyObject* obj);
    
    /// Sets whether a tooltip is currently visible
    static void SetIsOpen(DependencyObject* obj, bool isOpen);

    /// Gets the orientation of the tooltip when it opens, and specifies how the tooltip
    /// behaves when it overlaps screen boundaries
    static PlacementMode GetPlacement(const DependencyObject* obj);
    
    /// Sets the orientation of the tooltip when it opens, and specifies how the tooltip
    /// behaves when it overlaps screen boundaries
    static void SetPlacement(DependencyObject* obj, PlacementMode placement);

    /// Gets the rectangular area relative to which the tooltip is positioned
    static const Rect& GetPlacementRectangle(const DependencyObject* obj);
    
    /// Sets the rectangular area relative to which the tooltip is positioned
    static void SetPlacementRectangle(DependencyObject* obj, const Rect& rect);

    /// Gets the object relative to which the tooltip is positioned
    static UIElement* GetPlacementTarget(const DependencyObject* obj);
    
    /// Sets the object relative to which the tooltip is positioned
    static void SetPlacementTarget(DependencyObject* obj, UIElement* target);

    /// Gets the amount of time that a tooltip remains visible
    static int32_t GetShowDuration(const DependencyObject* obj);
    
    /// Sets the amount of time that a tooltip remains visible
    static void SetShowDuration(DependencyObject* obj, int32_t duration);

    /// Gets whether a tooltip appears for an object that is not enabled
    static bool GetShowOnDisabled(const DependencyObject* obj);
    
    /// Sets whether a tooltip appears for an object that is not enabled
    static void SetShowOnDisabled(DependencyObject* obj, bool showOnDisable);

    /// Gets the content of a tooltip
    static BaseComponent* GetToolTip(const DependencyObject* obj);
    
    /// Sets the content of a tooltip
    static void SetToolTip(DependencyObject* obj, BaseComponent* toolTip);

    /// Gets the distance from the top of the area that is specified for the tooltip by the
    /// *PlacementRectangle* and *PlacementTarget* properties
    static float GetVerticalOffset(const DependencyObject* obj);
    
    /// Sets the distance from the top of the area that is specified for the tooltip by the
    /// *PlacementRectangle* and *PlacementTarget* properties
    static void SetVerticalOffset(DependencyObject* obj, float offset);

    /// Finds the first object in the tree from the specified element that can show a tooltip
    static UIElement* FindValidToolTipOwner(UIElement* obj);

    NS_DECLARE_REFLECTION(ToolTipService, NoParent)
};

}


#endif
