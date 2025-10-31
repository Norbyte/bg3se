////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASEVIEW_H__
#define __GUI_BASEVIEW_H__


#include <NsCore/Noesis.h>
#include <NsGui/ControlsApi.h>
#include <NsGui/Animatable.h>


namespace Noesis
{

class ListViewItem;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the base class for views that define the appearance of data in a ListView control.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.viewbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CONTROLS_API BaseView: public Animatable
{
public:
    BaseView();
    virtual ~BaseView() = 0;

    /// Removes all bindings and styling that are set for an item
    virtual void ClearItem(ListViewItem* item);

    /// Prepares an item in the view for display, by setting bindings and styles
    virtual void PrepareItem(ListViewItem* item);

    NS_DECLARE_REFLECTION(BaseView, Animatable)
};

NS_WARNING_POP

}


#endif
