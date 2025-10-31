////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ELLIPSEGEOMETRY_H__
#define __GUI_ELLIPSEGEOMETRY_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Geometry.h>


namespace Noesis
{

struct Point;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the geometry of a circle or ellipse.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.ellipsegeometry.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API EllipseGeometry final: public Geometry
{
public:
    EllipseGeometry();
    EllipseGeometry(const Point& center, float rX, float rY);

    /// Gets or sets the center point
    //@{
    const Point& GetCenter() const;
    void SetCenter(const Point& center);
    //@}

    /// Gets or sets the x-radius value
    //@{
    float GetRadiusX() const;
    void SetRadiusX(float value);
    //@}

    /// Gets or sets the y-radius value
    //@{
    float GetRadiusY() const;
    void SetRadiusY(float value);
    //@}

    /// From Geometry
    //@{
    bool IsEmpty() const override;
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<EllipseGeometry> Clone() const;
    Ptr<EllipseGeometry> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* CenterProperty;
    static const DependencyProperty* RadiusXProperty;
    static const DependencyProperty* RadiusYProperty;
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
    enum UpdateFlags
    {
        UpdateFlags_Center = Geometry::UpdateFlags_Sentinel,
        UpdateFlags_RadiusX,
        UpdateFlags_RadiusY
    };

    NS_DECLARE_REFLECTION(EllipseGeometry, Geometry)
};

}


#endif
