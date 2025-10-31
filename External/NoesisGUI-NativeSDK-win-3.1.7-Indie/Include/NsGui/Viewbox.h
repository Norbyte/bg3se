////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VIEWBOX_H__
#define __GUI_VIEWBOX_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

struct Size;
struct Point;
class Decorator;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a content decorator that can stretch and scale a single child to fill the available
/// space.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.viewbox.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Viewbox: public FrameworkElement
{
public:
    Viewbox();
    ~Viewbox();

    /// Returns the scale necessary to stretch element size to the available size
    static Point GetStretchScale(const Size& elementSize, const Size& availableSize,
        Stretch stretch, StretchDirection stretchDirection = StretchDirection_Both);

    /// Gets or sets the *StretchDirection*, which determines how scaling is applied to the contents
    /// of a Viewbox
    //@{
    StretchDirection GetStretchDirection() const;
    void SetStretchDirection(StretchDirection stretchDirection);
    //@}
    
    /// Gets or sets the Viewbox *Stretch* mode, which determines how content fits into the
    /// available space
    //@{
    Stretch GetStretch() const;
    void SetStretch(Stretch stretch);
    //@}

    /// Gets or sets the single child of a Viewbox element
    /// \prop
    //@{
    UIElement* GetChild() const;
    void SetChild(UIElement* child);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* StretchDirectionProperty;
    static const DependencyProperty* StretchProperty;
    //@}

protected:
    /// From BaseRefCounted
    //@{
    int32_t OnDestroy() override;
    //@}

    /// From Visual
    //@{
    uint32_t GetVisualChildrenCount() const override;
    Visual* GetVisualChild(uint32_t index) const override;
    //@}

    /// From FrameworkElement
    //@{
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const override;
    uint32_t GetLogicalChildrenCount() const override;
    Ptr<BaseComponent> GetLogicalChild(uint32_t index) const override;
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    void OnTemplatedParentChanged(FrameworkElement* old_, FrameworkElement* new_) override;
    //@}

private:
    Size GetStretchSize(const Size& childSize, const Size& availableSize, Point& scale);
    Decorator* GetStretchDecorator() const;

private:
    NS_DECLARE_REFLECTION(Viewbox, FrameworkElement)
};

NS_WARNING_POP

}


#endif
