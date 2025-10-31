////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SCROLLCONTENTPRESENTER_H__
#define __GUI_SCROLLCONTENTPRESENTER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ContentPresenter.h>
#include <NsGui/IScrollInfo.h>
#include <NsDrawing/Size.h>
#include <NsDrawing/Point.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class ScrollViewer;
class AdornerLayer;
class RectangleGeometry;
class ItemsControlTest;
class ScrollViewerTest;
struct ScrollChangedEventArgs;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Displays the content of a ScrollViewer control.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.scrollcontentpresenter.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ScrollContentPresenter: public ContentPresenter, public IScrollInfo
{
public:
    ScrollContentPresenter();
    ~ScrollContentPresenter();

    /// Gets adorner layer
    AdornerLayer* GetAdornerLayer() const;

    /// Indicates whether the content, if it supports IScrollInfo, should be allowed to control
    /// scrolling
    //@{
    bool GetCanContentScroll() const;
    void SetCanContentScroll(bool value);
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
    static const DependencyProperty* CanContentScrollProperty;

protected:
    /// From BaseRefCounted
    //@{
    int32_t OnDestroy() override;
    //@}

    /// From DependencyObject
    //@{
    void OnInit() override;
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From Visual
    //@{
    uint32_t GetVisualChildrenCount() const override;
    Visual* GetVisualChild(uint32_t index) const override;
    //@}

    /// From UIElement
    //@{
    Ptr<Geometry> GetLayoutClip(const Size& layoutSlotSize) const override;
    //@}

    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    void OnTemplatedParentChanged(FrameworkElement* oldParent,
        FrameworkElement* newParent) override;
    //@}

    /// From ContentPresenter
    //@{
    void OnContentChanged(BaseComponent* oldContent, BaseComponent* newContent) override;
    //@}

private:
    bool IsScrollClient() const;
    void UpdateScrollInfo(FrameworkElement* templatedParent, BaseComponent* content);
    void OnScrollChanged(BaseComponent* sender, const ScrollChangedEventArgs& e);

    void EnsureScrollData();
    void DestroyScrollData();
    bool UpdateScrollData(const Size& extent, const Size& viewport);

private:
    friend class ItemsPresenter;
    friend class ScrollViewerTest;
    friend class ItemsControlTest;

    Ptr<AdornerLayer> mAdornerLayer;
    mutable Ptr<RectangleGeometry> mLayoutClip;

    struct ScrollData;
    ScrollData* mScrollData;
    IScrollInfo* mScrollInfo;

    NS_DECLARE_REFLECTION(ScrollContentPresenter, ContentPresenter)
};

NS_WARNING_POP

}


#endif
