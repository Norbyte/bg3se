////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_QUADRATICBEZIERSEGMENT_H__
#define __GUI_QUADRATICBEZIERSEGMENT_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/PathSegment.h>


namespace Noesis
{

struct Point;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a quadratic Bezier curve drawn between two points.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.quadraticbeziersegment
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API QuadraticBezierSegment: public PathSegment
{
public:
    QuadraticBezierSegment();
    QuadraticBezierSegment(const Point& point1, const Point& point2, bool isStroked);
    ~QuadraticBezierSegment();

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

    /// Adds this QuadraticBezierSegment to the path
    void BuildPath(PathContext& path) override;

    // Hides Freezable methods for convenience
    //@{
    Ptr<QuadraticBezierSegment> Clone() const;
    Ptr<QuadraticBezierSegment> CloneCurrentValue() const;
    //@}

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

public:
    static const DependencyProperty* Point1Property;
    static const DependencyProperty* Point2Property;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    NS_DECLARE_REFLECTION(QuadraticBezierSegment, PathSegment)
};

}


#endif
