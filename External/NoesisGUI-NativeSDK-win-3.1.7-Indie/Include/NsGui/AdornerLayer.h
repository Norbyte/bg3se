////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ADORNERLAYER_H__
#define __GUI_ADORNERLAYER_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Vector.h>
#include <NsCore/HashMap.h>
#include <NsGui/CoreApi.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

class Adorner;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a surface for rendering adorners. An adorner layer is guaranteed to be at a higher
/// Z-order than the elements being adorned, so adorners are always rendered on top of the
/// adorned elements.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.documents.adornerlayer
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API AdornerLayer: public FrameworkElement
{
public:
    AdornerLayer();
    ~AdornerLayer();

    /// Returns the first adorner layer in the visual tree above a specified Visual
    static AdornerLayer* GetAdornerLayer(Visual* visual);

    /// Adds an adorner to the adorner layer
    void Add(Adorner* adorner);

    /// Removes the specified Adorner from the adorner layer
    void Remove(Adorner* adorner);

    /// Updates the layout and redraws all of the adorners in the adorner layer
    void Update();

    /// Updates the layout and redraws all of the adorners in the adorner layer that are bound to
    /// the specified element
    void Update(UIElement* element);

protected:
    /// From BaseRefCounted
    //@{
    int32_t OnDestroy() override;
    //@}

    /// From Visual
    //@{
    uint32_t GetVisualChildrenCount() const override;
    Visual* GetVisualChild(uint32_t index) const override;
    void OnVisualParentChanged(Visual* oldParent) override;
    //@}

    /// From FrameworkElement
    //@{
    uint32_t GetLogicalChildrenCount() const override;
    Ptr<BaseComponent> GetLogicalChild(uint32_t index) const override;
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    typedef Vector<Adorner*> AdornerList;
    void UpdateAdorners();
    void UpdateAdorners(UIElement* element, const AdornerList& adorners, bool invalidate);

    void OnAdornedElementUpdated(Visual* visual);
    void OnAdornedElementDestroyed(DependencyObject* d);

    void Clear(UIElement* adornedElement);
    AdornerList& RegisterAdornedElement(UIElement* adornedElement);
    void UnregisterAdornedElement(UIElement* adornedElement, const AdornerList& adorners);

private:
    Vector<Ptr<Adorner>> mAdorners;

    typedef HashMap<UIElement*, AdornerList> AdornedElements;
    AdornedElements mAdornedElements;

    bool mInvalidated;

    NS_DECLARE_REFLECTION(AdornerLayer, FrameworkElement);
};

NS_WARNING_POP

}

#endif
