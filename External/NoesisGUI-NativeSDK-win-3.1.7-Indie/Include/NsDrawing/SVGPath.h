////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __DRAWING_SVGPATH_H__
#define __DRAWING_SVGPATH_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsDrawing/TypesApi.h>
#include <NsCore/Vector.h>


namespace Noesis
{

struct Rect;
struct Point;
struct PathCommandIterator;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Paths represent the geometry of the outline of an object
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_DRAWING_TYPES_API SVGPath
{
    /// Empty path constructor
    SVGPath();

    /// Builds a path from SVG path commands as described in http://www.w3.org/TR/SVG11/paths.html
    /// Example: "M 100, 100 L 200, 100 200, 200 Z"
    SVGPath(const char* svgCommands, bool reverseCw = false);

    /// Parses a string value to create an instance of a Path
    static bool TryParse(const char* str, SVGPath& result, bool reverseCw = false);

    /// Resets the path to an empty one
    void Clear();

    /// Rebuilds the path with a new set of SVG path commands
    void SetCommands(const char* svgCommands, bool reverseCw = false);

    /// Returns an iterator of commands describing the path
    PathCommandIterator GetCommands() const;

    /// Calculates path bounds
    Rect CalculateBounds() const;

    /// Indicates whether the path contains the specified point
    bool FillContains(const Point& point) const;

    /// Determines whether the specified point is contained in the given stroke
    bool StrokeContains(const Point& point, float strokeThickness) const;

public:
    /// Override default fill rule when drawing this path
    bool overrideFillRule;

    struct Move
    {
        bool absolute;
        float x, y;
    };

    struct Line
    {
        bool absolute;
        float x, y;
    };

    struct HLine
    {
        bool absolute;
        float coord;
    };

    enum Fill
    {
        Fill_EvenOdd,
        Fill_NonZero
    };

    struct VLine
    {
        bool absolute;
        float coord;
    };

    struct Cubic
    {
        bool absolute;
        float x0, y0;
        float x1, y1;
        float x2, y2;
    };

    struct Quadratic
    {
        bool absolute;
        float x0, y0;
        float x1, y1;
    };

    struct SmoothCubic
    {
        bool absolute;
        float x0, y0;
        float x1, y1;
    };

    struct SmoothQuadratic
    {
        bool absolute;
        float x0, y0;
    };

    struct Arc
    {
        float rx, ry;
        float rot;
        float x, y;
    
        bool isLarge;
        bool isCw;
        bool absolute;
    };

    enum FigureCommand
    {
        FigureCommand_Move,
        FigureCommand_Line,
        FigureCommand_HLine,
        FigureCommand_VLine,
        FigureCommand_Cubic,
        FigureCommand_Quadratic,
        FigureCommand_SmoothCubic,
        FigureCommand_SmoothQuadratic,
        FigureCommand_Arc,
        FigureCommand_Close
    };

    struct Command
    {
        FigureCommand figureCommand;

        union
        {
            Move move;
            Line line;
            HLine hLine;
            VLine vLine;
            Cubic cubic;
            Quadratic quadratic;
            SmoothCubic smoothCubic;
            SmoothQuadratic smoothQuadratic;
            Arc arc;
        };
    };

    Fill fillRule;
    Vector<uint32_t> commands;

    NS_DECLARE_REFLECTION(SVGPath, NoParent)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_DRAWING_TYPES_API PathContext
{
    void PushCommand(const SVGPath::Command& command);
    Vector<uint32_t, 512> commands;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_DRAWING_TYPES_API PathCommandIterator
{
    PathCommandIterator(const uint32_t* data, const uint32_t* end);
    PathCommandIterator(const PathCommandIterator& it);

    bool End() const;
    SVGPath::Command Next();

private:
    PathCommandIterator operator=(const PathCommandIterator&) = delete;

    const uint32_t* mData;
    const uint32_t* const mEnd;
};

NS_WARNING_POP

}

#endif
