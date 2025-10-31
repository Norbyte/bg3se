////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RECTANGLEGEOMETRY_H__
#define __GUI_RECTANGLEGEOMETRY_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Geometry.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes a two-dimensional rectangle geometry.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.rectanglegeometry.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API RectangleGeometry final: public Geometry
{
public:
    RectangleGeometry();
    RectangleGeometry(const Rect& rect, float rX = 0.0f, float rY = 0.0f);

    /// Gets or sets the x-radius of the ellipse used to round the corners of the rectangle
    //@{
    float GetRadiusX() const;
    void SetRadiusX(float value);
    //@}

    /// Gets or sets the y-radius of the ellipse used to round the corners of the rectangle
    //@{
    float GetRadiusY() const;
    void SetRadiusY(float value);
    //@}

    /// Gets or sets the dimensions of the rectangle
    //@{
    const Rect& GetRect() const;
    void SetRect(const Rect& value);
    //@}

    /// From Geometry
    //@{
    bool IsEmpty() const override;
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<RectangleGeometry> Clone() const;
    Ptr<RectangleGeometry> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* RadiusXProperty;
    static const DependencyProperty* RadiusYProperty;
    static const DependencyProperty* RectProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From Geometry
    //@{
    Rect GetRenderBoundsOverride(Pen* pen) const override;
    bool FillContainsOverride(const Point& point) const override;
    bool StrokeContainsOverride(Pen* pen, const Point& point) const override;
    //@}

private:
    friend class RectangleTest;

    enum UpdateFlags
    {
        UpdateFlags_Rect = Geometry::UpdateFlags_Sentinel,
        UpdateFlags_RadiusX,
        UpdateFlags_RadiusY
    };

    NS_DECLARE_REFLECTION(RectangleGeometry, Geometry)
};

}


#endif
