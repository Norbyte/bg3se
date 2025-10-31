////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BULLETDECORATOR_H__
#define __GUI_BULLETDECORATOR_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Decorator.h>


namespace Noesis
{

class Brush;
class TextBlock;
class RectangleGeometry;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a layout control that aligns a bullet and another visual object.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.bulletdecorator.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BulletDecorator: public Decorator
{
public:
    BulletDecorator();
    ~BulletDecorator();

    /// Gets or sets the background color for a BulletDecorator control.
    //@{
    Brush* GetBackground() const;
    void SetBackground(Brush* brush);
    //@}

    /// Gets or sets the object to use as the bullet in a BulletDecorator.
    //@{
    UIElement* GetBullet() const;
    void SetBullet(UIElement* bullet);
    //@}

public:
    static const DependencyProperty* BackgroundProperty;

protected:
    /// From DependencyObject
    //@{
    void OnInit() override;
    //@}

    /// From Visual
    //@{
    uint32_t GetVisualChildrenCount() const override;
    Visual* GetVisualChild(uint32_t index) const override;
    //@}

    /// From UIElement
    //@{
    void OnRender(DrawingContext* context) override;
    //@}

    /// From FrameworkElement
    //@{
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const override;
    uint32_t GetLogicalChildrenCount() const override;
    Ptr<BaseComponent> GetLogicalChild(uint32_t index) const override;
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    float GetFirstLineHeight(UIElement* element) const;
    TextBlock* FindText(Visual* visual) const;

private:
    Ptr<UIElement> mBullet;
    Ptr<RectangleGeometry> mBackgroundGeometry;

    NS_DECLARE_REFLECTION(BulletDecorator, Decorator)
};

NS_WARNING_POP

}

#endif
