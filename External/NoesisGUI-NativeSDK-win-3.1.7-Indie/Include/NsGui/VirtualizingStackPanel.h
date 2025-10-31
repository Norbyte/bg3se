////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VIRTUALIZINGSTACKPANEL_H__
#define __GUI_VIRTUALIZINGSTACKPANEL_H__


#include <NsCore/Noesis.h>
#include <NsGui/VirtualizingPanel.h>
#include <NsGui/IScrollInfo.h>


namespace Noesis
{

class ItemsControl;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Arranges and virtualizes content on a single line that is oriented either horizontally or
/// vertically.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.virtualizingstackpanel.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API VirtualizingStackPanel: public VirtualizingPanel, public IScrollInfo
{
public:
    VirtualizingStackPanel();
    ~VirtualizingStackPanel();

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

    // From Panel
    //@{
    void OnItemsChangedOverride(BaseComponent* sender, const ItemsChangedEventArgs& e) override;
    void GenerateChildren() override;
    void OnConnectToGenerator(ItemsControl* itemsControl) override;
    void OnDisconnectFromGenerator() override;
    //@}

    // From VirtualizingPanel
    //@{
    void BringIndexIntoViewOverride(int32_t index) override;
    //@}

private:
    friend class VirtualizingStackPanelTest;

    struct LayoutHelper;
    struct MakeVisibleHelper;
    struct MeasureHelper;
    struct ArrangeHelper;

    void MakeOffsetVisible(float& offset, float& p0, float& p1, float viewport);

    void OffsetToIndex(Point& offset, bool itemScrolling, bool isHorizontal);
    float OffsetToIndex(ItemContainerGenerator* generator, UIElementCollection* children,
        int numItems, const Size& constraint, bool isHorizontal, bool itemScrolling,
        bool isRecycling, float& direction);
    void AccumDesiredSize(Size& desiredSize, DependencyObject* container, bool isHorizontal);
    void RemoveDesiredSize(Size& desiredSize, float size, bool isHorizontal);
    int CachedItems(int viewportItems, float cacheLength, VirtualizationCacheLengthUnit cacheUnit);
    void EnsureEstimatedSize(ItemContainerGenerator* generator, UIElementCollection* children,
        const Size& constraint, bool isHorizontal, bool isRecycling);
    void UpdateItemSize(int index, float itemSize);
    float ItemSize(int i) const;
    float ItemSize(float size) const;

    Size MeasureViewport(ItemContainerGenerator* generator, UIElementCollection* children,
        const Size& constraint, float viewportSize, int numItems,
        int& firstVisible, int& lastVisible, float& accumVisibleSize, bool isHorizontal,
        bool itemScrolling, bool isRecycling);
    void MeasureUnrealized(ItemContainerGenerator* generator, UIElementCollection* children,
        const Size& constraint, Size& desiredSize, int start, int end,
        bool isHorizontal, bool updateArrangeOffset = false);
    float MeasureContainer(DependencyObject* container, const Size& constraint,
        int itemIndex, bool isHorizontal);
    void AccumDeltaSize(float oldSize, float newSize, float& deltaSize,
        bool shouldAccumSize = true);
    bool AdjustOffset(float direction, float viewportSize, float& deltaSize,
        float& accumVisibleSize, Size& desiredSize, float firstVisibleSize,
        float firstVisiblePerc, int& firstVisible, int& lastVisible, int itemIndex,
        bool isHorizontal, bool itemScrolling, bool makingVisible,
        bool visibleContainerMeasured, int visibleContainerIndex);
    void AdjustOffset(float deltaSize, bool isHorizontal);

    Point ArrangeOffset(bool isHorizontal, bool itemScrolling);
    Size ArrangeSize(const Size& finalSize, const Size& childSize, bool isHorizontal);
    void AccumArrangeOffset(Point& offset, int start, int end, float averageSize,
        bool isHorizontal);
    void AccumArrangeOffset(Point& offset, const Size& arrangeSize, bool isHorizontal);

    Ptr<DependencyObject> GenerateContainer(ItemContainerGenerator* generator,
        UIElementCollection* children, int index, bool isRecycling);
    Ptr<DependencyObject> GenerateContainer(ItemContainerGenerator* generator,
        UIElementCollection* children, int index, bool isRecycling, bool& isNewChild);
    bool IsRecycledContainer(DependencyObject* container, UIElementCollection* children,
        int index, bool isRecycling);
    void GenerateRange(ItemContainerGenerator* generator, UIElementCollection* children,
        int start, int end, bool isHorizontal, bool isRecycling,
        const Size& constraint, Size& desiredSize);

    void RemoveHiddenItems(ItemsControl* itemsControl, ItemContainerGenerator* generator,
        UIElementCollection* children, int firstVisible, int lastVisible, bool isRecycling);
    void RemoveRange(ItemContainerGenerator* generator, UIElementCollection* children,
        int start, int count, bool isRecycling);

    void UpdateScrollData(bool isHorizontal, bool itemScrolling,
        const Size& extent, int itemExtent,
        const Size& viewport, int itemViewport,
        const Point& offset, int itemOffset);

    void EnsureScrollData();
    bool IsScrolling() const;
    bool IsHorizontal() const;
    bool IsVertical() const;
    bool ItemScrolling() const;
    bool ItemScrolling(ItemsControl* itemsControl) const;

    void ResetScrolling();

    void UpdateSizes(BaseComponent* sender, const NotifyCollectionChangedEventArgs& e);

    void CheckVirtualization(BaseComponent* sender, const RoutedEventArgs& e);

private:
    struct ScrollData;
    ScrollData* mScrollData;

    NS_DECLARE_REFLECTION(VirtualizingStackPanel, VirtualizingPanel)
};

NS_WARNING_POP

}


#endif
