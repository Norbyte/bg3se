////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TOOLBARTRAY_H__
#define __GUI_TOOLBARTRAY_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

class Brush;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the container that handles the layout of a ToolBar.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.toolbartray.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ToolBarTray: public FrameworkElement
{
public:
    ToolBarTray();
    ~ToolBarTray();

    /// Gets a value that indicates whether a ToolBar can be moved inside a ToolBarTray
    static bool GetIsLocked(const DependencyObject* element);

    /// Sets a value that indicates whether a ToolBar can be moved inside a ToolBarTray
    static void SetIsLocked(DependencyObject* element, bool isLocked);

    /// Gets or sets a brush to use for the background color of the ToolBarTray
    //@{
    Brush* GetBackground() const;
    void SetBackground(Brush* brush);
    //@}

    /// Specifies the orientation of a ToolBarTray
    //@{
    Orientation GetOrientation() const;
    void SetOrientation(Orientation orientation);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* BackgroundProperty;
    static const DependencyProperty* IsLockedProperty; // Attached property
    static const DependencyProperty* OrientationProperty;
    //@}

    NS_DECLARE_REFLECTION(ToolBarTray, FrameworkElement)
};

}


#endif
