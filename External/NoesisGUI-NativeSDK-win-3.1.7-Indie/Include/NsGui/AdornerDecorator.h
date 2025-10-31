////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ADORNERDECORATOR_H__
#define __GUI_ADORNERDECORATOR_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Decorator.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class AdornerLayer;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides an adorner layer for elements beneath it in the visual tree.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.documents.adornerdecorator.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API AdornerDecorator: public Decorator
{
public:
    AdornerDecorator(bool logicalChild = true);
    ~AdornerDecorator();

    /// Gets adorner layer
    AdornerLayer* GetAdornerLayer() const;

protected:
    /// From BaseRefCounted
    //@{
    int32_t OnDestroy() override;
    //@}

    /// From DependencyObject
    //@{
    void OnInit() override;
    //@}

    /// From Visual
    //@{
    uint32_t GetVisualChildrenCount() const override;
    Visual* GetVisualChild(uint32_t index) const override;
    //@}

    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

    /// From Decorator
    //@{
    void OnChildChanged(UIElement* oldChild, UIElement* newChild) override;
    //@}

private:
    Ptr<AdornerLayer> mAdornerLayer;

    NS_DECLARE_REFLECTION(AdornerDecorator, Decorator);
};

NS_WARNING_POP

}

#endif
