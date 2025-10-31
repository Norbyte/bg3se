////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_STATUSBAR_H__
#define __GUI_STATUSBAR_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ItemsControl.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that displays items and information in a horizontal bar
/// (typically at the bottom) in an application window.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.statusbar.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API StatusBar: public ItemsControl
{
public:
    StatusBar();
    ~StatusBar();

    // From ItemsControl
    //@{
    Ptr<Panel> GetDefaultItemsPanel() const override;
    //@}

public:
    static const DependencyProperty* SeparatorStyleKey;

protected:
    /// From ItemsControl
    //@{
    Ptr<DependencyObject> GetContainerForItemOverride() const override;
    bool IsItemItsOwnContainerOverride(BaseComponent* item) const override;
    void ApplyItemContainerStyle(DependencyObject* container) const override;
    //@}

    NS_DECLARE_REFLECTION(StatusBar, ItemsControl)
};

}


#endif
