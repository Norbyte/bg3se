////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __MATH_VECTOR_H__
#define __MATH_VECTOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/StringFwd.h>
#include <NsCore/ReflectionImplement.h>
#include <NsMath/VectorMathApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 2D Vector
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Vector2
{
    float x;
    float y;

    /// Constructors
    Vector2() = default;
    Vector2(const Vector2& v) = default;
    Vector2(float x, float y);
    Vector2(const float* values);

    /// Accessors
    float& operator[](uint32_t i);
    float operator[](uint32_t i) const;
    const float* GetData() const;

    /// In-Place operators
    Vector2& operator=(const Vector2& v) = default;
    Vector2& operator+=(const Vector2& v);
    Vector2& operator-=(const Vector2& v);
    Vector2& operator*=(float v);
    Vector2& operator/=(float v);

    /// Logic operators
    bool operator==(const Vector2& v) const;
    bool operator!=(const Vector2& v) const;

    /// Constants
    static Vector2 Zero();
    static Vector2 XAxis();
    static Vector2 YAxis();

    /// Returns a string that represents the current vector
    NS_MATH_VECTORMATH_API String ToString() const;

    NS_IMPLEMENT_INLINE_REFLECTION(Vector2, NoParent)
    {
        NsProp("x", &Vector2::x);
        NsProp("y", &Vector2::y);
    }
};

float Length(const Vector2& v);
float LengthSquared(const Vector2& v);
Vector2 Normalize(const Vector2& v);

Vector2 PerpendicularCCW(const Vector2& v);
Vector2 PerpendicularCW(const Vector2& v);
Vector2 Perpendicular(const Vector2& v, bool cw);

float Dot(const Vector2& v0, const Vector2& v1);
float PerpDot(const Vector2& v0, const Vector2& v1);
const Vector2 Lerp(const Vector2& v0, const Vector2& v1, float t);

/// Returns the signed angle between two vectors
float SignedAngle(const Vector2& v0, const Vector2& v1);

/// Operators
const Vector2 operator+(const Vector2& v);
const Vector2 operator-(const Vector2& v);
const Vector2 operator+(const Vector2& v0, const Vector2& v1);
const Vector2 operator-(const Vector2& v0, const Vector2& v1);
const Vector2 operator*(const Vector2& v, float f);
const Vector2 operator*(float f, const Vector2& v);
const Vector2 operator/(const Vector2& v, float f);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 3D Vector
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Vector3
{
    float x;
    float y;
    float z;

    /// Constructors
    Vector3() = default;
    Vector3(const Vector3& v) = default;
    Vector3(float x, float y, float z);
    Vector3(const float* values);
    Vector3(const Vector2& v, float z);

    /// Accessors
    float& operator[](uint32_t i);
    float operator[](uint32_t i) const;
    const float* GetData() const;
    Vector2 XY() const;
    Vector2 XZ() const;
    Vector2 YZ() const;

    /// In-Place operators
    Vector3& operator=(const Vector3& v) = default;
    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);
    Vector3& operator*=(float v);
    Vector3& operator/=(float v);

    /// Logic operators
    bool operator==(const Vector3& v) const;
    bool operator!=(const Vector3& v) const;

    /// Constants
    static Vector3 Zero();
    static Vector3 XAxis();
    static Vector3 YAxis();
    static Vector3 ZAxis();

    /// Returns a string that represents the current vector
    NS_MATH_VECTORMATH_API String ToString() const;

    NS_IMPLEMENT_INLINE_REFLECTION(Vector3, NoParent)
    {
        NsProp("x", &Vector3::x);
        NsProp("y", &Vector3::y);
        NsProp("z", &Vector3::z);
    }
};

float Length(const Vector3& v);
float LengthSquared(const Vector3& v);
Vector3 Normalize(const Vector3& v);

Vector2 Project(const Vector3& v);
float Dot(const Vector3& v0, const Vector3& v1);
Vector3 Cross(const Vector3& v0, const Vector3& v1);

/// Operators
const Vector3 operator+(const Vector3& v);
const Vector3 operator-(const Vector3& v);
const Vector3 operator+(const Vector3& v0, const Vector3& v1);
const Vector3 operator-(const Vector3& v0, const Vector3& v1);
const Vector3 operator*(const Vector3& v, float f);
const Vector3 operator*(float f, const Vector3& v);
const Vector3 operator/(const Vector3& v, float f);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 4D Vector
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Vector4
{
    float x;
    float y;
    float z;
    float w;

    /// Constructors
    Vector4() = default;
    Vector4(const Vector4& v) = default;
    Vector4(float x, float y, float z, float w);
    Vector4(const float* values);
    Vector4(const Vector3& v, float w);
    Vector4(const Vector2& v, float z, float w);

    /// Accessors
    float& operator[](uint32_t i);
    float operator[](uint32_t i) const;
    const float* GetData() const;
    Vector2 XY() const;
    Vector2 XZ() const;
    Vector2 XW() const;
    Vector2 YZ() const;
    Vector2 YW() const;
    Vector3 XYZ() const;
    Vector3 XYW() const;

    /// In-Place operators
    Vector4& operator=(const Vector4& v) = default;
    Vector4& operator+=(const Vector4& v);
    Vector4& operator-=(const Vector4& v);
    Vector4& operator*=(float v);
    Vector4& operator/=(float v);

    /// Logic operators
    inline bool operator==(const Vector4& v) const;
    inline bool operator!=(const Vector4& v) const;

    /// Constants
    static Vector4 Zero();
    static Vector4 XAxis();
    static Vector4 YAxis();
    static Vector4 ZAxis();
    static Vector4 WAxis();

    /// Returns a string that represents the current vector
    NS_MATH_VECTORMATH_API String ToString() const;

    NS_IMPLEMENT_INLINE_REFLECTION(Vector4, NoParent)
    {
        NsProp("x", &Vector4::x);
        NsProp("y", &Vector4::y);
        NsProp("z", &Vector4::z);
        NsProp("w", &Vector4::w);
    }
};

float Length(const Vector4& v);
float LengthSquared(const Vector4& v);
Vector4 Normalize(const Vector4& v);

Vector3 Project(const Vector4& v);
float Dot(const Vector4& v0, const Vector4& v1);

/// Operators
const Vector4 operator+(const Vector4& v);
const Vector4 operator-(const Vector4& v);
const Vector4 operator+(const Vector4& v0, const Vector4& v1);
const Vector4 operator-(const Vector4& v0, const Vector4& v1);
const Vector4 operator*(const Vector4& v, float f);
const Vector4 operator*(float f, const Vector4& v);
const Vector4 operator/(const Vector4& v, float f);

}

#include <NsMath/Vector.inl>

#endif
