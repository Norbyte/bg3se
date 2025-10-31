////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __DRAWING_RECT_H__
#define __DRAWING_RECT_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionImplement.h>
#include <NsCore/StringFwd.h>
#include <NsDrawing/TypesApi.h>


namespace Noesis
{

class Transform2;
class Matrix4;
struct Size;
struct Sizei;
struct Point;
struct Pointi;
struct Recti;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A 2D Rectangle defined by an origin and a size
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Rect
{
    float x;
    float y;
    float width;
    float height;

    /// Default constructor which initializes all values to 0
    Rect();

    /// Constructor from coordinates for the sides of the rectangle
    /// The width of the new rectangle is right - left
    /// The height of the new rectangle is bottom - top
    Rect(float left, float top, float right, float bottom);

    /// Constructor from size. The created rectangle is located at the origin (0,0)
    explicit Rect(const Size& size);

    /// Constructor from location and size
    Rect(const Point& location, const Size& size);

    /// Constructor from an integer rect
    Rect(const Recti& rect);

    /// Copy constructor
    Rect(const Rect& rect) = default;

    /// Copy operator
    Rect& operator=(const Rect& rect) = default;

    /// Comparison operators
    bool operator==(const Rect& rect) const;
    bool operator!=(const Rect& rect) const;

    /// Gets or sets the position of the rectangle's top-left corner
    Point GetLocation() const;
    void SetLocation(const Point& location);

    /// Gets or sets the width and height of the rectangle
    Size GetSize() const;
    void SetSize(const Size& size);

    /// Accessors to rectangle sides
    float GetLeft() const;
    float GetRight() const;
    float GetTop() const;
    float GetBottom() const;

    /// Accessors to rectangle corners
    Point GetTopLeft() const;
    Point GetTopRight() const;
    Point GetBottomLeft() const;
    Point GetBottomRight() const;

    /// Indicates whether this rectangle is empty (its size is 0)
    bool IsEmpty() const;

    /// Determines whether this rectangle contains the specified point or rectangle
    bool Contains(float x, float y) const;
    bool Contains(const Point& point) const;
    bool Contains(const Rect& rect) const;

    /// Expands or contracts the rectangle by the specified amount
    /// The width of the resulting rectangle is increased or decreased by twice the specified width
    /// offset, because it is applied to both the left and right sides of the rectangle. Likewise,
    /// the height of the rectangle is increased or decreased by twice the specified height
    /// If the specified width or height modifiers shrink the rectangle by more than its current
    /// width or height (giving a negative area) this rectangle becomes the Empty rectangle
    void Inflate(float width, float height);
    void Inflate(const Size& size);

    /// Finds the intersection of two rectangles and stores the result as a Rect structure
    Rect Intersect(const Rect& rect) const;

    /// Indicates whether the specified rectangle intersects with this rectangle
    bool IntersectsWith(const Rect& rect) const;

    /// Translates the rectangle by the specified amount
    void Offset(const Point& offset);

    /// Multiplies the size of the current rectangle by the specified x and y value
    void Scale(float scaleX, float scaleY);

    /// Expands a rectangle exactly enough to contain the specified rectangle or point
    void Expand(const Point& point);
    void Expand(const Rect& rect);

    /// Transforms the rectangle by the given matrix
    void Transform(const Transform2& mtx);
    void Transform(const Matrix4& mtx);

    /// Creates a string representation of this rectangle
    /// The string has the following form: "x,y,width,height"
    NS_DRAWING_TYPES_API String ToString() const;

    /// Tries to parse a Rect from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Rect& result);

    /// Empty rect (with all members set to 0)
    static Rect Empty();

    NS_IMPLEMENT_INLINE_REFLECTION(Rect, NoParent)
    {
        NsProp("X", &Rect::x);
        NsProp("Y", &Rect::y);
        NsProp("Width", &Rect::width);
        NsProp("Height", &Rect::height);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A 2D Rectangle defined by an integer origin and an integer size
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Recti
{
    int x;
    int y;
    uint32_t width;
    uint32_t height;

    /// Default constructor which initializes all values to 0
    Recti();

    /// Constructor from coordinates for the sides of the rectangle
    /// The width of the new rectangle is right - left
    /// The height of the new rectangle is bottom - top
    Recti(int left, int top, int right, int bottom);

    /// Constructor from size. The created rectangle is located at the origin (0,0)
    explicit Recti(const Sizei& size);

    /// Constructor from location and size
    Recti(const Pointi& location, const Sizei& size);

    /// Copy constructor
    Recti(const Recti& rect) = default;

    /// Copy operator
    Recti& operator=(const Recti& rect) = default;

    /// Comparison operators
    bool operator==(const Recti& rect) const;
    bool operator!=(const Recti& rect) const;

    /// Gets or sets the position of the rectangle's top-left corner
    Pointi GetLocation() const;
    void SetLocation(const Pointi& location);

    /// Gets or sets the width and height of the rectangle
    Sizei GetSize() const;
    void SetSize(const Sizei& size);

    /// Accessors to rectangle sides
    int GetLeft() const;
    int GetRight() const;
    int GetTop() const;
    int GetBottom() const;

    /// Accessors to rectangle corners
    Pointi GetTopLeft() const;
    Pointi GetTopRight() const;
    Pointi GetBottomLeft() const;
    Pointi GetBottomRight() const;

    /// Indicates whether this rectangle is empty (its size is 0)
    bool IsEmpty() const;

    /// Determines whether this rectangle contains the specified point or rectangle
    bool Contains(int x, int y) const;
    bool Contains(const Pointi& point) const;
    bool Contains(const Recti& rect) const;

    /// Expands or contracts the rectangle by the specified amount
    /// The width of the resulting rectangle is increased or decreased by twice the specified width
    /// offset, because it is applied to both the left and right sides of the rectangle. Likewise,
    /// the height of the rectangle is increased or decreased by twice the specified height
    /// If the specified width or height modifiers shrink the rectangle by more than its current
    /// width or height (giving a negative area) this rectangle becomes the Empty rectangle
    void Inflate(int width, int height);
    void Inflate(const Sizei& size);

    /// Finds the intersection of two rectangles and stores the result as a Recti structure
    Recti Intersect(const Recti& rect) const;

    /// Indicates whether the specified rectangle intersects with this rectangle
    bool IntersectsWith(const Recti& rect) const;

    /// Translates the rectangle by the specified amount
    void Offset(const Pointi& offset);

    /// Multiplies the size of the current rectangle by the specified x and y value
    void Scale(int scaleX, int scaleY);

    /// Expands a rectangle exactly enough to contain the specified rectangle or point
    void Expand(const Pointi& point);
    void Expand(const Recti& rect);

    /// Creates a string representation of this rectangle
    /// The string has the following form: "x,y,width,height"
    NS_DRAWING_TYPES_API String ToString() const;

    /// Tries to parse a Recti from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Recti& result);

    /// Empty rect (with all members set to 0)
    static Recti Empty();

    NS_IMPLEMENT_INLINE_REFLECTION(Recti, NoParent)
    {
        NsProp("X", &Recti::x);
        NsProp("Y", &Recti::y);
        NsProp("Width", &Recti::width);
        NsProp("Height", &Recti::height);
    }
};

}

#include <NsDrawing/Rect.inl>

#endif
