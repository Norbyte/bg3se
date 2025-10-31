////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __MATH_MATRIX_H__
#define __MATH_MATRIX_H__


#include <NsCore/Noesis.h>
#include <NsCore/Error.h>
#include <NsCore/ReflectionImplementEmpty.h>
#include <NsMath/Vector.h>
#include <NsMath/VectorMathApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 2x2 Matrix
////////////////////////////////////////////////////////////////////////////////////////////////////
class Matrix2
{
public:
    /// Constructors
    Matrix2() = default;
    Matrix2(const Matrix2& m) = default;
    Matrix2(float v00, float v01, float v10, float v11);
    Matrix2(const Vector2& v0, const Vector2& v1);
    Matrix2(const float* values);

    /// Accessors
    Vector2& operator[](uint32_t i);
    const Vector2& operator[](uint32_t i) const;
    const float* GetData() const;

    /// In-Place operators
    Matrix2& operator=(const Matrix2& m) = default;
    Matrix2& operator*=(const Matrix2& m);
    Matrix2& operator*=(float v);
    Matrix2& operator/=(float v);

    /// Logic operators
    inline bool operator==(const Matrix2& m) const;
    inline bool operator!=(const Matrix2& m) const;

    /// Static functions
    static Matrix2 Identity();
    static Matrix2 Scale(float scaleX, float scaleY);
    static Matrix2 Rot(float radians);
    static Matrix2 ShearXY(float shear);
    static Matrix2 ShearYX(float shear);

    /// Returns a string that represents the current matrix
    NS_MATH_VECTORMATH_API String ToString() const;

private:
    Vector2 mVal[2];

    NS_IMPLEMENT_INLINE_REFLECTION_(Matrix2, NoParent)
};

/// Computes the transpose of the matrix
Matrix2 Transpose(const Matrix2& m);

/// Computes the inverse of the matrix
Matrix2 Inverse(const Matrix2& m);
Matrix2 Inverse(const Matrix2& m, float determinant);

/// Compute the determinant of the matrix
float Determinant(const Matrix2& m);

/// Left-multiplies a matrix by a scale matrix
Matrix2 PreScale(float scaleX, float scaleY, const Matrix2& m);

/// Left-multiplies a matrix by a Z-rotation matrix
Matrix2 PreRot(float radians, const Matrix2& m);

/// Right-multiples a matrix by a scale matrix
Matrix2 PostScale(const Matrix2& m, float scaleX, float scaleY);

/// Right-multiplies a matrix by a Z-rotation matrix
Matrix2 PostRot(const Matrix2& m, float radians);

/// Operators
const Matrix2 operator*(const Matrix2& m, float v);
const Matrix2 operator*(float v, const Matrix2& m);
const Matrix2 operator/(const Matrix2& m, float v);
const Vector2 operator*(const Vector2& v, const Matrix2& m);
const Matrix2 operator*(const Matrix2& m0, const Matrix2& m1);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 3x3 Matrix
////////////////////////////////////////////////////////////////////////////////////////////////////
class Matrix3
{
public:
    /// Constructors
    Matrix3() = default;
    Matrix3(const Matrix3& m) = default;
    Matrix3(float v00, float v01, float v02, float v10, float v11, float v12, float v20,
        float v21, float v22);
    Matrix3(const Vector3& v0, const Vector3& v1, const Vector3& v2);
    Matrix3(const float* values);

    /// Accessors
    Vector3& operator[](uint32_t i);
    const Vector3& operator[](uint32_t i) const;
    const float* GetData() const;

    /// In-Place operators
    Matrix3& operator=(const Matrix3& m) = default;
    Matrix3& operator*=(const Matrix3& m);
    Matrix3& operator*=(float v);
    Matrix3& operator/=(float v);

    /// Logic operators
    bool operator==(const Matrix3& m) const;
    bool operator!=(const Matrix3& m) const;

    /// Static functions
    //@{
    static Matrix3 Identity();
    static Matrix3 Scale(float scaleX, float scaleY, float scaleZ);
    static Matrix3 RotX(float radians);
    static Matrix3 RotY(float radians);
    static Matrix3 RotZ(float radians);

    /// Returns a string that represents the current matrix
    NS_MATH_VECTORMATH_API String ToString() const;

private:
    Vector3 mVal[3];

    NS_IMPLEMENT_INLINE_REFLECTION_(Matrix3, NoParent)
};

/// Computes the transpose of the matrix
Matrix3 Transpose(const Matrix3& m);

/// Affinity checking (last column is 0 0 1)
bool IsAffine(const Matrix3& m);

/// Computes the inverse of the matrix
Matrix3 Inverse(const Matrix3& m);
Matrix3 Inverse(const Matrix3& m, float determinant);

/// Computes the determinant of the matrix
float Determinant(const Matrix3& m);

/// Left-multiplies a matrix by a scale matrix
Matrix3 PreScale(float scaleX, float scaleY, float scaleZ, const Matrix3& m);

/// Left-multiplies a matrix by a X-rotation matrix
Matrix3 PreRotX(float radians, const Matrix3& m);

/// Left-multiplies a matrix by a Y-rotation matrix
Matrix3 PreRotY(float radians, const Matrix3& m);

/// Left-multiplies a matrix by a Z-rotation matrix
Matrix3 PreRotZ(float radians, const Matrix3& m);

/// Right-multiplies a matrix by a scale matrix
Matrix3 PostScale(const Matrix3& m, float scaleX, float scaleY, float scaleZ);

/// Right-multiplies a matrix by a X-rotation matrix
Matrix3 PostRotX(const Matrix3& m, float radians);

/// Right-multiplies a matrix by a Y-rotation matrix;
Matrix3 PostRotY(const Matrix3& m, float radians);

/// Right-multiplies a matrix by a Z-rotation matrix
Matrix3 PostRotZ(const Matrix3& m, float radians);

/// Operators
const Matrix3 operator*(const Matrix3& m, float v);
const Matrix3 operator*(float v, const Matrix3& m);
const Matrix3 operator/(const Matrix3& m, float v);
const Vector3 operator*(const Vector3& v, const Matrix3& m);
const Matrix3 operator*(const Matrix3& m0, const Matrix3& m1);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 4x4 Matrix
////////////////////////////////////////////////////////////////////////////////////////////////////
class Matrix4
{
public:
    /// Constructors
    Matrix4() = default;
    Matrix4(const Matrix4& m) = default;
    Matrix4(float v00, float v01, float v02, float v03, float v10, float v11, float v12, float v13,
        float v20, float v21, float v22, float v23, float v30, float v31, float v32, float v33);
    Matrix4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3);
    Matrix4(const float* values);

    /// Accessors
    Vector4& operator[](uint32_t i);
    const Vector4& operator[](uint32_t i) const;
    const float* GetData() const;

    /// In-Place operators
    Matrix4& operator=(const Matrix4& m) = default;
    Matrix4& operator*=(const Matrix4& m);
    Matrix4& operator*=(float v);
    Matrix4& operator/=(float v);
    
    /// Logic operators
    bool operator==(const Matrix4& m) const;
    bool operator!=(const Matrix4& m) const;

    /// Static functions
    static Matrix4 Identity();
    static Matrix4 Scale(float scaleX, float scaleY, float scaleZ);
    static Matrix4 RotX(float radians);
    static Matrix4 RotY(float radians);
    static Matrix4 RotZ(float radians);

    /// Returns a symmetric perspective projection matrix with far plane at infinite
    static Matrix4 PerspectiveFov(float fovY, float aspect, float zNear);

    /// Returns a viewport matrix that transform from normalized homogeneus space to screen space
    static Matrix4 Viewport(float width, float height);

    /// Returns a string that represents the current matrix
    NS_MATH_VECTORMATH_API String ToString() const;

private:
    Vector4 mVal[4];

    NS_IMPLEMENT_INLINE_REFLECTION_(Matrix4, NoParent)
};

/// Computes the transpose of the matrix
Matrix4 Transpose(const Matrix4& m);

/// Affinity checking (last column is 0 0 0 1)
bool IsAffine(const Matrix4& m);

/// Checks if the matrix behaves as the identity for (x, y, 0) points
bool IsIdentityAtZ0(const Matrix4& m);

/// Computes the inverse of the matrix
Matrix4 Inverse(const Matrix4& m);
Matrix4 Inverse(const Matrix4& m, float determinant);

/// Computes the determinant of the matrix
float Determinant(const Matrix4& m);

/// Operators
const Matrix4 operator*(const Matrix4& m, float v);
const Matrix4 operator*(float v, const Matrix4& m);
const Matrix4 operator/(const Matrix4& m, float v);
const Vector4 operator*(const Vector4& v, const Matrix4& m);
const Matrix4 operator*(const Matrix4& m0, const Matrix4& m1);

}

#include <NsMath/Matrix.inl>

#endif
