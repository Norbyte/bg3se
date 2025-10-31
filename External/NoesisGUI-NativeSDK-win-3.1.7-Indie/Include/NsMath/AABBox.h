////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __MATH_AABBOX_H__
#define __MATH_AABBOX_H__


#include <NsCore/Noesis.h>
#include <NsMath/Vector.h>
#include <NsMath/Transform.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 2D Axis Aligned Bounding Box
////////////////////////////////////////////////////////////////////////////////////////////////////
class AABBox2
{
public:
    /// Constructors
    AABBox2();
    AABBox2(const AABBox2& v) = default;
    AABBox2(float minX, float minY, float maxX, float maxY);
    AABBox2(const Vector2& min, const Vector2& max);
    AABBox2(const Vector2& center, float halfSide);

    // Copy operator
    AABBox2& operator=(const AABBox2& box) = default;

    /// Resets to empty bounding box
    void Reset();

    /// Returns ith corner point
    ///         X   Y
    /// [0] : (min,min)
    /// [1] : (max,min)
    /// [2] : (min,max)
    /// [3] : (max,max)
    Vector2 operator[](uint32_t i) const;

    /// Returns the minimal extents of the box
    Vector2& Min();
    const Vector2& Min() const;

    /// Returns the maximal extents of the box
    Vector2& Max();
    const Vector2& Max() const;

    /// Returns the center position of the box
    Vector2 Center() const;

    /// Returns the diagonal vector of the box
    inline Vector2 Diagonal() const;

    /// Expands bounding box to fit a given box
    AABBox2& operator+=(const AABBox2& box);

    /// Expands bounding box to fit the given point
    AABBox2& operator+=(const Vector2& point);

    /// Scales the box. The origin of the scale if the center of the box. 
    AABBox2& Scale(float s);

    /// Logic operators
    bool operator==(const AABBox2& box) const;
    bool operator!=(const AABBox2& box) const;

    /// Checks if the bounding box is empty
    bool Empty() const;

private:
    Vector2 mMin, mMax;
};

/// Operators
const AABBox2 operator+(const AABBox2& box0, const AABBox2& box1);
const AABBox2 operator+(const AABBox2& box, const Vector2& point);
const AABBox2 operator+(const Vector2& point, const AABBox2& box);
const AABBox2 operator*(const AABBox2& box, const Transform2& mtx);
const AABBox2 operator*(const AABBox2& box, float s);
const AABBox2 operator*(float s, const AABBox2& box);

/// Checks if specified point is inside the bounding box
bool Inside(const AABBox2& box, const Vector2& p);

/// Checks if b0 is fully inside b1
bool BoxInside(const AABBox2& b0, const AABBox2& b1);

///  Checks if given bounding boxes overlap
bool BoxesIntersect(const AABBox2& box0, const AABBox2& box1);

/// Returns the intersection of given boxes
AABBox2 Intersect(const AABBox2& box0, const AABBox2& box1);

}

#include <NsMath/AABBox.inl>

#endif
