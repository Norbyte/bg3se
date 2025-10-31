////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __DRAWING_SIZE_H__
#define __DRAWING_SIZE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionImplement.h>
#include <NsDrawing/TypesApi.h>
#include <NsCore/StringFwd.h>


namespace Noesis
{

struct Size;
struct Sizei;
struct Point;
struct Pointi;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Size. Describes the size of an object.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Size
{
    float width;
    float height;

    /// Default constructor which initializes all values to 0
    Size();

    /// Constructor from width and height values
    Size(float w, float h);

    /// Constructor from integer Sizei
    Size(const Sizei& size);

    /// Constructor from a Point
    NS_DRAWING_TYPES_API explicit Size(const Point& point);

    /// Copy constructor
    Size(const Size& size) = default;

    /// Copy operator
    Size& operator=(const Size& size) = default;

    /// Comparison operators
    bool operator==(const Size& size) const;
    bool operator!=(const Size& size) const;

    /// Math operators
    Size operator+(const Size& size) const;
    Size operator-(const Size& size) const;
    Size operator*(float k) const;
    Size operator/(float k) const;
    Size& operator+=(const Size& size);
    Size& operator-=(const Size& size);
    Size& operator*=(float k);
    Size& operator/=(float k);

    /// Expands width and height with the specified size
    void Expand(const Size& size);

    /// Scales width and height by a factor
    void Scale(float scaleX, float scaleY);

    /// Generates a string representation of the size
    /// The string has the following form: "width,height"
    NS_DRAWING_TYPES_API String ToString() const;

    /// Tries to parse a Size from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Size& result);

    /// Empty size (width and height of 0)
    static Size Zero();

    /// Infinite size (defined as the higher positive integer)
    static Size Infinite();

    NS_IMPLEMENT_INLINE_REFLECTION(Size, NoParent)
    {
        NsProp("Width", &Size::width);
        NsProp("Height", &Size::height);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Sizei. Describes the integer size of an object.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Sizei
{
    uint32_t width;
    uint32_t height;

    /// Default constructor which initializes all values to 0
    Sizei();

    /// Constructor from width and height values
    Sizei(uint32_t w, uint32_t h);

    /// Constructor from a float size
    Sizei(const Size& size);

    /// Constructor from a Pointi
    NS_DRAWING_TYPES_API explicit Sizei(const Pointi& point);

    /// Copy constructor
    Sizei(const Sizei& size) = default;

    /// Copy operator
    Sizei& operator=(const Sizei& size) = default;

    /// Comparison operators
    bool operator==(const Sizei& size) const;
    bool operator!=(const Sizei& size) const;

    /// Math operators
    Sizei operator+(const Sizei& size) const;
    Sizei operator-(const Sizei& size) const;
    Sizei operator*(uint32_t k) const;
    Sizei operator/(uint32_t k) const;
    Sizei& operator+=(const Sizei& size);
    Sizei& operator-=(const Sizei& size);
    Sizei& operator*=(uint32_t k);
    Sizei& operator/=(uint32_t k);

    /// Expands width and height with the specified size
    void Expand(const Sizei& size);

    /// Scales width and height by a factor
    void Scale(uint32_t scaleX, uint32_t scaleY);

    /// Generates a string representation of the size
    /// The string has the following form: "width,height"
    NS_DRAWING_TYPES_API String ToString() const;

    /// Tries to parse a Sizei from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Sizei& result);

    /// Empty size (width and height of 0)
    static Sizei Zero();

    NS_IMPLEMENT_INLINE_REFLECTION(Sizei, NoParent)
    {
        NsProp("Width", &Size::width);
        NsProp("Height", &Size::height);
    }
};

}

#include <NsDrawing/Size.inl>

#endif
