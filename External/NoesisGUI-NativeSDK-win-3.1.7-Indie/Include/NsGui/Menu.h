////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MENU_H__
#define __GUI_MENU_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BaseMenu.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a Windows menu control that enables you to hierarchically organize elements 
/// associated with commands and event handlers.
///
/// http://msdn.microsoft.com/en-us/library/ms611602.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Menu: public BaseMenu
{
public:
    Menu();

    /// Gets or sets a value that indicates whether this Menu receives a main menu activation 
    /// notification
    //@{
    bool GetIsMainMenu() const;
    void SetIsMainMenu(bool value);
    //@}

    /// Gets or sets a value that indicates whether this Menu is activated and sub menu items
    /// get opened when mouse moves over them
    //@{
    bool GetActivated() const;
    void SetActivated(bool activated);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* IsMainMenuProperty;
    //@}

protected:
    // From UIElement
    //@{
    void OnMouseWheel(const MouseWheelEventArgs& e) override;
    //@}

private:
    void CloseSubmenus();

    void MouseCapture();
    void MouseRelease();

private:
    bool mIsActivated;

    NS_DECLARE_REFLECTION(Menu, BaseMenu)
};

}


#endif
