////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __DRAWING_POINT_H__
#define __DRAWING_POINT_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionImplementEmpty.h>
#include <NsDrawing/TypesApi.h>
#include <NsDrawing/Size.h>
#include <NsMath/Vector.h>


namespace Noesis
{

struct Pointi;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an x- and y-coordinate pair in two-dimensional space
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Point: public Vector2
{
    /// Default constructor that creates a (0,0) point
    Point();

    /// Constructor for x, y
    Point(float x, float y);

    /// Construct from Pointi
    Point(const Pointi& point);

    /// Constructor from Vector
    Point(const Vector2& v);

    /// Constructor from size
    explicit Point(const Size& size);

    /// Copy constructor
    Point(const Point& point) = default;

    /// Copy operator
    Point& operator=(const Point& point) = default;

    /// Generates a string representation of the point
    /// The string has the following form: "x,y"
    NS_DRAWING_TYPES_API String ToString() const;

    /// Tries to parse a Point from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Point& result);

    NS_IMPLEMENT_INLINE_REFLECTION_(Point, Vector2)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an x- and y-coordinate pair in integer two-dimensional space
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Pointi
{
    int x, y;

    /// Default constructor that creates a (0,0) point
    Pointi();

    /// Constructor for x, y
    Pointi(int x, int y);

    /// Constructor from float Point
    Pointi(const Point& p);

    /// Constructor from size
    explicit Pointi(const Sizei& size);

    /// Copy constructor
    Pointi(const Pointi& point) = default;

    /// Copy operator
    Pointi& operator=(const Pointi& point) = default;

    /// Generates a string representation of the point
    /// The string has the following form: "x,y"
    NS_DRAWING_TYPES_API String ToString() const;

    /// Tries to parse a Pointi from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Pointi& result);

    NS_IMPLEMENT_INLINE_REFLECTION_(Pointi, NoParent)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an x-, y-, and z-coordinate point in 3-D space
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Point3D
{
    float x, y, z;

    /// Default constructor that creates a (0,0,0) point
    Point3D();

    /// Constructors
    Point3D(float x, float y, float z);
    Point3D(const Point& point);
    Point3D(const Point3D& point) = default;

    /// Copy operators
    Point3D& operator=(const Point& point);
    Point3D& operator=(const Point3D& point) = default;

    /// Comparison operators
    bool operator==(const Point3D& p) const;
    bool operator!=(const Point3D& p) const;

    /// Generates a string representation of the point
    /// The string has the following form: "x,y,z"
    NS_DRAWING_TYPES_API String ToString() const;

    /// Tries to parse a Point3D from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Point3D& result);

    NS_IMPLEMENT_INLINE_REFLECTION_(Point3D, NoParent)
};

const Point3D operator+(const Point3D& p0, const Point3D& p1);
const Point3D operator-(const Point3D& p0, const Point3D& p1);
const Point3D operator*(const Point3D& p, float v);
const Point3D operator*(float v, const Point3D& p);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an x-, y-, z-, and w-coordinate point in world space
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Point4D
{
    float x, y, z, w;

    /// Default constructor that creates a (0,0,0,0) point
    Point4D();

    /// Constructors
    Point4D(float x, float y, float z, float w);
    Point4D(const Point& point);
    Point4D(const Point3D& point);
    Point4D(const Point4D& point) = default;

    /// Copy operators
    Point4D& operator=(const Point& point);
    Point4D& operator=(const Point3D& point);
    Point4D& operator=(const Point4D& point) = default;

    /// Comparison operators
    bool operator==(const Point4D& p) const;
    bool operator!=(const Point4D& p) const;

    /// Generates a string representation of the point
    /// The string has the following form: "x,y,z,w"
    NS_DRAWING_TYPES_API String ToString() const;

    /// Tries to parse a Point3D from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Point4D& result);

    NS_IMPLEMENT_INLINE_REFLECTION_(Point4D, NoParent)
};

const Point4D operator+(const Point4D& p0, const Point4D& p1);
const Point4D operator-(const Point4D& p0, const Point4D& p1);
const Point4D operator*(const Point4D& p, float v);
const Point4D operator*(float v, const Point4D& p);

}

#include <NsDrawing/Point.inl>

#endif
