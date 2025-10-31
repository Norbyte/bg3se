////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_POLYQUADRATICBEZIERSEGMENT_H__
#define __GUI_POLYQUADRATICBEZIERSEGMENT_H__


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
/// Represents one or more quadratic Bezier curves.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.polyquadraticbeziersegment
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API PolyQuadraticBezierSegment: public PathSegment
{
public:
    PolyQuadraticBezierSegment ();
    PolyQuadraticBezierSegment(const Point* points, uint32_t numPoints, bool isStroked);
    ~PolyQuadraticBezierSegment();

    /// Gets or sets the end point of the line segment.
    //@{
    PointCollection* GetPoints() const;
    void SetPoints(PointCollection* points);
    //@}

    /// Adds this PolyQuadraticBezierSegment to the path
    void BuildPath(PathContext& path) override;

    // Hides Freezable methods for convenience
    //@{
    Ptr<PolyQuadraticBezierSegment> Clone() const;
    Ptr<PolyQuadraticBezierSegment> CloneCurrentValue() const;
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

    NS_DECLARE_REFLECTION(PolyQuadraticBezierSegment, PathSegment)
};

}


#endif
