////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_LINE_H__
#define __GUI_LINE_H__


#include <NsCore/Noesis.h>
#include <NsGui/Shape.h>
#include <NsDrawing/Point.h>


namespace Noesis
{

class LineGeometry;
class MatrixTransform;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Draws a line.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.shapes.line.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Line final: public Shape
{
public:
    Line();
    ~Line();

    /// Gets or sets the x-coordinate of the Line start point
    //@{
    float GetX1() const;
    void SetX1(float x1);
    //@}

    /// Gets or sets the y-coordinate of the Line start point
    //@{
    float GetY1() const;
    void SetY1(float y1);
    //@}

    /// Gets or sets the x-coordinate of the Line end point
    //@{
    float GetX2() const;
    void SetX2(float x2);
    //@}

    /// Gets or sets the y-coordinate of the Line end point
    //@{
    float GetY2() const;
    void SetY2(float y2);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* X1Property;
    static const DependencyProperty* Y1Property;
    static const DependencyProperty* X2Property;
    static const DependencyProperty* Y2Property;
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
    Ptr<LineGeometry> mLineGeometry;
    Ptr<MatrixTransform> mStretchMatrix;

    NS_DECLARE_REFLECTION(Line, Shape)
};

NS_WARNING_POP

}


#endif
