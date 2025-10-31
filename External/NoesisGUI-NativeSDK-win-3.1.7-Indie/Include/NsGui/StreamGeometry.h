////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_STREAMGEOMETRY_H__
#define __GUI_STREAMGEOMETRY_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Geometry.h>
#include <NsDrawing/Rect.h>
#include <NsDrawing/SVGPath.h>


namespace Noesis
{

class StreamGeometryContext;
enum FillRule: int32_t;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a geometric shape, described using a StreamGeometryContext or a SVG command string
/// as described in the `W3C <http://www.w3.org/TR/SVG11/paths.html>`_ specs.
///
/// .. code-block:: c#
///
///    StreamGeometry geometry = new StreamGeometry();
///
///    using (StreamGeometryContext context = geometry.Open())
///    {
///        context.BeginFigure(new Point(100.0f, 100.0f), true, true);
///        context.LineTo(new Point(200.0f, 100.0f), false, false);
///        context.LineTo(new Point(200.0f, 200.0f), false, false);
///    }
///
/// .. code-block:: c#
///
///    StreamGeometry geometry = new StreamGeometry();
///    geometry.SetData("M 100, 100 L 200, 100 200, 200 Z");
///
/// .. code-block:: xml
///
///    <Canvas xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
///      <Path Fill="#FFF2F000" Data="M 100, 100 L 200, 100 200, 200 Z"/>
///    </Canvas>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.streamgeometry.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API StreamGeometry: public Geometry
{
public:
    /// Constructor
    StreamGeometry(const char* data = "");

    /// Constructs a StreamGeometry from SVG path commands
    static bool TryParse(const char* str, Ptr<StreamGeometry>& result);

    /// Rebuild the geometry with a new set of `SVG <http://www.w3.org/TR/SVG11/paths.html>`_ 
    /// path commands
    ///
    /// Example:
    ///
    /// .. code-block:: xml
    ///
    ///     M 100, 100 L 200, 100 200, 200 Z
    void SetData(const char* data);

    /// Constructs a stream geometry specifying drawing commands
    StreamGeometryContext Open();

    /// Gets or sets a value that determines how the intersecting areas contained in this 
    /// StreamGeometry are combined
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
    Ptr<StreamGeometry> Clone() const;
    Ptr<StreamGeometry> CloneCurrentValue() const;
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
    static const DependencyProperty* FillRuleProperty;

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

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
    void Close();
    void UpdateFillRule();

private:
    friend class StreamGeometryContext;
    friend class StreamGeometryTest;

    enum UpdateFlags
    {
        UpdateFlags_Path = Geometry::UpdateFlags_Sentinel,
        UpdateFlags_FillRule
    };

    SVGPath mPath;
    mutable Rect mBounds;

    NS_DECLARE_REFLECTION(StreamGeometry, Geometry)
};

NS_WARNING_POP

}


#endif
