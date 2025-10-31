////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BEZIERSEGMENT_H__
#define __GUI_BEZIERSEGMENT_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/PathSegment.h>


namespace Noesis
{

struct Point;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a cubic Bezier curve drawn between two points.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.beziersegment
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BezierSegment: public PathSegment
{
public:
    BezierSegment();
    BezierSegment(const Point& point1, const Point& point2, const Point& point3, bool isStroked);
    ~BezierSegment();

    /// Gets or sets the first control point of the curve.
    //@{
    const Point& GetPoint1() const;
    void SetPoint1(const Point& point);
    //@}

    /// Gets or sets the second control point of the curve.
    //@{
    const Point& GetPoint2() const;
    void SetPoint2(const Point& point);
    //@}

    /// Gets or sets the third control point of the curve.
    //@{
    const Point& GetPoint3() const;
    void SetPoint3(const Point& point);
    //@}

    /// Adds this BezierSegment to the path
    void BuildPath(PathContext& path) override;

    // Hides Freezable methods for convenience
    //@{
    Ptr<BezierSegment> Clone() const;
    Ptr<BezierSegment> CloneCurrentValue() const;
    //@}

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

public:
    static const DependencyProperty* Point1Property;
    static const DependencyProperty* Point2Property;
    static const DependencyProperty* Point3Property;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    NS_DECLARE_REFLECTION(BezierSegment, PathSegment)
};

}


#endif
