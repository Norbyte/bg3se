////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMBINEDGEOMETRY_H__
#define __GUI_COMBINEDGEOMETRY_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Geometry.h>
#include <NsCore/ReflectionDeclareEnum.h>


namespace Noesis
{

enum GeometryCombineMode: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a 2D geometric shape defined by the combination of two Geometry objects.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.combinedgeometry.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API CombinedGeometry: public Geometry
{
public:
    CombinedGeometry();
    CombinedGeometry(Geometry* geometry1, Geometry* geometry2, GeometryCombineMode mode);

    /// Gets or sets the first Geometry object of this CombinedGeometry object.
    //@{
    Geometry* GetGeometry1() const;
    void SetGeometry1(Geometry* geometry);
    //@}
    
    /// Gets or sets the second Geometry object of this CombinedGeometry object.
    //@{
    Geometry* GetGeometry2() const;
    void SetGeometry2(Geometry* geometry);
    //@}
    
    /// Gets or sets the method by which the two geometries (specified by the Geometry1 and Geometry2 properties) are combined. 
    //@{
    GeometryCombineMode GetGeometryCombineMode() const;
    void SetGeometryCombineMode(GeometryCombineMode mode);
    //@}

    /// From Geometry
    //@{
    bool IsEmpty() const;
    //@}

    /// From Freezable
    //@{
    Ptr<CombinedGeometry> Clone() const;
    Ptr<CombinedGeometry> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* Geometry1Property;
    static const DependencyProperty* Geometry2Property;
    static const DependencyProperty* GeometryCombineModeProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    bool OnSubPropertyChanged(const DependencyProperty* dp);
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const;
    //@}

    /// From Geometry
    //@{
    Rect GetRenderBoundsOverride(Pen* pen) const;
    bool FillContainsOverride(const Point& point) const;
    bool StrokeContainsOverride(Pen* pen, const Point& point) const;
    //@}

private:
    bool ContainsPoint(bool pointInGeo1, bool pointInGeo2) const;

private:
    enum UpdateFlags
    {
        UpdateFlags_Geometry1 = Geometry::UpdateFlags_Sentinel,
        UpdateFlags_Geometry2,
        UpdateFlags_CombineMode
    };

    NS_DECLARE_REFLECTION(CombinedGeometry, Geometry)
};

}

#endif
