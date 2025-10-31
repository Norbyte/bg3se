////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __DRAWING_INT32RECT_H__
#define __DRAWING_INT32RECT_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionImplement.h>
#include <NsCore/StringFwd.h>
#include <NsDrawing/TypesApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Describes the width, height, and location of an integer rectangle.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Int32Rect
{
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;

    /// Default constructor which initializes all values to 0
    Int32Rect();

    /// Constructor which sets initial position and size of the rectangle
    Int32Rect(int32_t x, int32_t y, uint32_t width, uint32_t height);

    /// Copy constructor
    Int32Rect(const Int32Rect& rect) = default;

    /// Copy operator
    Int32Rect& operator=(const Int32Rect& rect) = default;

    /// Comparison operators
    bool operator==(const Int32Rect& rect) const;
    bool operator!=(const Int32Rect& rect) const;

    /// Indicates whether this rectangle is empty (all its members are 0)
    bool IsEmpty() const;

    /// Returns true if this rect has area (width and height greater than 0)
    bool HasArea() const;

    /// Creates a string representation of this rectangle
    /// The string has the following form: "x,y,width,height"
    NS_DRAWING_TYPES_API String ToString() const;

    /// Tries to parse an Int32Rect from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Int32Rect& result);

    /// Empty rect (with all members set to 0)
    static Int32Rect Empty();

    NS_IMPLEMENT_INLINE_REFLECTION(Int32Rect, NoParent)
    {
        NsProp("X", &Int32Rect::x);
        NsProp("Y", &Int32Rect::y);
        NsProp("Width", &Int32Rect::width);
        NsProp("Height", &Int32Rect::height);
    }
};

}

#include <NsDrawing/Int32Rect.inl>


#endif
