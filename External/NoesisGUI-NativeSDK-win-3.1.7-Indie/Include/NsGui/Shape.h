////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SHAPE_H__
#define __GUI_SHAPE_H__


#include <NsCore/Noesis.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

class Brush;
class Pen;
class Geometry;
class MatrixTransform;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a base class for shape elements.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.shapes.shape.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Shape: public FrameworkElement
{
public:
    Shape();
    Shape(const Shape&) = delete;
    Shape& operator=(const Shape&) = delete;
    virtual ~Shape() = 0;

    /// Gets or sets the Brush that specifies how the shape's interior is painted
    //@{
    Brush* GetFill() const;
    void SetFill(Brush* fill);
    //@}

    /// Gets or sets a Stretch value that describes how the shape fills its allocated space
    //@{
    Stretch GetStretch() const;
    void SetStretch(Stretch stretch);
    //@}

    /// Gets or sets the Brush that specifies how the shape's outline is painted
    //@{
    Brush* GetStroke() const;
    void SetStroke(Brush* stroke);
    //@}

    /// Gets or sets a collection of float values that indicate the pattern of dashes and gaps that
    /// is used to outline shapes
    //@{
    const char* GetStrokeDashArray() const;
    void SetStrokeDashArray(const char* dashArray);
    //@}

    /// Gets or sets a PenLineCap enumeration value that specifies how the ends of a dash are drawn
    //@{
    PenLineCap GetStrokeDashCap() const;
    void SetStrokeDashCap(PenLineCap cap);
    //@}

    /// Gets or sets a float that specifies the distance within the dash pattern where a dash begins
    //@{
    float GetStrokeDashOffset() const;
    void SetStrokeDashOffset(float offset);
    //@}

    /// Gets or sets a PenLineCap enumeration value that describes the Shape at the end of a line
    //@{
    PenLineCap GetStrokeEndLineCap() const;
    void SetStrokeEndLineCap(PenLineCap cap);
    //@}

    /// Gets or sets a PenLineJoin enumeration value that specifies the type of join that is used
    /// at the vertices of a Shape
    //@{
    PenLineJoin GetStrokeLineJoin() const;
    void SetStrokeLineJoin(PenLineJoin join);
    //@}

    /// Gets or sets a limit on the ratio of the miter length to half the StrokeThickness of a
    /// Shape element
    //@{
    float GetStrokeMiterLimit() const;
    void SetStrokeMiterLimit(float limit);
    //@}

    /// Gets or sets a PenLineCap enumeration value describing the Shape at the start of a Stroke
    //@{
    PenLineCap GetStrokeStartLineCap() const;
    void SetStrokeStartLineCap(PenLineCap cap);
    //@}

    /// Gets or sets the width of the Shape outline
    //@{
    float GetStrokeThickness() const;
    void SetStrokeThickness(float thickness);
    //@}

    /// Gets or sets the amount to trim the start of the geometry path
    //@{
    float GetTrimStart() const;
    void SetTrimStart(float value);
    //@}

    /// Gets or sets the amount to trim the end of the geometry path
    //@{
    float GetTrimEnd() const;
    void SetTrimEnd(float value);
    //@}

    /// Gets or sets the amount to offset trimming the geometry path
    //@{
    float GetTrimOffset() const;
    void SetTrimOffset(float value);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* FillProperty;
    static const DependencyProperty* StretchProperty;
    static const DependencyProperty* StrokeProperty;
    static const DependencyProperty* StrokeDashArrayProperty;
    static const DependencyProperty* StrokeDashCapProperty;
    static const DependencyProperty* StrokeDashOffsetProperty;
    static const DependencyProperty* StrokeEndLineCapProperty;
    static const DependencyProperty* StrokeLineJoinProperty;
    static const DependencyProperty* StrokeMiterLimitProperty;
    static const DependencyProperty* StrokeStartLineCapProperty;
    static const DependencyProperty* StrokeThicknessProperty;
    static const DependencyProperty* TrimStartProperty;
    static const DependencyProperty* TrimEndProperty;
    static const DependencyProperty* TrimOffsetProperty;
    //@}

protected:
    /// Gets stroke thickness validating stroke brush and thickness size
    float GetValidStrokeThickness() const;

    /// Gets a pen with the shape stroke params set
    Pen* GetPen() const;

    /// Calculates stretching parameters for the given geometry bounds
    void GetStretchInfo(const Rect& bounds, const Size& availableSize,
        Point& offset, Point& scale) const;

    /// Calculates stretched size for the given geometry bounds
    //@{
    Size GetStretchSize(const Rect& bounds,
        const Size& availableSize);
    Size GetStretchSize(const Rect& bounds, const Point& scale);
    //@}

    /// Calculates stretch matrix and returns final arrange size for the given geometry
    Size GetArrangeStretch(Geometry* geometry,
        MatrixTransform* stretchMatrix, const Size& finalSize);

    /// Calculates bounds of the shape for the final size
    Rect GetFinalBounds(const Size& finalSize) const;

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

private:
    void EnsurePen() const;

private:
    mutable Ptr<Pen> mPen;

    NS_DECLARE_REFLECTION(Shape, FrameworkElement)
};

NS_WARNING_POP

}


#endif
