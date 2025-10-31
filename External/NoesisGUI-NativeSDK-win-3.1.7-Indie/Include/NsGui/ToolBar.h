////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TOOLBAR_H__
#define __GUI_TOOLBAR_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/HeaderedItemsControl.h>


namespace Noesis
{

class ToolBarPanel;
class ToolBarOverflowPanel;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a container for a group of commands or controls. 
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.toolbar.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ToolBar: public HeaderedItemsControl
{
public:
    ToolBar();
    ~ToolBar();

    /// Gets a value that indicates when an item should be placed in the overflow panel instead of
    /// in the main panel
    static OverflowMode GetOverflowMode(const DependencyObject* element);

    /// Sets a value that indicates when an item should be placed in the overflow panel instead of
    /// in the main panel
    static void SetOverflowMode(DependencyObject* element, OverflowMode mode);

    /// Gets a value that indicates whether the ToolBar item is an overflow item
    static bool GetIsOverflowItem(const DependencyObject* element);

    // Sets a value that indicates whether the ToolBar item is an overflow item
    // Note: Property is read-only, this is an internal function
    static void SetIsOverflowItem(DependencyObject* element, bool isOverflowItem);

    /// Gets or sets the band index number that indicates the position of the ToolBar on the band
    //@{
    int32_t GetBandIndex() const;
    void SetBandIndex(int32_t value);
    //@}

    /// Gets or sets a value that indicates where the ToolBar should be located in the ToolBarTray
    //@{
    int32_t GetBand() const;
    void SetBand(int32_t value);
    //@}

    /// Gets a value that indicates whether the ToolBar has items that are not visible
    bool GetHasOverflowItems() const;

    /// Gets or sets a value that indicates whether the ToolBar overflow area is currently visible
    //@{
    bool GetIsOverflowOpen() const;
    void SetIsOverflowOpen(bool value);
    //@}

    /// Gets the orientation of the ToolBar
    Orientation GetOrientation() const;

    // Gets ToolBarPanel used to place this ToolBar's items
    ToolBarPanel* GetPanel() const;

    // Gets ToolBarPanel used to place this ToolBar's overflow items
    ToolBarOverflowPanel* GetOverflowPanel() const;

    // Updates *HasOverflowItems* property
    void UpdateHasOverflowItems(bool hasOverflowItems);

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* BandIndexProperty;
    static const DependencyProperty* BandProperty;
    static const DependencyProperty* HasOverflowItemsProperty;
    static const DependencyProperty* IsOverflowItemProperty; // Attached property
    static const DependencyProperty* IsOverflowOpenProperty;
    static const DependencyProperty* OrientationProperty;
    static const DependencyProperty* OverflowModeProperty; // Attached property
    //@}

    /// TODO: Convert to static values instead of dependency properties
    //@{
    static const DependencyProperty* ButtonStyleKey;
    static const DependencyProperty* ToggleButtonStyleKey;
    static const DependencyProperty* CheckBoxStyleKey;
    static const DependencyProperty* RadioButtonStyleKey;
    static const DependencyProperty* TextBoxStyleKey;
    static const DependencyProperty* ComboBoxStyleKey;
    static const DependencyProperty* SeparatorStyleKey;
    static const DependencyProperty* MenuStyleKey;
    //@}

protected:
    /// From Control
    //@{
    void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot);
    //@}

    /// From ItemsControl
    //@{
    bool IsItemItsOwnContainerOverride(BaseComponent* item) const;
    void ApplyItemContainerStyle(DependencyObject* container) const;
    //@}

private:
    Ptr<ToolBarPanel> mPanelInternal;
    Ptr<ToolBarOverflowPanel> mOverflowPanelInternal;

    NS_DECLARE_REFLECTION(ToolBar, HeaderedItemsControl)
};

NS_WARNING_POP

}


#endif
