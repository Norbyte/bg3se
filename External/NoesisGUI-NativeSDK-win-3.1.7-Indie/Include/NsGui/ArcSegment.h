////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ARCSEGMENT_H__
#define __GUI_ARCSEGMENT_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/PathSegment.h>
#include <NsGui/Enums.h>
#include <NsCore/ReflectionDeclareEnum.h>


namespace Noesis
{

struct Point;
struct Size;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines the direction an elliptical arc is drawn.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum SweepDirection
{
    /// Specifies that arcs are drawn in a counter clockwise (negative-angle) direction
    SweepDirection_Counterclockwise,
    /// Specifies that arcs are drawn in a clockwise (positive-angle) direction
    SweepDirection_Clockwise
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an elliptical arc between two points.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.arcsegment
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ArcSegment: public PathSegment
{
public:
    ArcSegment();
    ArcSegment(const Point& point, const Size& size, float rotationAngle, bool isLargeArc,
        SweepDirection sweepDirection, bool isStroked);
    ~ArcSegment();

    /// Gets or sets the end point of the elliptical arc.
    //@{
    const Point& GetPoint() const;
    void SetPoint(const Point& point);
    //@}

    /// Gets or sets the x- and y-radius of the arc.
    //@{
    const Size& GetSize() const;
    void SetSize(const Size& size);
    //@}

    /// Gets or sets the amount (in degrees) by which the ellipse is rotated about the x-axis
    //@{
    float GetRotationAngle() const;
    void SetRotationAngle(float angle);
    //@}

    /// Gets or sets a value that indicates whether the arc should be greater than 180 degrees.
    //@{
    bool GetIsLargeArc() const;
    void SetIsLargeArc(bool isLargeArc);
    //@}

    /// Gets or sets a value that specifies whether the arc is drawn in the Clockwise or
    /// Counterclockwise direction.
    //@{
    SweepDirection GetSweepDirection() const;
    void SetSweepDirection(SweepDirection sweepDirection);
    //@}

    /// Adds this ArcSegment to the path
    void BuildPath(PathContext& path) override;

    // Hides Freezable methods for convenience
    //@{
    Ptr<ArcSegment> Clone() const;
    Ptr<ArcSegment> CloneCurrentValue() const;
    //@}

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

public:
    static const DependencyProperty* PointProperty;
    static const DependencyProperty* SizeProperty;
    static const DependencyProperty* RotationAngleProperty;
    static const DependencyProperty* IsLargeArcProperty;
    static const DependencyProperty* SweepDirectionProperty;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    NS_DECLARE_REFLECTION(ArcSegment, PathSegment)
};

}


NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::SweepDirection);


#endif
