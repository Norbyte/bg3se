////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RECTANGLE_H__
#define __GUI_RECTANGLE_H__


#include <NsCore/Noesis.h>
#include <NsGui/Shape.h>


namespace Noesis
{

class RectangleGeometry;
class RectangleTest;
struct Size;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Draws a rectangle.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.shapes.rectangle.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Rectangle final: public Shape
{
public:
    Rectangle();
    ~Rectangle();

    /// Gets or sets the x-axis radius of the ellipse that is used to round the corners of the
    /// rectangle
    //@{
    float GetRadiusX() const;
    void SetRadiusX(float radius);
    //@}

    /// Gets or sets the y-axis radius of the ellipse that is used to round the corners of the
    /// rectangle
    //@{
    float GetRadiusY() const;
    void SetRadiusY(float radius);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* RadiusXProperty;
    static const DependencyProperty* RadiusYProperty;
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
    Size GetMinimumSize() const;

private:
    friend class RectangleTest;

    Ptr<RectangleGeometry> mRenderGeometry;

    NS_DECLARE_REFLECTION(Rectangle, Shape)
};

NS_WARNING_POP

}


#endif
