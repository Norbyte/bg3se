////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASETEXTBOX_H__
#define __GUI_BASETEXTBOX_H__


#include <NsCore/Noesis.h>
#include <NsGui/Control.h>


namespace Noesis
{

class Brush;
class ScrollViewer;
struct CanExecuteRoutedEventArgs;
struct ExecutedRoutedEventArgs;
enum PanningMode: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// An abstract base class that provides functionality for text editing controls.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.textboxbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseTextBox: public Control
{
public:
    BaseTextBox();
    virtual ~BaseTextBox() = 0;

    /// Gets or sets a value that indicates how the text editing control responds when the user
    /// presses the *ENTER* key.
    //@{
    bool GetAcceptsReturn() const;
    void SetAcceptsReturn(bool value);
    //@}

    /// Gets or sets a value that indicates how the text editing control responds when the user
    /// presses the *TAB* key.
    //@{
    bool GetAcceptsTab() const;
    void SetAcceptsTab(bool value);
    //@}

    /// Gets or sets the brush that is used to paint the caret of the text box
    //@{
    Brush* GetCaretBrush() const;
    void SetCaretBrush(Brush* brush);
    //@}

    /// Gets or sets a value that indicates whether a horizontal scroll bar is shown
    //@{
    ScrollBarVisibility GetHorizontalScrollBarVisibility() const;
    void SetHorizontalScrollBarVisibility(ScrollBarVisibility value);
    //@}

    /// Gets or sets a value that indicates whether the text editing control is read-only to a user 
    /// interacting with the control
    //@{
    bool GetIsReadOnly() const;
    void SetIsReadOnly(bool value);
    //@}

    /// Gets a value that indicates whether the text box has focus and selected text
    bool GetIsSelectionActive() const;

    /// Gets or sets the way TextBox reacts to touch manipulation
    //@{
    PanningMode GetPanningMode() const;
    void SetPanningMode(PanningMode panningMode);
    //@}

    /// Gets or sets the brush that highlights selected text
    //@{
    Brush* GetSelectionBrush() const;
    void SetSelectionBrush(Brush* selectionBrush);
    //@}

    /// Gets or sets the opacity of the *SelectionBrush*. If *SelectionOpacity* is set to 0, the
    /// *SelectionBrush* is transparent and is not visible. If *SelectionOpacity* is set to 1.0 or
    /// greater, the *SelectionBrush* is opaque and the selected text is not visible
    //@{
    float GetSelectionOpacity() const;
    void SetSelectionOpacity(float selectionOpacity);
    //@}

    /// Gets or sets a value that indicates whether a vertical scroll bar is shown
    //@{
    ScrollBarVisibility GetVerticalScrollBarVisibility() const;
    void SetVerticalScrollBarVisibility(ScrollBarVisibility value);
    //@}

    /// Horizontal size of scrollable content; 0.0 if TextBox is non-scrolling
    /// \prop
    float GetExtentWidth() const;

    /// Vertical size of scrollable content; 0.0 if TextBox is non-scrolling
    /// \prop
    float GetExtentHeight() const;

    /// Horizontal size of scroll area; 0.0 if TextBox is non-scrolling
    /// \prop
    float GetViewportWidth() const;

    /// Vertical size of scroll area; 0.0 if TextBox is non-scrolling
    /// \prop
    float GetViewportHeight() const;

    /// Actual HorizontalOffset contains the ScrollViewer's current horizontal offset.
    /// This is a computed value, depending on the state of ScrollViewer, its Viewport, Extent
    /// and previous scrolling commands.
    /// \prop
    float GetHorizontalOffset() const;

    /// Actual VerticalOffset contains the ScrollViewer's current vertical offset.
    /// This is a computed value, depending on the state of ScrollViewer, its Viewport, Extent
    /// and previous scrolling commands.
    /// \prop
    float GetVerticalOffset() const;

    /// Scroll content by one line to the left.
    void LineLeft();

    /// Scroll content by one line to the right.
    void LineRight();

    /// Scroll content by one page to the left.
    void PageLeft();

    /// Scroll content by one page to the right.
    void PageRight();

    /// Scroll content by one line to the top.
    void LineUp();

    /// Scroll content by one line to the bottom.
    void LineDown();

    /// Scroll content by one page to the top.
    void PageUp();

    /// Scroll content by one page to the bottom.
    void PageDown();

    /// Vertically scroll to the beginning of the content.
    void ScrollToHome();

    /// Vertically scroll to the end of the content.
    void ScrollToEnd();

    /// Scroll horizontally to the specified offset.
    void ScrollToHorizontalOffset(float offset);

    /// Scroll vertically to the specified offset.
    void ScrollToVerticalOffset(float offset);

    /// Occurs when the text selection has changed
    UIElement::RoutedEvent_<RoutedEventHandler> SelectionChanged();

    /// Occurs when content changes in the text element
    UIElement::RoutedEvent_<RoutedEventHandler> TextChanged();

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* AcceptsReturnProperty;
    static const DependencyProperty* AcceptsTabProperty;
    static const DependencyProperty* CaretBrushProperty;
    static const DependencyProperty* HorizontalScrollBarVisibilityProperty;
    static const DependencyProperty* IsReadOnlyProperty;
    static const DependencyProperty* IsSelectionActiveProperty;
    static const DependencyProperty* PanningModeProperty;
    static const DependencyProperty* SelectionBrushProperty;
    static const DependencyProperty* SelectionOpacityProperty;
    static const DependencyProperty* VerticalScrollBarVisibilityProperty;
    //@}

    /// Routed Events
    //@{
    static const RoutedEvent* SelectionChangedEvent;
    static const RoutedEvent* TextChangedEvent;
    //@}

protected:
    void SetIsSelectionActive(bool isActive);

    void RaiseSelectionChanged();
    virtual void OnSelectionChanged(const RoutedEventArgs& e);

    void RaiseTextChanged();
    virtual void OnTextChanged(const RoutedEventArgs& e);

    // Notifies inheritors of BaseTextBox property changes
    //@{
    virtual void OnCaretBrushChanged(Brush* oldBrush, Brush* newBrush);
    virtual void OnSelectionBrushChanged(Brush* oldBrush, Brush* newBrush);
    virtual void OnSelectionOpacityChanged(float oldOpacity, float newOpacity);
    virtual void OnIsReadOnlyChanged(bool oldValue, bool newValue);
    //@}

    // Do the work of line up. Can be overridden by subclass to implement true line up.
    virtual void DoLineUp(ScrollViewer* scrollViewer);

    // Do the work of line down. Can be overridden by subclass to implement true line down.
    virtual void DoLineDown(ScrollViewer* scrollViewer);

    // Gets the ScrollViewer that manages text scroll
    virtual ScrollViewer* GetScrollViewer() const;

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From UIElement
    //@{
    void OnMouseWheel(const MouseWheelEventArgs& e) override;
    //@}

    /// From Control
    //@{
    void UpdateVisualStates() override;
    //@}

    NS_DECLARE_REFLECTION(BaseTextBox, Control)
};

}


#endif
