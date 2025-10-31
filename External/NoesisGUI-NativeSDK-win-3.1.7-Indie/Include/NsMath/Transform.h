////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __MATH_TRANSFORM_H__
#define __MATH_TRANSFORM_H__


#include <NsCore/Noesis.h>
#include <NsCore/StringFwd.h>
#include <NsCore/ReflectionImplementEmpty.h>
#include <NsCore/Error.h>
#include <NsMath/Vector.h>
#include <NsMath/Matrix.h>
#include <NsMath/VectorMathApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Affine 2D transformation, a Matrix3 where last column is assumed to be(0.0, 0.0, 1.0)
////////////////////////////////////////////////////////////////////////////////////////////////////
class Transform2
{
public:
    /// Constructors
    Transform2() = default;
    Transform2(const Transform2& m) = default;
    Transform2(float v00, float v01, float v10, float v11, float v20, float v21);
    Transform2(const Vector2& v0, const Vector2& v1, const Vector2& v2);
    Transform2(const float* values);

    /// Accessors
    Vector2& operator[](uint32_t i);
    const Vector2& operator[](uint32_t i) const;
    const float* GetData() const;

    /// Access to the orientation
    Matrix2& Rotation();
    const Matrix2& Rotation() const;

    /// Access to the translation
    Vector2& Translation();
    const Vector2& Translation() const;

    /// Conversion to full 3x3 Matrix
    const Matrix3 ToMatrix3() const;

    /// In-Place operators
    Transform2& operator=(const Transform2& m) = default;
    Transform2& operator*=(const Transform2& m);
    Transform2& operator*=(float v);
    Transform2& operator/=(float v);
    
    void RotateAt(float radians, float centerX, float centerY);
    void ScaleAt(float scaleX, float scaleY, float centerX, float centerY);
    void Translate(float dx, float dy);

    /// Logic operators
    bool operator==(const Transform2& m) const;
    bool operator!=(const Transform2& m) const;

    /// Static functions
    static Transform2 Identity();
    static Transform2 Scale(float scaleX, float scaleY);
    static Transform2 Trans(float transX, float transY);
    static Transform2 Rot(float radians);
    static Transform2 ShearXY(float shear);
    static Transform2 ShearYX(float shear);
    static Transform2 Skew(float radiansX, float radiansY);

    /// Creates a Transform from a string with a list of values
    NS_MATH_VECTORMATH_API static bool TryParse(const char* str, Transform2& output);

    /// Returns a string that represents the current transform
    NS_MATH_VECTORMATH_API String ToString() const;

private:
    Vector2 mVal[3];

    NS_IMPLEMENT_INLINE_REFLECTION_(Transform2, NoParent)
};

/// Computes the inverse of the matrix
Transform2 Inverse(const Transform2& m);
Transform2 Inverse(const Transform2& m, float determinant);

/// Left-multiplies a matrix by a scale matrix
Transform2 PreScale(float scaleX, float scaleY, const Transform2& m);

/// Left-multiplies a matrix by a translation matrix
Transform2 PreTrans(float transX, float transY, const Transform2& m);

/// Left-multiplies a matrix by a X-rotation matrix
Transform2 PreRot(float radians, const Transform2& m);

/// Left-multiplies a matrix by a skew matrix
Transform2 PreSkew(float radiansX, float radiansY, const Transform2& m);

/// Right-multiplies a matrix by a scale matrix
Transform2 PostScale(const Transform2& m, float scaleX, float scaleY);

/// Right-multiplies a matrix by a translation matrix
Transform2 PostTrans(const Transform2& m, float transX, float transY);

/// Right-multiplies a matrix by a X-rotation matrix
Transform2 PostRot(const Transform2& m, float radians);

/// Right-multiplies a matrix by a skew matrix
Transform2 PostSkew(const Transform2& m, float radiansX, float radiansY);

/// Mutiply vector expanded to (x, y, 1) by transform matrix
const Vector2 PointTransform(const Vector2& v, const Transform2& m);

/// Mutiply vector expanded to (x, y, 0) by transform matrix
const Vector2 VectorTransform(const Vector2& v, const Transform2& m);

/// Returns the x-scale applied to the given transform
float ScaleX(const Transform2& m);

/// Returns the y-scale applied to the given transform
float ScaleY(const Transform2& m);

/// Returns the max scale applied to the given transform
float MaxScale(const Transform2& m);

/// Returns the horizontal displacement applied to the given transform
float TransX(const Transform2& m);

/// Returns the vertical displacement applied to the given transform
float TransY(const Transform2& m);

/// Returns whether there are rotations applied to the given transform
bool IsRotating(const Transform2& m);

/// This operator is implemented using PointTransform
const Vector2 operator*(const Vector2& v, const Transform2& m);

/// Operators
const Transform2 operator*(const Transform2& m, float v);
const Transform2 operator*(float v, const Transform2& m);
const Transform2 operator/(const Transform2& m, float v);
const Vector3 operator*(const Vector3& v, const Transform2& m);
const Transform2 operator*(const Transform2& m0, const Transform2& m1);
const Matrix3 operator*(const Transform2& m0, const Matrix3& m1);
const Matrix3 operator*(const Matrix3& m0, const Transform2& m1);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Affine 3D transformation, a Matrix4 where last column is assumed to be (0, 0, 0, 1)
////////////////////////////////////////////////////////////////////////////////////////////////////
class Transform3
{
public:
    /// Constructors
    Transform3() = default;
    Transform3(const Transform3& m) = default;
    Transform3(float v00, float v01, float v02, float v10, float v11, float v12, float v20,
        float v21, float v22, float v30, float v31, float v32);
    Transform3(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3);
    Transform3(const float* values);

    /// Accessors
    Vector3& operator[](uint32_t i);
    const Vector3& operator[](uint32_t i) const;
    const float* GetData() const;

    /// Access to the orientation
    Matrix3& Rotation();
    const Matrix3& Rotation() const;

    /// Access to the translation
    Vector3& Translation();
    const Vector3& Translation() const;

    /// Conversion to full 4x4 Matrix
    const Matrix4 ToMatrix4() const;

    /// In-Place operators
    Transform3& operator=(const Transform3& m) = default;
    Transform3& operator*=(const Transform3& m);
    Transform3& operator*=(float v);
    Transform3& operator/=(float v);

    /// Logic operators
    bool operator==(const Transform3& m) const;
    bool operator!=(const Transform3& m) const;

    /// Static functions
    static Transform3 Identity();
    static Transform3 Scale(float scaleX, float scaleY, float scaleZ);
    static Transform3 Trans(float transX, float transY, float transZ);
    static Transform3 RotX(float radians);
    static Transform3 RotY(float radians);
    static Transform3 RotZ(float radians);

    /// Returns a string that represents the current transform
    NS_MATH_VECTORMATH_API String ToString() const;

private:
    Vector3 mVal[4];

    NS_IMPLEMENT_INLINE_REFLECTION_(Transform3, NoParent)
};

/// Computes the inverse of the matrix
Transform3 Inverse(const Transform3& m);

/// Left-multiplies a matrix by a scale matrix
Transform3 PreScale(float scaleX, float scaleY, float scaleZ, const Transform3& m);

/// Left-multiplies a matrix by a translation matrix
Transform3 PreTrans(float transX, float transY, float transZ, const Transform3& m);

/// Left-multiplies a matrix by a X-rotation matrix
 Transform3 PreRotX(float radians, const Transform3& m);

/// Left-multiplies a matrix by a Y-rotation matrix
Transform3 PreRotY(float radians, const Transform3& m);

/// Left-multiplies a matrix by a Z-rotation matrix
Transform3 PreRotZ(float radians, const Transform3& m);

/// Right-multiplies a matrix by a scale matrix
Transform3 PostScale(const Transform3& m, float scaleX, float scaleY, float scaleZ);

/// Right-multiplies a matrix by a translation matrix
Transform3 PostTrans(const Transform3& m, float transX, float transY, float transZ);

/// Right-multiplies a matrix by a xrotation matrix
Transform3 PostRotX(const Transform3& m, float radians);

/// Right-multiplies a matrix by a Y-rotation matrix
Transform3 PostRotY(const Transform3& m, float radians);

/// Right-multiplies a matrix by a Z-rotation matrix
Transform3 PostRotZ(const Transform3& m, float radians);

/// Mutiply vector expanded to (x, y, z, 1) by transform matrix
const Vector3 PointTransform(const Vector3& v, const Transform3& m);

/// Mutiply vector expanded to (x, y, z, 0) by transform matrix
const Vector3 VectorTransform(const Vector3& v, const Transform3& m);

/// This operator is implemented using PointTransform
const Vector3 operator*(const Vector3& v, const Transform3& m);

/// Operators
const Transform3 operator*(const Transform3& m, float v);
const Transform3 operator*(float v, const Transform3& m);
const Transform3 operator/(const Transform3& m, float v);
const Vector4 operator*(const Vector4& v, const Transform3& m);
const Transform3 operator*(const Transform3& m0, const Transform3& m1);
const Matrix4 operator*(const Transform3& m0, const Matrix4& m1);
const Matrix4 operator*(const Matrix4& m0, const Transform3& m1);

}

#include <NsMath/Transform.inl>

#endif
