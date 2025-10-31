////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_USERCONTROL_H__
#define __GUI_USERCONTROL_H__


#include <NsCore/Noesis.h>
#include <NsGui/ContentControl.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a simple way to create a composition of controls.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.forms.usercontrol.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API UserControl: public ContentControl
{
protected:
    /// From FrameworkElement
    //@{
    FrameworkElement* GetStateGroupsRoot() const override;
    //@}

    NS_DECLARE_REFLECTION(UserControl, ContentControl)
};

}


#endif
