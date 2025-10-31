////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EXPANDER_H__
#define __GUI_EXPANDER_H__


#include <NsCore/Noesis.h>
#include <NsGui/HeaderedContentControl.h>
#include <NsCore/ReflectionDeclareEnum.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that displays a header with a collapsible window to display content.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.expander.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Expander: public HeaderedContentControl
{
public:
    Expander();

    /// Gets or sets the direction in which the Expander content window opens
    //@{
    ExpandDirection GetExpandDirection() const;
    void SetExpandDirection(ExpandDirection direction);
    //@}

    /// Gets or sets whether the Expander content window is visible
    //@{
    bool GetIsExpanded() const;
    void SetIsExpanded(bool isExpanded);
    //@}

    /// Occurs when the content window of an Expander control closes and only the Header is visible
    UIElement::RoutedEvent_<RoutedEventHandler> Collapsed();

    /// Occurs when the content window of an Expander control opens to display both its header
    /// and content
    UIElement::RoutedEvent_<RoutedEventHandler> Expanded();

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ExpandDirectionProperty;
    static const DependencyProperty* IsExpandedProperty;
    //@}

    /// Routed events
    //@{
    static const RoutedEvent* CollapsedEvent;
    static const RoutedEvent* ExpandedEvent;
    //@}

protected:
    virtual void OnCollapsed();
    virtual void OnExpanded();

    /// From Control
    //@{
    void UpdateVisualStates() override;
    //@}

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    NS_DECLARE_REFLECTION(Expander, HeaderedContentControl)
};

}


#endif
