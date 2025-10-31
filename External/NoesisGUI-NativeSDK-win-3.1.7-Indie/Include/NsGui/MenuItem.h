////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MENUITEM_H__
#define __GUI_MENUITEM_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsGui/CoreApi.h>
#include <NsGui/HeaderedItemsControl.h>
#include <NsGui/ICommandSource.h>


namespace Noesis
{

class PopupLayer;
class ItemCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a selectable item inside a Menu or ContextMenu.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.menuitem.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API MenuItem: public HeaderedItemsControl, public ICommandSource
{
public:
    MenuItem();
    ~MenuItem();

    /// Gets or sets the parameter to pass to the Command property of a MenuItem
    //@{
    BaseComponent* GetCommandParameter() const override;
    void SetCommandParameter(BaseComponent* parameter);
    //@}

    /// Gets or sets the command associated with the menu item
    //@{
    ICommand* GetCommand() const override;
    void SetCommand(ICommand* command);
    //@}

    /// Gets or sets the target element on which to raise the specified command
    //@{
    UIElement* GetCommandTarget() const override;
    void SetCommandTarget(UIElement* target);
    //@}

    /// Gets or sets the icon that appears in a MenuItem
    //@{
    BaseComponent* GetIcon() const;
    void SetIcon(BaseComponent* icon);
    //@}

    /// Sets the text describing an input gesture that will call the command tied to the specified 
    /// item
    //@{
    const char* GetInputGestureText() const;
    void SetInputGestureText(const char* text);
    //@}

    /// Gets a value that indicates whether a MenuItem can be checked
    //@{
    bool GetIsCheckable() const;
    void SetIsCheckable(bool value);
    //@}

    /// Gets or sets a value that indicates whether the MenuItem is checked
    //@{
    bool GetIsChecked() const;
    void SetIsChecked(bool value);
    //@}

    /// Gets a value that indicates whether a MenuItem is highlighted
    bool GetIsHighlighted() const;

    /// Gets a value that indicates whether a MenuItem is pressed
    bool GetIsPressed() const;

    /// Gets or sets a value that indicates whether the submenu of the MenuItem is open
    //@{
    bool GetIsSubmenuOpen() const;
    void SetIsSubmenuOpen(bool value);
    //@}

    /// Gets a value that indicates the role of a MenuItem
    MenuItemRole GetRole() const;

    /// Gets or sets a value that indicates that the submenu in which this MenuItem is located 
    /// should not close when this item is clicked
    //@{
    bool GetStaysOpenOnClick() const;
    void SetStaysOpenOnClick(bool value);
    //@}

    /// Occurs when a menu item is checked
    UIElement::RoutedEvent_<RoutedEventHandler> Checked();

    /// Occurs when a MenuItem is clicked
    UIElement::RoutedEvent_<RoutedEventHandler> Click();

    /// Occurs when the state of the IsSubmenuOpen property changes to false
    UIElement::RoutedEvent_<RoutedEventHandler> SubmenuClosed();

    /// Occurs when the state of the IsSubmenuOpen property changes to true
    UIElement::RoutedEvent_<RoutedEventHandler> SubmenuOpened();

    /// Occurs when a MenuItem is unchecked
    UIElement::RoutedEvent_<RoutedEventHandler> Unchecked();

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    static const DependencyProperty* CommandParameterProperty;
    static const DependencyProperty* CommandProperty;
    static const DependencyProperty* CommandTargetProperty;
    static const DependencyProperty* IconProperty;
    static const DependencyProperty* InputGestureTextProperty;
    static const DependencyProperty* IsCheckableProperty;
    static const DependencyProperty* IsCheckedProperty;
    static const DependencyProperty* IsHighlightedProperty;
    static const DependencyProperty* IsPressedProperty;
    static const DependencyProperty* IsSubmenuOpenProperty;
    static const DependencyProperty* RoleProperty;
    static const DependencyProperty* StaysOpenOnClickProperty;

    static const DependencyProperty* SeparatorStyleKey;

    static const RoutedEvent* CheckedEvent;
    static const RoutedEvent* ClickEvent;
    static const RoutedEvent* SubmenuClosedEvent;
    static const RoutedEvent* SubmenuOpenedEvent;
    static const RoutedEvent* UncheckedEvent;

protected:
    /// From DependencyObject
    //@{
    void OnInit() override;
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From Visual
    //@{
    void OnDisconnectFromView() override;
    //@}

    /// From UIElement
    //@{
    bool IsEnabledCore() const override;
    void OnMouseEnter(const MouseEventArgs& e) override;
    void OnMouseLeave(const MouseEventArgs& e) override;
    void OnMouseMove(const MouseEventArgs& e) override;
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e) override;
    void OnMouseRightButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseRightButtonUp(const MouseButtonEventArgs& e) override;
    void OnGotKeyboardFocus(const KeyboardFocusChangedEventArgs& e) override;
    void OnLostKeyboardFocus(const KeyboardFocusChangedEventArgs& e) override;
    void OnKeyDown(const KeyEventArgs& e) override;
    //@}

    /// From FrameworkElement
    //@{
    void OnLogicalParentChanged(FrameworkElement* oldParent) override;
    //@}

    /// From ItemsControl
    //@{
    Ptr<DependencyObject> GetContainerForItemOverride() const override;
    bool IsItemItsOwnContainerOverride(BaseComponent* item) const override;
    void ApplyItemContainerStyle(DependencyObject* container) const override;
    void OnItemsChanged(const NotifyCollectionChangedEventArgs& args) override;
    //@}

private:
    friend class ContextMenu;
    static void SetIsInsideContextMenu(DependencyObject* element, bool value);
    bool GetIsInsideContextMenu() const;

    bool IsSeparator() const;

    void OnIsCheckedChanged(bool isChecked);
    void OnIsSubmenuOpenChanged(bool isOpen);

    void OnHeaderClicked();
    void OnItemClicked();

    void OnDirectlyOverChanged(UIElement* directlyOver);

    void OnMouseButtonDown(const MouseButtonEventArgs& e, bool clickAllowed);
    void OnMouseButtonUp(const MouseButtonEventArgs& e, bool clickAllowed);

    void UpdateIsPressed();

    void UpdateSubmenuVisibility(bool visible, bool focus = false);
    void UpdateRole();

    void ShowSubmenu();
    void HideSubmenu();

    static void UpdateCommandCanExecute(ItemCollection* items);

    void ActivateMenu(bool activate);
    void NavigateMenu(FocusNavigationDirection direction);
    void NavigateSubmenu(FocusNavigationDirection direction);

    void CloseAncestors();
    void CloseDescendants();

    void RegisterMouse();
    void UnregisterMouse();

    void RegisterCommand(ICommand* command);
    void UnregisterCommand(ICommand* command);
    void UpdateCanExecute(ICommand* command);
    void OnCanExecuteChanged(BaseComponent* sender, const EventArgs& e);

    friend class Popup;
    void SetSubmenuLayer(PopupLayer* submenuLayer);

private:
    static const DependencyProperty* IsInsideContextMenuProperty;

    PopupLayer* mSubmenuLayer;

    union
    {
        uint8_t mAllFlags;
        struct
        {
            bool ownClose:1;
            bool canExecute:1;
            bool isMouseRegistered:1;
        } mFlags;
    };

    NS_DECLARE_REFLECTION(MenuItem, HeaderedItemsControl)
};

NS_WARNING_POP

}


#endif
