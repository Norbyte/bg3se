////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_PATHSEGMENT_H__
#define __GUI_PATHSEGMENT_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Animatable.h>


namespace Noesis
{

struct PathContext;
class PathFigure;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a complex shape that may be composed of arcs, curves, ellipses, lines and rectangles.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.pathsegment
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API PathSegment: public Animatable
{
public:
    PathSegment();
    virtual ~PathSegment() = 0;

    /// Gets or sets a value that indicates whether the join between this PathSegment and the
    /// previous PathSegment is treated as a corner when it is stroked with a Pen
    //@{
    bool GetIsSmoothJoin() const;
    void SetIsSmoothJoin(bool smoothJoin);
    //@}

    /// Gets or sets a value that determines how the intersecting areas contained in this 
    /// PathSegment are combined
    //@{
    bool GetIsStroked() const;
    void SetIsStroked(bool stroked);
    //@}

    /// Adds this PathSegment to the path
    virtual void BuildPath(PathContext& path) = 0;

public:
    /// Dependency property
    static const DependencyProperty* IsSmoothJoinProperty;
    static const DependencyProperty* IsStrokedProperty;

private:
    friend class PathFigure;
    void SetPathFigure(PathFigure* pathFigure);

protected:
    void InvalidatePath();

private:
    PathFigure* mPathFigure;

    NS_DECLARE_REFLECTION(PathSegment, Animatable)
};

}


#endif
