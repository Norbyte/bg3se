////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASEBUTTON_H__
#define __GUI_BASEBUTTON_H__


#include <NsCore/Noesis.h>
#include <NsGui/ContentControl.h>
#include <NsGui/ICommandSource.h>


namespace Noesis
{

NS_INTERFACE ICommand;
enum ClickMode: int32_t;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the base class for all button controls.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.buttonbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseButton: public ContentControl, public ICommandSource
{
public:
    BaseButton();
    BaseButton(const BaseButton&) = delete;
    BaseButton& operator=(const BaseButton&) = delete;
    virtual ~BaseButton() = 0;

    /// Gets or sets when the Click event occurs
    //@{
    ClickMode GetClickMode() const;
    void SetClickMode(ClickMode mode);
    //@}

    /// Gets or sets the command to invoke when this button is pressed
    //@{
    ICommand* GetCommand() const override;
    void SetCommand(ICommand* command);
    //@}

    /// Gets or sets the parameter to pass to the Command property
    //@{
    BaseComponent* GetCommandParameter() const override;
    void SetCommandParameter(BaseComponent* param);
    //@}

    /// Gets or sets the element on which to raise the specified command
    //@{
    UIElement* GetCommandTarget() const override;
    void SetCommandTarget(UIElement* target);
    //@}

    /// Gets a value that indicates whether a button is currently activated
    bool GetIsPressed() const;

    /// Occurs when a Button is clicked
    UIElement::RoutedEvent_<RoutedEventHandler> Click();

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ClickModeProperty;
    static const DependencyProperty* CommandProperty;
    static const DependencyProperty* CommandParameterProperty;
    static const DependencyProperty* CommandTargetProperty;
    static const DependencyProperty* IsPressedProperty;
    //@}

    /// Routed events
    //@{
    static const RoutedEvent* ClickEvent;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From UIElement
    //@{
    bool IsEnabledCore() const override;
    void OnKeyDown(const KeyEventArgs& e) override;
    void OnKeyUp(const KeyEventArgs& e) override;
    void OnLostKeyboardFocus(const KeyboardFocusChangedEventArgs& e) override;
    void OnLostMouseCapture(const MouseEventArgs& e) override;
    void OnMouseEnter(const MouseEventArgs& e) override;
    void OnMouseLeave(const MouseEventArgs& e) override;
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e) override;
    void OnMouseMove(const MouseEventArgs& e) override;
    //@}
    
    /// From Control
    //@{
    void UpdateVisualStates() override;
    //@}

    /// Event handlers
    //@{
    virtual void OnClick();
    virtual void OnIsPressedChanged(const DependencyPropertyChangedEventArgs& e);
    //@}

private:
    void RegisterCommand(ICommand* command);
    void UnregisterCommand(ICommand* command);
    void UpdateCanExecute(ICommand* command);
    void OnCanExecuteChanged(BaseComponent* sender, const EventArgs& e);

private:
    union
    {
        uint8_t mAllFlags;
        struct
        {
            bool isSpaceKeyPressed:1;
            bool isMouseLeftButtonPressed:1;
            bool canExecute:1;
        } mFlags;
    };

    NS_DECLARE_REFLECTION(BaseButton, ContentControl)
};

NS_WARNING_POP

}

#endif
