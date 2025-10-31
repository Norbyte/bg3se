////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VIRTUALIZINGPANEL_H__
#define __GUI_VIRTUALIZINGPANEL_H__


#include <NsCore/Noesis.h>
#include <NsGui/Panel.h>
#include <NsGui/VirtualizationCacheLength.h>
#include <NsCore/ReflectionDeclareEnum.h>


namespace Noesis
{

NS_INTERFACE IItemContainerGenerator;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies the virtualization mode of a VirtualizingPanel.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum VirtualizationMode
{
    /// Standard virtualization mode: containers are thrown away when offscreen
    VirtualizationMode_Standard,
    /// Recycling virtualization mode: containers are re-used when offscreen
    VirtualizationMode_Recycling
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies the scroll unit of the VirtualizingPanel.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum ScrollUnit
{
    ScrollUnit_Pixel,
    ScrollUnit_Item
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class that provides access to information that is useful for implementing virtualization.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.virtualizingpanel.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API VirtualizingPanel: public Panel
{
public:
    VirtualizingPanel();
    virtual ~VirtualizingPanel() = 0;

    /// Gets the size of the cache before and after the viewport when is virtualizing
    static const VirtualizationCacheLength& GetCacheLength(const DependencyObject* element);
    /// Sets the size of the cache before and after the viewport when is virtualizing
    static void SetCacheLength(DependencyObject* element, const VirtualizationCacheLength& len);

    /// Gets the type of unit that is used by the CacheLength property
    static VirtualizationCacheLengthUnit GetCacheLengthUnit(const DependencyObject* element);
    /// Sets the type of unit that is used by the CacheLength property
    static void SetCacheLengthUnit(DependencyObject* element, VirtualizationCacheLengthUnit unit);

    /// Gets a value that indicates whether this VirtualizingPanel should virtualize an item
    static bool GetIsContainerVirtualizable(const DependencyObject* element);
    /// Sets a value that indicates whether this VirtualizingPanel should virtualize an item.
    static void SetIsContainerVirtualizable(DependencyObject* element, bool isVirtualizable);

    /// Gets a value that indicates if virtualization is turned on or off in the ItemsControl
    static bool GetIsVirtualizing(const DependencyObject* element);
    /// Gets a value that indicates if virtualization is turned on or off in the ItemsControl
    static void SetIsVirtualizing(DependencyObject* element, bool isVirtualizing);

    /// Gets a value that indicates if scrolling is measured as items or as pixels
    static ScrollUnit GetScrollUnit(const DependencyObject* element);
    /// Sets a value that indicates if scrolling is measured as items or as pixels
    static void SetScrollUnit(DependencyObject* element, ScrollUnit unit);

    /// Gets the virtualization mode in the ItemsControl
    static VirtualizationMode GetVirtualizationMode(const DependencyObject* element);
    /// Sets the virtualization mode in the ItemsControl
    static void SetVirtualizationMode(DependencyObject* element, VirtualizationMode mode);

    /// The generator associated with this panel
    IItemContainerGenerator* GetItemContainerGenerator() const;

    /// Generates the item at the specified index and calls BringIntoView on it
    void BringIndexIntoView(int32_t index);

public:
    static const DependencyProperty* CacheLengthProperty; // attached property
    static const DependencyProperty* CacheLengthUnitProperty; // attached property
    static const DependencyProperty* IsContainerVirtualizableProperty; // attached property
    static const DependencyProperty* IsVirtualizingProperty; // attached property
    static const DependencyProperty* ScrollUnitProperty; // attached property
    static const DependencyProperty* VirtualizationModeProperty; // attached property

protected:
    virtual void BringIndexIntoViewOverride(int32_t index);

    // From Panel
    //@{
    void GenerateChildren() override;
    void OnItemsChangedOverride(BaseComponent* sender, const ItemsChangedEventArgs& e) override;
    //@}

    NS_DECLARE_REFLECTION(VirtualizingPanel, Panel)
};

NS_WARNING_POP

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::VirtualizationMode)
NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::ScrollUnit)


#endif
