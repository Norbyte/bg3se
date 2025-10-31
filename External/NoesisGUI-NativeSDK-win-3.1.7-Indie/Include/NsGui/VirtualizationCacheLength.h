////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VIRTUALIZATIONCACHELENGTH_H__
#define __GUI_VIRTUALIZATIONCACHELENGTH_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsCore/StringFwd.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
enum VirtualizationCacheLengthUnit
{
    /// The value is expressed as a pixel
    VirtualizationCacheLengthUnit_Pixel,
    /// The value is expressed as an item
    VirtualizationCacheLengthUnit_Item,
    /// The value is expressed as a page full of items
    VirtualizationCacheLengthUnit_Page
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the measurements for the *VirtualizingPanel.CacheLength* attached property.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.virtualizationcachelength.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API VirtualizationCacheLength
{
public:
    VirtualizationCacheLength();
    VirtualizationCacheLength(float cacheBeforeAndAfterViewport);
    VirtualizationCacheLength(float cacheBeforeViewport, float cacheAfterViewport);

    /// Gets the size of the cache before the viewport when the VirtualizingPanel is virtualizing
    /// \prop
    float GetCacheBeforeViewport() const;

    /// Gets the size of the cache after the viewport when the VirtualizingPanel is virtualizing
    /// \prop
    float GetCacheAfterViewport() const;

    // Copy operators
    //@{
    VirtualizationCacheLength(const VirtualizationCacheLength& other);
    VirtualizationCacheLength& operator=(const VirtualizationCacheLength& other);
    //@}

    // Comparison operators
    //@{
    bool operator==(const VirtualizationCacheLength& other) const;
    bool operator!=(const VirtualizationCacheLength& other) const;
    //@}

    // Creates a string representation of this VirtualizationCacheLength structure
    // The string has the following form: "beforeViewportLength,afterViewportLength"
    String ToString() const;

    // Converts a string into the equivalent VirtualizationCacheLength structure
    static bool TryParse(const char* str, VirtualizationCacheLength& result);

private:
    float mCacheBeforeViewport;
    float mCacheAfterViewport;

    NS_DECLARE_REFLECTION(VirtualizationCacheLength, NoParent)
};

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::VirtualizationCacheLengthUnit)


#endif
