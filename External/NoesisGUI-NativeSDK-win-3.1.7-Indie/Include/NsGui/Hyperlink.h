////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_HYPERLINK_H__
#define __GUI_HYPERLINK_H__


#include <NsCore/Noesis.h>
#include <NsGui/Span.h>
#include <NsGui/ICommandSource.h>
#include <NsGui/RoutedEvent.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
struct RequestNavigateEventArgs: public RoutedEventArgs
{
    const char* uri;
    const char* target;

    RequestNavigateEventArgs(BaseComponent* source, const RoutedEvent* event, const char* uri,
        const char* target);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Delegate<void (BaseComponent*, const RequestNavigateEventArgs&)> RequestNavigateEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// An inline-level flow content element that hosts hyperlinks within flow content.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.documents.hyperlink.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Hyperlink: public Span, public ICommandSource
{
public:
    Hyperlink();
    Hyperlink(Inline* childInline);

    /// Gets or sets a command to associate with the Hyperlink
    //@{
    ICommand* GetCommand() const override;
    void SetCommand(ICommand* command);
    //@}

    /// Gets or sets command parameters associated with the command specified
    //@{
    BaseComponent* GetCommandParameter() const override;
    void SetCommandParameter(BaseComponent* param);
    //@}

    /// Gets or sets a target element on which to execute the command specified
    //@{
    UIElement* GetCommandTarget() const override;
    void SetCommandTarget(UIElement* target);
    //@}

    /// Gets or sets a Uri to navigate to when the Hyperlink is activated
    //@{
    const char* GetNavigateUri() const;
    void SetNavigateUri(const char* navigateUri);
    //@}

    /// Gets or sets the name of a target window or frame for the Hyperlink
    //@{
    const char* GetTargetName() const;
    void SetTargetName(const char* targetName);
    //@}

    /// Occurs when the left mouse button is clicked on a Hyperlink
    RoutedEvent_<RoutedEventHandler> Click();
    /// Occurs when navigation events are requested
    RoutedEvent_<RequestNavigateEventHandler> RequestNavigate();

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    static const DependencyProperty* CommandProperty;
    static const DependencyProperty* CommandParameterProperty;
    static const DependencyProperty* CommandTargetProperty;
    static const DependencyProperty* NavigateUriProperty;
    static const DependencyProperty* TargetNameProperty;

    static const RoutedEvent* ClickEvent;
    static const RoutedEvent* RequestNavigateEvent;

protected:
    virtual void OnClick();

    // From UIElement
    //@{
    void OnKeyDown(const KeyEventArgs& e) override;
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e) override;
    //@}

private:
    void NavigateToUri(const char* navigateUri, const char* targetName);

private:
    static const DependencyProperty* IsHyperlinkPressedProperty;

    NS_DECLARE_REFLECTION(Hyperlink, Span)
};

NS_WARNING_POP

}

#include <NsGui/Hyperlink.inl>

#endif
