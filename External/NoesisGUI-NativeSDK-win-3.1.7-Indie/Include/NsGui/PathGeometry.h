////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_PATHGEOMETRY_H__
#define __GUI_PATHGEOMETRY_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Geometry.h>
#include <NsGui/Enums.h>
#include <NsDrawing/Rect.h>
#include <NsDrawing/SVGPath.h>


namespace Noesis
{

struct NotifyCollectionChangedEventArgs;
class PathFigure;
template<class T> class FreezableCollection;
typedef FreezableCollection<PathFigure> PathFigureCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a complex shape that may be composed of arcs, curves, ellipses, lines and rectangles.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
///        <Path Fill="Red">
///            <Path.Data>
///                <PathGeometry>
///                    <PathFigure IsClosed="True" StartPoint="0,0">
///                        <QuadraticBezierSegment Point1="25,25" Point2="50,0"/>
///                        <LineSegment Point="100,0"/>
///                        <ArcSegment Size="25,50" RotationAngle="30" Point="100,50"/>
///                        <LineSegment Point="100,100"/>
///                        <LineSegment Point="0,100"/>
///                        <BezierSegment Point1="25,75" Point2="25,50" Point3="0,50"/>
///                    </PathFigure>
///                </PathGeometry>
///            </Path.Data>
///        </Path>
///    </Grid>
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.pathgeometry
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API PathGeometry: public Geometry
{
public:
    PathGeometry();

    /// Gets or sets the collection of PathFigure objects that describe the path's contents
    //@{
    PathFigureCollection* GetFigures() const;
    void SetFigures(PathFigureCollection* figures);
    //@}

    /// Gets or sets a value that determines how the intersecting areas contained in this 
    /// PathGeometry are combined
    //@{
    FillRule GetFillRule() const;
    void SetFillRule(FillRule rule);
    //@}

    /// From Geometry
    //@{
    bool IsEmpty() const override;
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<PathGeometry> Clone() const;
    Ptr<PathGeometry> CloneCurrentValue() const;
    //@}

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

public:
    /// Dependency property
    static const DependencyProperty* FiguresProperty;
    static const DependencyProperty* FillRuleProperty;

protected:
    /// From Freezable
    //@{
    void CloneCommonCore(const Freezable* source) override;
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From Geometry
    //@{
    Rect GetRenderBoundsOverride(Pen* pen) const override;
    bool FillContainsOverride(const Point& point) const override;
    bool StrokeContainsOverride(Pen* pen, const Point& point) const override;
    //@}

private:
    friend class PathFigure;
    void InvalidatePath();
    void InvalidateFillRule(FillRule fillRule);

    bool IsDirty() const;
    void BuildPath() const;

    void OnFiguresCollectionChanged(BaseComponent* s, const NotifyCollectionChangedEventArgs& e);

private:
    enum UpdateFlags
    {
        UpdateFlags_Path = Geometry::UpdateFlags_Sentinel,
        UpdateFlags_FillRule
    };

    mutable SVGPath mPath;
    mutable Rect mBounds;

    NS_DECLARE_REFLECTION(PathGeometry, Geometry)
};

NS_WARNING_POP

}


#endif
