////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MESHGEOMETRY_H__
#define __GUI_MESHGEOMETRY_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Geometry.h>
#include <NsDrawing/Rect.h>
#include <NsCore/Vector.h>


namespace Noesis
{

struct Point;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a geometric shape described using a mesh of vertices and indices.
///
/// Geometry is directly sent to the GPU without extra preprocessing, no tessellation, making this
/// class ideal when rendering dynamic geometry that changes per frame.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API MeshGeometry: public Geometry
{
public:
    MeshGeometry();
    ~MeshGeometry();

    /// Sets number of vertices
    void SetNumVertices(uint32_t numVertices);

    /// Sets number of indices
    void SetNumIndices(uint32_t numIndices);

    /// Sets bounding box
    void SetBounds(const Rect& bounds);

    /// Returns a pointer to the internal vertex buffer
    Point* GetVertices();

    /// Returns a pointer to the internal index buffer
    uint16_t* GetIndices();

    /// Notify about a change in the geometry
    void Update();

    /// From Geometry
    //@{
    bool IsEmpty() const override;
    //@}

    /// From Freezable
    //@{
    Ptr<MeshGeometry> Clone() const;
    Ptr<MeshGeometry> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    void CloneCommonCore(const Freezable* source) override;
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
        UpdateFlags_Mesh = Geometry::UpdateFlags_Sentinel
    };

    Vector<Point> mVertices;
    Vector<uint16_t> mIndices;
    Rect mBounds;

    NS_DECLARE_REFLECTION(MeshGeometry, Geometry)
};

NS_WARNING_POP

}


#endif
