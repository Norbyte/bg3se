////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SCROLLVIEWER_H__
#define __GUI_SCROLLVIEWER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ContentControl.h>


namespace Noesis
{

NS_INTERFACE IScrollInfo;
enum Visibility: int32_t;
enum ScrollBarVisibility: int32_t;
class ScrollBar;
class ScrollViewerTest;
struct RequestBringIntoViewEventArgs;
struct CanExecuteRoutedEventArgs;
struct ExecutedRoutedEventArgs;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes a change in the scrolling state for a ScrollViewer.ScrollChanged event.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct ScrollChangedEventArgs: public RoutedEventArgs
{
    /// Gets the updated *height* of the ScrollViewer *extent*
    float extentHeight;
    /// Gets a value that indicates the change in *height* of the ScrollViewer *extent*
    float extentHeightChange;
    /// Gets the updated *width* of the ScrollViewer *extent*
    float extentWidth;
    /// Gets a value that indicates the change in *width* of the ScrollViewer *extent*
    float extentWidthChange;
    /// Gets a value that indicates the change in *horizontal offset* for a ScrollViewer
    float horizontalChange;
    /// Gets the updated *horizontal offset* value for a ScrollViewer
    float horizontalOffset;
    /// Gets a value that indicates the change in *vertical offset* of a ScrollViewer
    float verticalChange;
    /// Gets the updated value of the *vertical offset* for a ScrollViewer
    float verticalOffset;
    /// Gets the updated *height* for a ScrollViewer *viewport*
    float viewportHeight;
    /// Gets a value that indicates the change in *height* for ScrollViewer *viewport*
    float viewportHeightChange;
    /// Gets the updated *width* for a ScrollViewer *viewport*
    float viewportWidth;
    /// Gets a value that indicates the change in *width* of a ScrollViewer *viewport*
    float viewportWidthChange;

    ScrollChangedEventArgs(BaseComponent* source);
};

typedef Delegate<void (BaseComponent*, const ScrollChangedEventArgs&)> ScrollChangedEventHandler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies how ScrollViewer reacts to touch manipulation
////////////////////////////////////////////////////////////////////////////////////////////////////
enum PanningMode: int32_t
{
    /// The ScrollViewer scrolls horizontally and vertically
    PanningMode_Both,
    /// The ScrollViewer scrolls when the user moves a finger horizontally first. If the user moves
    /// the vertically first, the movement is treated as mouse events. After the ScrollViewer begins
    /// to scroll, it will scroll horizontally and vertically
    PanningMode_HorizontalFirst,
    /// The ScrollViewer scrolls horizontally only
    PanningMode_HorizontalOnly,
    /// The ScrollViewer does not respond to touch input
    PanningMode_None,
    /// The ScrollViewer scrolls when the user moves a finger vertically first. If the user moves
    /// the horizontally first, the movement is treated as mouse events. After the ScrollViewer begins
    /// to scroll, it will scroll horizontally and vertically
    PanningMode_VerticalFirst,
    /// The ScrollViewer scrolls vertically only
    PanningMode_VerticalOnly
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a scrollable area that can contain other visible elements.
///
/// http://msdn.microsoft.com/en-us/library/ms612678.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ScrollViewer: public ContentControl
{
public:
    ScrollViewer();
    ~ScrollViewer();

    /// Gets a value that indicates whether elements that support the IScrollInfo interface are
    /// allowed to scroll
    static bool GetCanContentScroll(const DependencyObject* element);
    /// Sets a value that indicates whether elements that support the IScrollInfo interface are
    /// allowed to scroll
    static void SetCanContentScroll(DependencyObject* element, bool canScroll);

    /// Gets a value that indicates whether a horizontal ScrollBar should be displayed
    static ScrollBarVisibility GetHorizontalScrollBarVisibility(const DependencyObject* element);
    /// Sets a value that indicates whether a horizontal ScrollBar should be displayed
    static void SetHorizontalScrollBarVisibility(DependencyObject* element,
        ScrollBarVisibility visibility);

    /// Gets a value that indicates whether a vertical ScrollBar should be displayed
    static ScrollBarVisibility GetVerticalScrollBarVisibility(const DependencyObject* element);
    /// Sets a value that indicates whether a vertical ScrollBar should be displayed
    static void SetVerticalScrollBarVisibility(DependencyObject* element,
        ScrollBarVisibility visibility);

    /// Gets a value that indicates whether the ScrollViewer should scroll contents immediately
    /// during a thumb drag or defer until a drag completes
    static bool GetIsDeferredScrollingEnabled(const DependencyObject* element);
    /// Sets a value that indicates whether the ScrollViewer should scroll contents immediately
    /// during a thumb drag or defer until a drag completes
    static void SetIsDeferredScrollingEnabled(DependencyObject* element, bool value);

    /// Gets the way ScrollViewer reacts to touch manipulation
    static PanningMode GetPanningMode(const DependencyObject* element);
    /// Sets the way ScrollViewer reacts to touch manipulation
    static void SetPanningMode(DependencyObject* element, PanningMode panningMode);

    /// Gets the rate ScrollViewer slows in pixel per squared millisecond when in inertia
    static float GetPanningDeceleration(const DependencyObject* element);
    /// Sets the rate ScrollViewer slows in pixel per squared millisecond when in inertia
    static void SetPanningDeceleration(DependencyObject* element, float deceleration);

    /// Gets the ratio of scrolling offset to translate manipulation offset
    static float GetPanningRatio(const DependencyObject* element);
    /// Sets the ratio of scrolling offset to translate manipulation offset
    static void SetPanningRatio(DependencyObject* element, float panningRatio);

    /// Gets or sets a value that indicates whether content that support the IScrollInfo interface
    /// are allowed to scroll
    //@}
    bool GetCanContentScroll() const;
    void SetCanContentScroll(bool canScroll);
    //@}

    /// Gets or sets a value that indicates whether a horizontal ScrollBar should be displayed
    //@{
    ScrollBarVisibility GetHorizontalScrollBarVisibility() const;
    void SetHorizontalScrollBarVisibility(ScrollBarVisibility visibility);
    //@}

    /// Gets or sets a value that indicates whether a vertical ScrollBar should be displayed
    //@{
    ScrollBarVisibility GetVerticalScrollBarVisibility() const;
    void SetVerticalScrollBarVisibility(ScrollBarVisibility visibility);
    //@}

    /// Gets or sets a value that indicates whether the ScrollViewer should scroll contents
    /// immediately during a thumb drag or defer until a drag completes
    //@{
    bool GetIsDeferredScrollingEnabled() const;
    void SetIsDeferredScrollingEnabled(bool value);
    //@}

    /// Gets or sets the way ScrollViewer reacts to touch manipulation
    //@{
    PanningMode GetPanningMode() const;
    void SetPanningMode(PanningMode panningMode);
    //@}

    /// Gets or sets the rate ScrollViewer slows in pixel per squared millisecond when in inertia
    //@{
    float GetPanningDeceleration() const;
    void SetPanningDeceleration(float deceleration);
    //@}

    /// Gets or sets the ratio of scrolling offset to translate manipulation offset
    //@{
    float GetPanningRatio() const;
    void SetPanningRatio(float panningRatio);
    //@}

    /// Gets a value that indicates whether the horizontal ScrollBar is visible
    Visibility GetComputedHorizontalScrollBarVisibility() const;

    /// Gets a value that indicates whether the vertical ScrollBar is visible
    Visibility GetComputedVerticalScrollBarVisibility() const;

    /// Gets a value that contains the vertical size of the extent
    float GetExtentHeight() const;

    /// Gets a value that contains the horizontal size of the extent
    float GetExtentWidth() const;

    /// Gets a value that contains the horizontal offset of the scrolled content
    float GetHorizontalOffset() const;

    /// Gets a value that represents the vertical size of the content element that can be scrolled
    float GetScrollableHeight() const;

    /// Gets a value that represents the horizontal size of the content element that can be scrolled
    float GetScrollableWidth() const;

    /// Gets a value that contains the vertical offset of the scrolled content
    float GetVerticalOffset() const;

    /// Gets a value that contains the vertical size of the content's viewport
    float GetViewportHeight() const;

    /// Gets a value that contains the horizontal size of the content's viewport
    float GetViewportWidth() const;

    /// Scrolls left within content by one logical unit
    void LineLeft();

    /// Scrolls right within content by one logical unit
    void LineRight();

    /// Scrolls up within content by one logical unit
    void LineUp();

    /// Scrolls down within content by one logical unit
    void LineDown();

    /// Scrolls left within content by one page
    void PageLeft();

    /// Scrolls right within content by one page
    void PageRight();

    /// Scrolls up within content by one page
    void PageUp();

    /// Scrolls down within content by one page
    void PageDown();

    /// Horizontally scroll to the beginning of the content.
    void ScrollToLeftEnd();

    /// Horizontally scroll to the end of the content.
    void ScrollToRightEnd();

    /// Vertically scroll to the beginning of the content.
    void ScrollToTop();

    /// Vertically scroll to the end of the content.
    void ScrollToBottom();

    /// Scroll to Top-Left of the content.
    void ScrollToHome();

    /// Scroll to Bottom-Left of the content.
    void ScrollToEnd();

    /// Scrolls the content within the ScrollViewer to the specified horizontal offset position.
    void ScrollToHorizontalOffset(float offset);

    /// Scrolls the content within the ScrollViewer to the specified vertical offset position.
    void ScrollToVerticalOffset(float offset);

    /// Called by ScrollInfo object when any scrolling property changes
    void InvalidateScrollInfo();

    /// Notifies about changes in scroll position, extent, or viewport size
    UIElement::RoutedEvent_<ScrollChangedEventHandler> ScrollChanged();

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* CanContentScrollProperty;
    static const DependencyProperty* ComputedHorizontalScrollBarVisibilityProperty;
    static const DependencyProperty* ComputedVerticalScrollBarVisibilityProperty;
    static const DependencyProperty* ExtentHeightProperty;
    static const DependencyProperty* ExtentWidthProperty;
    static const DependencyProperty* HorizontalOffsetProperty;
    static const DependencyProperty* HorizontalScrollBarVisibilityProperty;
    static const DependencyProperty* IsDeferredScrollingEnabledProperty;
    static const DependencyProperty* ScrollableHeightProperty;
    static const DependencyProperty* ScrollableWidthProperty;
    static const DependencyProperty* VerticalOffsetProperty;
    static const DependencyProperty* VerticalScrollBarVisibilityProperty;
    static const DependencyProperty* ViewportHeightProperty;
    static const DependencyProperty* ViewportWidthProperty;
    static const DependencyProperty* PanningModeProperty;
    static const DependencyProperty* PanningDecelerationProperty;
    static const DependencyProperty* PanningRatioProperty;
    //@}

    /// Routed events
    //@{
    static const RoutedEvent* ScrollChangedEvent;
    //@}

protected:
    friend class ScrollContentPresenter;
    friend class TextContainer;
    friend class TextBox;
    friend class BaseTextBox;
    friend class ScrollViewerTest;

    /// Gets or sets the element that implements the IScrollInfo interface and provides values for
    /// scrolling properties of this ScrollViewer
    //@{
    IScrollInfo* GetScrollInfo() const;
    void SetScrollInfo(IScrollInfo* scrollInfo);
    //@}

    /// Fires the ScrollChanged event. Inheritors should call base OnScrollChanged(e) if they want
    /// the event to be fired
    virtual void OnScrollChanged(const ScrollChangedEventArgs& e);

    /// From Control
    //@{
    void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot) override;
    //@}

    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    void OnTemplatedParentChanged(FrameworkElement* oldParent, FrameworkElement* newParent) override;
    //@}

    /// From UIElement
    //@{
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseWheel(const MouseWheelEventArgs& e) override;
    void OnManipulationStarting(const ManipulationStartingEventArgs& e) override;
    void OnManipulationStarted(const ManipulationStartedEventArgs& e) override;
    void OnManipulationDelta(const ManipulationDeltaEventArgs& e) override;
    void OnManipulationInertiaStarting(const ManipulationInertiaStartingEventArgs& e) override;
    void OnManipulationCompleted(const ManipulationCompletedEventArgs& e) override;
    //@}

    /// From Visual
    //@{
    void OnDisconnectFromView() override;
    //@}

    /// From DependencyProperty
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

private:
    void DeferScrollToHorizontalOffset(float offset);
    void DeferScrollToVerticalOffset(float offset);

    void BindToTemplatedParent(const DependencyProperty* property);

    void AdjustOffsets(bool& fitsWidth, bool& fitsHeight);

    void RegisterLayoutUpdated();
    void UnregisterLayoutUpdated();
    void OnLayoutUpdated(BaseComponent* sender, const EventArgs& e);

private:
    Ptr<ScrollBar> mHorizontalScrollBar;
    Ptr<ScrollBar> mVerticalScrollBar;
    Ptr<IScrollInfo> mScrollInfo;

    union
    {
        struct
        {
            bool measureInvalidated : 1;
            bool launchScrollEvent : 1;
            bool isTouchPanning : 1;
            bool startedManipulation : 1;
            bool completeManipulation : 1;
            bool disableWheelScrolling : 1;
            bool layoutUpdatedRegistered : 1;
        } mFlags;

        // To quickly set all flags to 0
        uint32_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(ScrollViewer, ContentControl)
};

NS_WARNING_POP

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::PanningMode)

#include <NsGui/ScrollViewer.inl>

#endif
