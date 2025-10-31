////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __DRAWING_CORNERRADIUS_H__
#define __DRAWING_CORNERRADIUS_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionImplementEmpty.h>
#include <NsDrawing/TypesApi.h>
#include <NsCore/String.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// CornerRadius. Represents the radii of rectangle corners.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct CornerRadius
{
    float topLeft;
    float topRight;
    float bottomRight;
    float bottomLeft;

    CornerRadius();
    CornerRadius(float radius);
    CornerRadius(float tl, float tr, float br, float bl);
    CornerRadius(const CornerRadius& cornerRadius) = default;

    /// Copy operator
    CornerRadius& operator=(const CornerRadius& cornerRadius) = default;

    /// Validates this instance for the given constraints
    NS_DRAWING_TYPES_API bool IsValid(bool allowNegative, bool allowNegativeInf,
        bool allowPositiveInf, bool allowNaN) const;

    /// Comparisson operators
    bool operator==(const CornerRadius& cornerRadius) const;
    bool operator!=(const CornerRadius& cornerRadius) const;

    /// Creates a string representation of this CornerRadius structure
    /// The string has the following form: "topLeft,topRight,bottomRight,bottomLeft" or "radius"
    NS_DRAWING_TYPES_API String ToString() const;

    /// Tries to parse a CornerRadius from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, CornerRadius& radius);

    NS_IMPLEMENT_INLINE_REFLECTION_(CornerRadius, NoParent)
};

}

#include <NsDrawing/CornerRadius.inl>

#endif
