////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_LINEGEOMETRY_H__
#define __GUI_LINEGEOMETRY_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Geometry.h>
#include <NsGui/IRenderProxyCreator.h>


namespace Noesis
{

struct Point;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the geometry of a line. 
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.linegeometry.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API LineGeometry final: public Geometry
{
public:
    LineGeometry();
    LineGeometry(const Point& p1, const Point& p2);

    /// Gets or sets the end point of a line.
    //@{
    const Point& GetEndPoint() const;
    void SetEndPoint(const Point& end);
    //@}
    
    /// Gets or sets the start point of the line.
    //@{
    const Point& GetStartPoint() const;
    void SetStartPoint(const Point& start);
    //@}

    /// From Geometry
    //@{
    bool IsEmpty() const override;
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<LineGeometry> Clone() const;
    Ptr<LineGeometry> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* EndPointProperty;
    static const DependencyProperty* StartPointProperty;
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
    bool StrokeContainsOverride(Pen* pen, const Point& point) const override;
    //@}

private:
    enum UpdateFlags
    {
        UpdateFlags_StartPoint = Geometry::UpdateFlags_Sentinel,
        UpdateFlags_EndPoint
    };

    NS_DECLARE_REFLECTION(LineGeometry, Geometry)
};

}


#endif
