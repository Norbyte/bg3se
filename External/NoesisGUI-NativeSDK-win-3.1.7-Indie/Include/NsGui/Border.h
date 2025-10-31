////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BORDER_H__
#define __GUI_BORDER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Decorator.h>


namespace Noesis
{

class Brush;
class Pen;
class StreamGeometryContext;
struct Thickness;
struct CornerRadius;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Draws a border, background, or both around another element. 
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.border.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Border: public Decorator
{
public:
    Border(bool logicalChild = true);
    ~Border();

    /// Gets or sets border background
    //@{
    Brush* GetBackground() const;
    void SetBackground(Brush* brush);
    //@}

    /// Gets or sets border brush
    //@{
    Brush* GetBorderBrush() const;
    void SetBorderBrush(Brush* brush);
    //@}

    /// Gets or sets border thickness
    //@{
    const Thickness& GetBorderThickness() const;
    void SetBorderThickness(const Thickness& thickness);
    //@}

    /// Gets or sets corner radius
    //@{
    const CornerRadius& GetCornerRadius() const;
    void SetCornerRadius(const CornerRadius& corners);
    //@}

    /// Gets or sets the amount of space between a border and its child element
    //@{
    const Thickness& GetPadding() const;
    void SetPadding(const Thickness& padding);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* BackgroundProperty;
    static const DependencyProperty* BorderBrushProperty;
    static const DependencyProperty* BorderThicknessProperty;
    static const DependencyProperty* CornerRadiusProperty;
    static const DependencyProperty* PaddingProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From UIElement
    //@{
    void OnRender(DrawingContext* context) override;
    //@}

    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    void CreatePen();
    void UpdateGeometries();
    void GenerateSimpleGeometry(const Thickness& borders,
        const CornerRadius& corners);
    void GenerateComplexGeometry(const Thickness& borders,
        const CornerRadius& corners);

private:
    Ptr<Pen> mPen;

    Ptr<Geometry> mBorderGeometry;
    Ptr<Geometry> mBackgroundGeometry;

    Size mPreviousRenderSize;

    union
    {
        struct
        {
            bool isComplexGeometry: 1;
            bool isValidGeometry: 1;
            bool hasBorder: 1;
        } mFlags;

        // To quickly set all flags to 0
        uint8_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(Border, Decorator)
};

NS_WARNING_POP

}

#endif
