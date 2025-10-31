////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __DRAWING_THICKNESS_H__
#define __DRAWING_THICKNESS_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionImplement.h>
#include <NsDrawing/TypesApi.h>
#include <NsCore/StringFwd.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Thickness. Describes the thickness of a frame around a rectangle.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Thickness
{
    float left;
    float top;
    float right;
    float bottom;

    Thickness();
    Thickness(float thickness);
    Thickness(float lr, float tb);
    Thickness(float l, float t, float r, float b);
    Thickness(const Thickness& thickness) = default;

    /// Copy operator
    Thickness& operator=(const Thickness& thickness) = default;

    /// Comparisson operators
    bool operator==(const Thickness& thickness) const;
    bool operator!=(const Thickness& thickness) const;

    /// Validates this instance for the given constraints
    NS_DRAWING_TYPES_API bool IsValid(bool allowNegative, bool allowNegativeInf,
        bool allowPositiveInf, bool allowNaN) const;

    /// Creates a string representation of this thickness structure
    /// The string has the following form: "left,top,right,bottom" or "left,top" or "left"
    NS_DRAWING_TYPES_API String ToString() const;

    /// Tries to parse a Thickness from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Thickness& result);

    NS_IMPLEMENT_INLINE_REFLECTION(Thickness, NoParent)
    {
        NsProp("Left", &Thickness::left);
        NsProp("Top", &Thickness::top);
        NsProp("Right", &Thickness::right);
        NsProp("Bottom", &Thickness::bottom);
    }
};

}

#include <NsDrawing/Thickness.inl>

#endif
