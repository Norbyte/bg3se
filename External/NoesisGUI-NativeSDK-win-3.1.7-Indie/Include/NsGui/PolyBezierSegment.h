////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_POLYBEZIERSEGMENT_H__
#define __GUI_POLYBEZIERSEGMENT_H__


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
/// Represents one or more cubic Bezier curves.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.polybeziersegment
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API PolyBezierSegment: public PathSegment
{
public:
    PolyBezierSegment ();
    PolyBezierSegment(const Point* points, uint32_t numPoints, bool isStroked);
    ~PolyBezierSegment();

    /// Gets or sets the end point of the line segment.
    //@{
    PointCollection* GetPoints() const;
    void SetPoints(PointCollection* points);
    //@}

    /// Adds this PolyBezierSegment to the path
    void BuildPath(PathContext& path) override;

    // Hides Freezable methods for convenience
    //@{
    Ptr<PolyBezierSegment> Clone() const;
    Ptr<PolyBezierSegment> CloneCurrentValue() const;
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

    NS_DECLARE_REFLECTION(PolyBezierSegment, PathSegment)
};

}


#endif
