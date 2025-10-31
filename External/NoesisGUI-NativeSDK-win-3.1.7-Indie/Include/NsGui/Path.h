////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_PATH_H__
#define __GUI_PATH_H__


#include <NsCore/Noesis.h>
#include <NsGui/Shape.h>
#include <NsGui/MatrixTransform.h>


namespace Noesis
{

class Geometry;
class GeometryGroup;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Draws a series of connected lines and curves.
///
/// .. code-block:: c++
///
///    // Geometry
///    Ptr<StreamGeometry> geometry = *new StreamGeometry();
///
///    StreamGeometryContext context = geometry->Open();
///    context.BeginFigure(Point(260.0f, 200.0f), true);
///    context.ArcTo(Point(140.0f, 200.0f), Size(60.0f, 60.0f), 0, false, true);
///    context.ArcTo(Point(260.0f, 200.0f), Size(60.0f, 60.0f), 0, false, true);
///    context.Close();
///
///    // Path
///    Ptr<Path> path = *new Path();
///    Ptr<Brush> red = *new SolidColorBrush(Color::Red);
///    path->SetFill(red);
///    path->SetData(geometry);
///
///    // Root canvas
///    Ptr<Canvas> root = *new Canvas();
///    Ptr<Brush> gray = *new SolidColorBrush(Color::LightGray);
///    root->SetBackground(gray);
///    root->GetChildren()->Add(path);
///
/// http://msdn.microsoft.com/en-us/library/system.windows.shapes.path.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Path final: public Shape
{
public:
    Path();
    ~Path();

    /// Gets or sets the geometry that specifies the shape to be drawn
    //@{
    Geometry* GetData() const;
    void SetData(Geometry* data);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* DataProperty;
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
    MatrixTransform mStretchMatrix;
    Ptr<GeometryGroup> mRenderGeometry;

    NS_DECLARE_REFLECTION(Path, Shape)
};

NS_WARNING_POP

}


#endif
