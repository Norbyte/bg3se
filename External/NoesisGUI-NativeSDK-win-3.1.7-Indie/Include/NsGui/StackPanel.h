////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_STACKPANEL_H__
#define __GUI_STACKPANEL_H__


#include <NsCore/Noesis.h>
#include <NsGui/Panel.h>
#include <NsGui/IScrollInfo.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Arranges child elements into a single horizontal or vertical line.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.stackpanel.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API StackPanel: public Panel, public IScrollInfo
{
public:
    StackPanel();
    ~StackPanel();

    /// Gets or sets orientation of children elements
    //@{
    Orientation GetOrientation() const;
    void SetOrientation(Orientation orientation);
    //@}

    // From IScrollInfo
    //@{
    bool GetCanHorizontallyScroll() const override;
    void SetCanHorizontallyScroll(bool canScroll) override;
    bool GetCanVerticallyScroll() const override;
    void SetCanVerticallyScroll(bool canScroll) override;
    float GetExtentWidth() const override;
    float GetExtentHeight() const override;
    float GetViewportWidth() const override;
    float GetViewportHeight() const override;
    float GetHorizontalOffset() const override;
    float GetVerticalOffset() const override;
    ScrollViewer* GetScrollOwner() const override;
    void SetScrollOwner(ScrollViewer* owner) override;
    void LineLeft() override;
    void LineRight() override;
    void LineUp() override;
    void LineDown() override;
    void PageLeft() override;
    void PageRight() override;
    void PageUp() override;
    void PageDown() override;
    void MouseWheelLeft(float delta = 1.0f) override;
    void MouseWheelRight(float delta = 1.0f) override;
    void MouseWheelUp(float delta = 1.0f) override;
    void MouseWheelDown(float delta = 1.0f) override;
    void SetHorizontalOffset(float offset) override;
    void SetVerticalOffset(float offset) override;
    Rect MakeVisible(Visual* visual, const Rect& rect) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* OrientationProperty;
    //@}

protected:
    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    void EnsureScrollData();
    bool IsScrolling() const;

private:
    friend class VirtualizingStackPanel;
    friend class ScrollViewerTest;

    struct ScrollData;
    ScrollData* mScrollData;

    NS_DECLARE_REFLECTION(StackPanel, Panel)
};

NS_WARNING_POP

}


#endif
