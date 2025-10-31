////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_LINESEGMENT_H__
#define __GUI_LINESEGMENT_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/PathSegment.h>


namespace Noesis
{

struct Point;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Creates a line between two points in a PathFigure.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.linesegment
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API LineSegment: public PathSegment
{
public:
    LineSegment();
    LineSegment(const Point& point, bool isStroked);
    ~LineSegment();

    /// Gets or sets the end point of the line segment.
    //@{
    const Point& GetPoint() const;
    void SetPoint(const Point& point);
    //@}

    /// Adds this LineSegment to the path
    void BuildPath(PathContext& path) override;

    // Hides Freezable methods for convenience
    //@{
    Ptr<LineSegment> Clone() const;
    Ptr<LineSegment> CloneCurrentValue() const;
    //@}

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

public:
    static const DependencyProperty* PointProperty;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    NS_DECLARE_REFLECTION(LineSegment, PathSegment)
};

}


#endif
