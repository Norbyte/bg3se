////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_POLYLINESEGMENT_H__
#define __GUI_POLYLINESEGMENT_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/PathSegment.h>


namespace Noesis
{

struct Point;
struct NotifyCollectionChangedEventArgs;
template<class T> class BoxedFreezableCollection;
typedef BoxedFreezableCollection<Point> PointCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a set of line segments defined by a PointCollection with each Point specifying the
/// end point of a line segment.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.polylinesegment
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API PolyLineSegment: public PathSegment
{
public:
    PolyLineSegment ();
    PolyLineSegment(const Point* points, uint32_t numPoints, bool isStroked);
    ~PolyLineSegment();

    /// Gets or sets the end point of the line segment.
    //@{
    PointCollection* GetPoints() const;
    void SetPoints(PointCollection* points);
    //@}

    /// Adds this PolyLineSegment to the path
    void BuildPath(PathContext& path) override;

    // Hides Freezable methods for convenience
    //@{
    Ptr<PolyLineSegment> Clone() const;
    Ptr<PolyLineSegment> CloneCurrentValue() const;
    //@}

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

public:
    static const DependencyProperty* PointsProperty;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

private:
    void OnPointsCollectionChanged(BaseComponent* s, const NotifyCollectionChangedEventArgs& e);

    NS_DECLARE_REFLECTION(PolyLineSegment, PathSegment)
};

}


#endif
