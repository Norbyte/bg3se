////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <float.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2::Matrix2(float v00, float v01, float v10, float v11)
{
    mVal[0] = Vector2(v00, v01);
    mVal[1] = Vector2(v10, v11);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2::Matrix2(const Vector2& v0, const Vector2& v1)
{
    mVal[0] = v0;
    mVal[1] = v1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2::Matrix2(const float* values)
{
    NS_ASSERT(values != nullptr);
    mVal[0] = Vector2(values[0], values[1]);
    mVal[1] = Vector2(values[2], values[3]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Vector2& Matrix2::operator[](uint32_t i)
{
    NS_ASSERT(i < 2);
    return mVal[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector2& Matrix2::operator[](uint32_t i) const
{
    NS_ASSERT(i < 2);
    return mVal[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const float* Matrix2::GetData() const
{
    return mVal[0].GetData();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2& Matrix2::operator*=(const Matrix2& m)
{
    mVal[0] = Vector2
    (
        mVal[0][0] * m[0][0] + mVal[0][1] * m[1][0], mVal[0][0] * m[0][1] + 
        mVal[0][1] * m[1][1]
    );

    mVal[1] = Vector2
    (
        mVal[1][0] * m[0][0] + mVal[1][1] * m[1][0], mVal[1][0] * m[0][1] + 
        mVal[1][1] * m[1][1]
    );

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2& Matrix2::operator*=(float v)
{
    mVal[0] *= v;
    mVal[1] *= v;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2& Matrix2::operator/=(float v)
{
    mVal[0] /= v;
    mVal[1] /= v;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Matrix2::operator==(const Matrix2& m) const
{
    return mVal[0] == m[0] && mVal[1] == m[1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Matrix2::operator!=(const Matrix2& m) const
{
    return !(operator==(m));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 Matrix2::Identity()
{
    return Matrix2(Vector2(1.0f, 0.0f), Vector2(0.0f, 1.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 Matrix2::Scale(float scaleX, float scaleY)
{
    return Matrix2(Vector2(scaleX, 0.0f), Vector2(0.0f, scaleY));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 Matrix2::Rot(float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);
    return Matrix2(Vector2(cs, sn), Vector2(-sn, cs));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 Matrix2::ShearXY(float shear)
{
    return Matrix2(Vector2(1.0f, 0.0f), Vector2(shear, 1.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 Matrix2::ShearYX(float shear)
{
    return Matrix2(Vector2(1.0f, shear), Vector2(0.0f, 1.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 Transpose(const Matrix2& m)
{
    return Matrix2(m[0][0], m[1][0], m[0][1], m[1][1]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 Inverse(const Matrix2& m)
{
    return Inverse(m, Determinant(m));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 Inverse(const Matrix2& m, float determinant)
{
    NS_ASSERT(fabsf(determinant) > FLT_EPSILON);
    return Matrix2(m[1][1], -m[0][1], -m[1][0], m[0][0]) / determinant;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline float Determinant(const Matrix2& m)
{
    return (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 PreScale(float scaleX, float scaleY, const Matrix2& m)
{
    return Matrix2
    (
        m[0][0] * scaleX, m[0][1] * scaleX,
        m[1][0] * scaleY, m[1][1] * scaleY
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 PreRot(float radians, const Matrix2& m)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix2
    (
        m[0][0] * cs + m[1][0] * sn, m[0][1] * cs + m[1][1] * sn,
        m[1][0] * cs - m[0][0] * sn, m[1][1] * cs - m[0][1] * sn
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 PostScale(const Matrix2& m, float scaleX, float scaleY)
{
    return Matrix2
    (
        m[0][0] * scaleX, m[0][1] * scaleY,
        m[1][0] * scaleX, m[1][1] * scaleY
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2 PostRot(const Matrix2& m, float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix2
    (
        m[0][0] * cs - m[0][1] * sn, m[0][1] * cs + m[0][0] * sn,
        m[1][0] * cs - m[1][1] * sn, m[1][1] * cs + m[1][0] * sn
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix2 operator*(const Matrix2& m, float v)
{
    return Matrix2(m[0] * v, m[1] * v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix2 operator*(float v, const Matrix2& m)
{
    return Matrix2(m[0] * v, m[1] * v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix2 operator/(const Matrix2& m, float v)
{
    return Matrix2(m[0] / v, m[1] / v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector2 operator*(const Vector2& v, const Matrix2& m)
{
    return Vector2(v[0] * m[0][0] + v[1] * m[1][0], v[0] * m[0][1] + v[1] * m[1][1]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix2 operator*(const Matrix2& m0, const Matrix2& m1)
{
    return Matrix2
    (
        m0[0][0] * m1[0][0] + m0[0][1] * m1[1][0], m0[0][0] * m1[0][1] + m0[0][1] * m1[1][1],
        m0[1][0] * m1[0][0] + m0[1][1] * m1[1][0], m0[1][0] * m1[0][1] + m0[1][1] * m1[1][1]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3::Matrix3(float v00, float v01, float v02, float v10, float v11, float v12, float v20,
    float v21, float v22)
{
    mVal[0] = Vector3(v00, v01, v02);
    mVal[1] = Vector3(v10, v11, v12);
    mVal[2] = Vector3(v20, v21, v22);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3::Matrix3(const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
    mVal[0] = v0;
    mVal[1] = v1;
    mVal[2] = v2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3::Matrix3(const float* values)
{
    NS_ASSERT(values != nullptr);
    mVal[0] = Vector3(values[0], values[1], values[2]);
    mVal[1] = Vector3(values[3], values[4], values[5]);
    mVal[2] = Vector3(values[6], values[7], values[8]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Vector3& Matrix3::operator[](uint32_t i)
{
    NS_ASSERT(i < 3);
    return mVal[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector3& Matrix3::operator[](uint32_t i) const
{
    NS_ASSERT(i < 3);
    return mVal[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const float* Matrix3::GetData() const
{
    return mVal[0].GetData();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3& Matrix3::operator*=(const Matrix3& m)
{
    mVal[0] = Vector3
    (
        mVal[0][0] * m[0][0] + mVal[0][1] * m[1][0] + mVal[0][2] * m[2][0],
        mVal[0][0] * m[0][1] + mVal[0][1] * m[1][1] + mVal[0][2] * m[2][1],
        mVal[0][0] * m[0][2] + mVal[0][1] * m[1][2] + mVal[0][2] * m[2][2]
    );

    mVal[1] = Vector3
    (
        mVal[1][0] * m[0][0] + mVal[1][1] * m[1][0] + mVal[1][2] * m[2][0],
        mVal[1][0] * m[0][1] + mVal[1][1] * m[1][1] + mVal[1][2] * m[2][1],
        mVal[1][0] * m[0][2] + mVal[1][1] * m[1][2] + mVal[1][2] * m[2][2]
    );

    mVal[2] = Vector3
    (
        mVal[2][0] * m[0][0] + mVal[2][1] * m[1][0] + mVal[2][2] * m[2][0],
        mVal[2][0] * m[0][1] + mVal[2][1] * m[1][1] + mVal[2][2] * m[2][1],
        mVal[2][0] * m[0][2] + mVal[2][1] * m[1][2] + mVal[2][2] * m[2][2]
    );

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3& Matrix3::operator*=(float v)
{
    mVal[0] *= v;
    mVal[1] *= v;
    mVal[2] *= v;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3& Matrix3::operator/=(float v)
{
    mVal[0] /= v;
    mVal[1] /= v;
    mVal[2] /= v;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Matrix3::operator==(const Matrix3& m) const
{
    return mVal[0] == m[0] && mVal[1] == m[1] && mVal[2] == m[2];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Matrix3::operator!=(const Matrix3& m) const
{
    return !(operator==(m));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 Matrix3::Identity()
{
    return Matrix3
    (
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 0.0f, 1.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 Matrix3::Scale(float scaleX, float scaleY, float scaleZ)
{
    return Matrix3
    (
        Vector3(scaleX, 0.0f, 0.0f), 
        Vector3(0.0f, scaleY, 0.0f),
        Vector3(0.0f, 0.0f, scaleZ)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 Matrix3::RotX(float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix3
    (
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(0.0f, cs, sn),
        Vector3(0.0f, -sn, cs)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 Matrix3::RotY(float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix3
    (
        Vector3(cs, 0.0f, sn),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(-sn, 0.0f, cs)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 Matrix3::RotZ(float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix3
    (
        Vector3(cs, sn, 0.0f),
        Vector3(-sn, cs, 0.0f),
        Vector3(0.0f, 0.0f, 1.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 Transpose(const Matrix3& m)
{
    return Matrix3(m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool IsAffine(const Matrix3& m)
{
    return fabsf(LengthSquared(Vector3(m[0][2], m[1][2], m[2][2]) - Vector3(0.0f, 0.0f, 
        1.0f))) < 0.00001f;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 Inverse(const Matrix3& m)
{
    float determinant = Determinant(m);
    NS_ASSERT(fabsf(determinant) > FLT_EPSILON);
    return Inverse(m, determinant);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 Inverse(const Matrix3& m, float determinant)
{
    NS_ASSERT(fabsf(determinant) > FLT_EPSILON);

    return Matrix3(m[1][1] * m[2][2] - m[1][2] * m[2][1],
        m[0][2] * m[2][1] - m[0][1] * m[2][2],
        m[0][1] * m[1][2] - m[0][2] * m[1][1],

        m[1][2] * m[2][0] - m[1][0] * m[2][2],
        m[0][0] * m[2][2] - m[0][2] * m[2][0],
        m[0][2] * m[1][0] - m[0][0] * m[1][2],

        m[1][0] * m[2][1] - m[1][1] * m[2][0],
        m[0][1] * m[2][0] - m[0][0] * m[2][1],
        m[0][0] * m[1][1] - m[0][1] * m[1][0]) / determinant; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline float Determinant(const Matrix3& m)
{
    return 
        (m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1]) - 
        (m[0][0] * m[1][2] * m[2][1] + m[0][1] * m[1][0] * m[2][2] + m[0][2] * m[1][1] * m[2][0]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 PreScale(float scaleX, float scaleY, float scaleZ, const Matrix3& m)
{
    return Matrix3
    (
        m[0][0] * scaleX, m[0][1] * scaleX, m[0][2] * scaleX,
        m[1][0] * scaleY, m[1][1] * scaleY, m[1][2] * scaleY,
        m[2][0] * scaleZ, m[2][1] * scaleZ, m[2][2] * scaleZ
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 PreRotX(float radians, const Matrix3& m)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix3
    (
        m[0][0], m[0][1], m[0][2],
        m[1][0] * cs + m[2][0] * sn, m[1][1] * cs + m[2][1] * sn, m[1][2] * cs + m[2][2] * sn,
        m[2][0] * cs - m[1][0] * sn, m[2][1] * cs - m[1][1] * sn, m[2][2] * cs - m[1][2] * sn
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 PreRotY(float radians, const Matrix3& m)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix3
    (
        m[0][0] * cs + m[2][0] * sn, m[0][1] * cs + m[2][1] * sn, m[0][2] * cs + m[2][2] * sn,
        m[1][0], m[1][1], m[1][2],
        m[2][0] * cs - m[0][0] * sn, m[2][1] * cs - m[0][1] * sn, m[2][2] * cs - m[0][2] * sn
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 PreRotZ(float radians, const Matrix3& m)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix3
    (
        m[0][0] * cs + m[1][0] * sn, m[0][1] * cs + m[1][1] * sn, m[0][2] * cs + m[1][2] * sn,
        m[1][0] * cs - m[0][0] * sn, m[1][1] * cs - m[0][1] * sn, m[1][2] * cs - m[0][2] * sn,
        m[2][0], m[2][1], m[2][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 PostScale(const Matrix3& m, float scaleX, float scaleY, float scaleZ)
{
    return Matrix3
    (
        m[0][0] * scaleX, m[0][1] * scaleY, m[0][2] * scaleZ,
        m[1][0] * scaleX, m[1][1] * scaleY, m[1][2] * scaleZ,
        m[2][0] * scaleX, m[2][1] * scaleY, m[2][2] * scaleZ
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 PostRotX(const Matrix3& m, float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix3
    (
        m[0][0], m[0][1] * cs - m[0][2] * sn, m[0][2] * cs + m[0][1] * sn,
        m[1][0], m[1][1] * cs - m[1][2] * sn, m[1][2] * cs + m[1][1] * sn,
        m[2][0], m[2][1] * cs - m[2][2] * sn, m[2][2] * cs + m[2][1] * sn
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 PostRotY(const Matrix3& m, float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix3
    (
        m[0][0] * cs - m[0][2] * sn, m[0][1], m[0][2] * cs + m[0][0] * sn,
        m[1][0] * cs - m[1][2] * sn, m[1][1], m[1][2] * cs + m[1][0] * sn,
        m[2][0] * cs - m[2][2] * sn, m[2][1], m[2][2] * cs + m[2][0] * sn
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3 PostRotZ(const Matrix3& m, float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix3
    (
        m[0][0] * cs - m[0][1] * sn, m[0][1] * cs + m[0][0] * sn, m[0][2],
        m[1][0] * cs - m[1][1] * sn, m[1][1] * cs + m[1][0] * sn, m[1][2],
        m[2][0] * cs - m[2][1] * sn, m[2][1] * cs + m[2][0] * sn, m[2][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix3 operator*(const Matrix3& m, float v)
{
    return Matrix3(m[0] * v, m[1] * v, m[2] * v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix3 operator*(float v, const Matrix3& m)
{
    return Matrix3(m[0] * v, m[1] * v, m[2] * v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix3 operator/(const Matrix3& m, float v)
{
    return Matrix3(m[0] / v, m[1] / v, m[2] / v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector3 operator*(const Vector3& v, const Matrix3& m)
{
    return Vector3(v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0], 
        v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1],
        v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix3 operator*(const Matrix3& m0, const Matrix3& m1)
{
    return Matrix3
    (
        m0[0][0] * m1[0][0] + m0[0][1] * m1[1][0] + m0[0][2] * m1[2][0],
        m0[0][0] * m1[0][1] + m0[0][1] * m1[1][1] + m0[0][2] * m1[2][1],
        m0[0][0] * m1[0][2] + m0[0][1] * m1[1][2] + m0[0][2] * m1[2][2],

        m0[1][0] * m1[0][0] + m0[1][1] * m1[1][0] + m0[1][2] * m1[2][0],
        m0[1][0] * m1[0][1] + m0[1][1] * m1[1][1] + m0[1][2] * m1[2][1],
        m0[1][0] * m1[0][2] + m0[1][1] * m1[1][2] + m0[1][2] * m1[2][2],

        m0[2][0] * m1[0][0] + m0[2][1] * m1[1][0] + m0[2][2] * m1[2][0],
        m0[2][0] * m1[0][1] + m0[2][1] * m1[1][1] + m0[2][2] * m1[2][1],
        m0[2][0] * m1[0][2] + m0[2][1] * m1[1][2] + m0[2][2] * m1[2][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4::Matrix4(float v00, float v01, float v02, float v03, float v10, float v11,
    float v12, float v13, float v20, float v21, float v22, float v23, float v30, float v31,
    float v32, float v33)
{
    mVal[0] = Vector4(v00, v01, v02, v03);
    mVal[1] = Vector4(v10, v11, v12, v13);
    mVal[2] = Vector4(v20, v21, v22, v23);
    mVal[3] = Vector4(v30, v31, v32, v33);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4::Matrix4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3)
{
    mVal[0] = v0;
    mVal[1] = v1;
    mVal[2] = v2;
    mVal[3] = v3;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4::Matrix4(const float* values)
{
    NS_ASSERT(values != nullptr);
    mVal[0] = Vector4(values[0], values[1], values[2], values[3]);
    mVal[1] = Vector4(values[4], values[5], values[6], values[7]);
    mVal[2] = Vector4(values[8], values[9], values[10], values[11]);
    mVal[3] = Vector4(values[12], values[13], values[14], values[15]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Vector4& Matrix4::operator[](uint32_t i)
{
    NS_ASSERT(i < 4);
    return mVal[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector4& Matrix4::operator[](uint32_t i) const
{
    NS_ASSERT(i < 4);
    return mVal[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const float* Matrix4::GetData() const
{
    return mVal[0].GetData();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4& Matrix4::operator*=(const Matrix4& m)
{
    mVal[0] = Vector4
    (
        mVal[0][0] * m[0][0] + mVal[0][1] * m[1][0] + mVal[0][2] * m[2][0] + mVal[0][3] * m[3][0],
        mVal[0][0] * m[0][1] + mVal[0][1] * m[1][1] + mVal[0][2] * m[2][1] + mVal[0][3] * m[3][1],
        mVal[0][0] * m[0][2] + mVal[0][1] * m[1][2] + mVal[0][2] * m[2][2] + mVal[0][3] * m[3][2],
        mVal[0][0] * m[0][3] + mVal[0][1] * m[1][3] + mVal[0][2] * m[2][3] + mVal[0][3] * m[3][3]
    );

    mVal[1] = Vector4
    (
        mVal[1][0] * m[0][0] + mVal[1][1] * m[1][0] + mVal[1][2] * m[2][0] + mVal[1][3] * m[3][0],
        mVal[1][0] * m[0][1] + mVal[1][1] * m[1][1] + mVal[1][2] * m[2][1] + mVal[1][3] * m[3][1],
        mVal[1][0] * m[0][2] + mVal[1][1] * m[1][2] + mVal[1][2] * m[2][2] + mVal[1][3] * m[3][2],
        mVal[1][0] * m[0][3] + mVal[1][1] * m[1][3] + mVal[1][2] * m[2][3] + mVal[1][3] * m[3][3]
    );

    mVal[2] = Vector4
    (
        mVal[2][0] * m[0][0] + mVal[2][1] * m[1][0] + mVal[2][2] * m[2][0] + mVal[2][3] * m[3][0],
        mVal[2][0] * m[0][1] + mVal[2][1] * m[1][1] + mVal[2][2] * m[2][1] + mVal[2][3] * m[3][1],
        mVal[2][0] * m[0][2] + mVal[2][1] * m[1][2] + mVal[2][2] * m[2][2] + mVal[2][3] * m[3][2],
        mVal[2][0] * m[0][3] + mVal[2][1] * m[1][3] + mVal[2][2] * m[2][3] + mVal[2][3] * m[3][3]
    );

    mVal[3] = Vector4
    (
        mVal[3][0] * m[0][0] + mVal[3][1] * m[1][0] + mVal[3][2] * m[2][0] + mVal[3][3] * m[3][0],
        mVal[3][0] * m[0][1] + mVal[3][1] * m[1][1] + mVal[3][2] * m[2][1] + mVal[3][3] * m[3][1],
        mVal[3][0] * m[0][2] + mVal[3][1] * m[1][2] + mVal[3][2] * m[2][2] + mVal[3][3] * m[3][2],
        mVal[3][0] * m[0][3] + mVal[3][1] * m[1][3] + mVal[3][2] * m[2][3] + mVal[3][3] * m[3][3]
    );

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4& Matrix4::operator*=(float v)
{
    mVal[0] *= v;
    mVal[1] *= v;
    mVal[2] *= v;
    mVal[3] *= v;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4& Matrix4::operator/=(float v)
{
    mVal[0] /= v;
    mVal[1] /= v;
    mVal[2] /= v;
    mVal[3] /= v;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Matrix4::operator==(const Matrix4& m) const
{
    return mVal[0] == m[0] && mVal[1] == m[1] && mVal[2] == m[2] && mVal[3] == m[3];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Matrix4::operator!=(const Matrix4& m) const
{
    return !(operator==(m));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4 Matrix4::Identity()
{
    return Matrix4
    (
        Vector4(1.0f, 0.0f, 0.0f, 0.0f),
        Vector4(0.0f, 1.0f, 0.0f, 0.0f),
        Vector4(0.0f, 0.0f, 1.0f, 0.0f),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4 Matrix4::Scale(float scaleX, float scaleY, float scaleZ)
{
    return Matrix4
    (
        Vector4(scaleX, 0.0f, 0.0f, 0.0f),
        Vector4(0.0f, scaleY, 0.0f, 0.0f),
        Vector4(0.0f, 0.0f, scaleZ, 0.0f),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4 Matrix4::RotX(float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix4
    (
        Vector4(1.0f, 0.0f, 0.0f, 0.0f),
        Vector4(0.0f, cs, sn, 0.0f),
        Vector4(0.0f, -sn, cs, 0.0f),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4 Matrix4::RotY(float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix4
    (
        Vector4(cs, 0.0f, sn, 0.0f),
        Vector4(0.0f, 1.0f, 0.0f, 0.0f),
        Vector4(-sn, 0.0f, cs, 0.0f),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4 Matrix4::RotZ(float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Matrix4
    (
        Vector4(cs, sn, 0.0f, 0.0f),
        Vector4(-sn, cs, 0.0f, 0.0f),
        Vector4(0.0f, 0.0f, 1.0f, 0.0f),
        Vector4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4 Matrix4::PerspectiveFov(float fovY, float aspect, float zNear)
{
    float yScale = 1.0f / (tanf(fovY * 0.5f));

    return Matrix4
    (
        yScale / aspect,   0.0f,       0.0f,      0.0f,
        0.0f,            yScale,       0.0f,      0.0f,
        0.0f,              0.0f,       1.0f,      1.0f,
        0.0f,              0.0f,     -zNear,      0.0f
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4 Matrix4::Viewport(float width, float height)
{
    return Matrix4
    (
        width * 0.5f,   0.0f,           0.0f, 0.0f,
        0.0f,           height * 0.5f,  0.0f, 0.0f,
        0.0f,           0.0f,           1.0f, 0.0f,
        width * 0.5f,   height * 0.5f,  0.0f, 1.0f
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4 Transpose(const Matrix4& m)
{
    return Matrix4
    (
        m[0][0], m[1][0], m[2][0], m[3][0],
        m[0][1], m[1][1], m[2][1], m[3][1],
        m[0][2], m[1][2], m[2][2], m[3][2],
        m[0][3], m[1][3], m[2][3], m[3][3]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool IsAffine(const Matrix4& m)
{
    return fabsf(LengthSquared(Vector4(m[0][3], m[1][3], m[2][3], m[3][3]) - 
        Vector4(0.0f, 0.0f, 0.0f, 1.0f))) < 0.00001f;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool IsIdentityAtZ0(const Matrix4& m)
{
    return IsOne(m[0][0] / m[3][3], 1e-2f) && IsOne(m[1][1] / m[3][3], 1e-2f) &&
           IsZero(m[3][0] / m[3][3], 1e-2f) && IsZero(m[3][1] / m[3][3], 1e-2f) &&
           IsZero(m[0][1]) && IsZero(m[1][0]) && IsZero(m[0][3]) && IsZero(m[1][3]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4 Inverse(const Matrix4& m)
{
    return Inverse(m, Determinant(m));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix4 Inverse(const Matrix4& m, float determinant)
{
    NS_ASSERT(fabsf(determinant) > FLT_EPSILON);
    float invDet = 1.0f / determinant;

    float v0 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
    float v1 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
    float v2 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
    float v3 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
    float v4 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
    float v5 = m[2][2] * m[3][3] - m[2][3] * m[3][2];

    float t00 = v5 * m[1][1] - v4 * m[1][2] + v3 * m[1][3];
    float t10 = -(v5 * m[1][0] - v2 * m[1][2] + v1 * m[1][3]);
    float t20 = v4 * m[1][0] - v2 * m[1][1] + v0 * m[1][3];
    float t30 = -(v3 * m[1][0] - v1 * m[1][1] + v0 * m[1][2]);

    float d00 = t00 * invDet;
    float d10 = t10 * invDet;
    float d20 = t20 * invDet;
    float d30 = t30 * invDet;

    float d01 = -(v5 * m[0][1] - v4 * m[0][2] + v3 * m[0][3]) * invDet;
    float d11 = (v5 * m[0][0] - v2 * m[0][2] + v1 * m[0][3]) * invDet;
    float d21 = -(v4 * m[0][0] - v2 * m[0][1] + v0 * m[0][3]) * invDet;
    float d31 = (v3 * m[0][0] - v1 * m[0][1] + v0 * m[0][2]) * invDet;

    v0 = m[1][0] * m[3][1] - m[1][1] * m[3][0];
    v1 = m[1][0] * m[3][2] - m[1][2] * m[3][0];
    v2 = m[1][0] * m[3][3] - m[1][3] * m[3][0];
    v3 = m[1][1] * m[3][2] - m[1][2] * m[3][1];
    v4 = m[1][1] * m[3][3] - m[1][3] * m[3][1];
    v5 = m[1][2] * m[3][3] - m[1][3] * m[3][2];

    float d02 = (v5 * m[0][1] - v4 * m[0][2] + v3 * m[0][3]) * invDet;
    float d12 = -(v5 * m[0][0] - v2 * m[0][2] + v1 * m[0][3]) * invDet;
    float d22 = (v4 * m[0][0] - v2 * m[0][1] + v0 * m[0][3]) * invDet;
    float d32 = -(v3 * m[0][0] - v1 * m[0][1] + v0 * m[0][2]) * invDet;

    v0 = m[2][1] * m[1][0] - m[2][0] * m[1][1];
    v1 = m[2][2] * m[1][0] - m[2][0] * m[1][2];
    v2 = m[2][3] * m[1][0] - m[2][0] * m[1][3];
    v3 = m[2][2] * m[1][1] - m[2][1] * m[1][2];
    v4 = m[2][3] * m[1][1] - m[2][1] * m[1][3];
    v5 = m[2][3] * m[1][2] - m[2][2] * m[1][3];

    float d03 = -(v5 * m[0][1] - v4 * m[0][2] + v3 * m[0][3]) * invDet;
    float d13 = (v5 * m[0][0] - v2 * m[0][2] + v1 * m[0][3]) * invDet;
    float d23 = -(v4 * m[0][0] - v2 * m[0][1] + v0 * m[0][3]) * invDet;
    float d33 = (v3 * m[0][0] - v1 * m[0][1] + v0 * m[0][2]) * invDet;

    return Matrix4(d00, d01, d02, d03, d10, d11, d12, d13, d20, d21, d22, d23, d30, d31, 
        d32, d33);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline float Determinant(const Matrix4& m)
{
    float v0 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
    float v1 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
    float v2 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
    float v3 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
    float v4 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
    float v5 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
    
    float t00 = v5 * m[1][1] - v4 * m[1][2] + v3 * m[1][3];
    float t10 = -(v5 * m[1][0] - v2 * m[1][2] + v1 * m[1][3]);
    float t20 = v4 * m[1][0] - v2 * m[1][1] + v0 * m[1][3];
    float t30 = -(v3 * m[1][0] - v1 * m[1][1] + v0 * m[1][2]);
    
    return t00 * m[0][0] + t10 * m[0][1] + t20 * m[0][2] + t30 * m[0][3];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix4 operator*(const Matrix4& m, float v)
{
    return Matrix4(m[0] * v, m[1] * v, m[2] * v, m[3] * v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix4 operator*(float v, const Matrix4& m)
{
    return Matrix4(m[0] * v, m[1] * v, m[2] * v, m[3] * v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix4 operator/(const Matrix4& m, float v)
{
    return Matrix4(m[0] / v, m[1] / v, m[2] / v, m[3] / v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector4 operator*(const Vector4& v, const Matrix4& m)
{
    return Vector4
    (
        v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0] + v[3] * m[3][0],
        v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1] + v[3] * m[3][1],
        v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] + v[3] * m[3][2],
        v[0] * m[0][3] + v[1] * m[1][3] + v[2] * m[2][3] + v[3] * m[3][3]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix4 operator*(const Matrix4& m0, const Matrix4& m1)
{
    return Matrix4
    (
        m0[0][0] * m1[0][0] + m0[0][1] * m1[1][0] + m0[0][2] * m1[2][0] + m0[0][3] * m1[3][0],
        m0[0][0] * m1[0][1] + m0[0][1] * m1[1][1] + m0[0][2] * m1[2][1] + m0[0][3] * m1[3][1],
        m0[0][0] * m1[0][2] + m0[0][1] * m1[1][2] + m0[0][2] * m1[2][2] + m0[0][3] * m1[3][2],
        m0[0][0] * m1[0][3] + m0[0][1] * m1[1][3] + m0[0][2] * m1[2][3] + m0[0][3] * m1[3][3],

        m0[1][0] * m1[0][0] + m0[1][1] * m1[1][0] + m0[1][2] * m1[2][0] + m0[1][3] * m1[3][0],
        m0[1][0] * m1[0][1] + m0[1][1] * m1[1][1] + m0[1][2] * m1[2][1] + m0[1][3] * m1[3][1],
        m0[1][0] * m1[0][2] + m0[1][1] * m1[1][2] + m0[1][2] * m1[2][2] + m0[1][3] * m1[3][2],
        m0[1][0] * m1[0][3] + m0[1][1] * m1[1][3] + m0[1][2] * m1[2][3] + m0[1][3] * m1[3][3],

        m0[2][0] * m1[0][0] + m0[2][1] * m1[1][0] + m0[2][2] * m1[2][0] + m0[2][3] * m1[3][0],
        m0[2][0] * m1[0][1] + m0[2][1] * m1[1][1] + m0[2][2] * m1[2][1] + m0[2][3] * m1[3][1],
        m0[2][0] * m1[0][2] + m0[2][1] * m1[1][2] + m0[2][2] * m1[2][2] + m0[2][3] * m1[3][2],
        m0[2][0] * m1[0][3] + m0[2][1] * m1[1][3] + m0[2][2] * m1[2][3] + m0[2][3] * m1[3][3],

        m0[3][0] * m1[0][0] + m0[3][1] * m1[1][0] + m0[3][2] * m1[2][0] + m0[3][3] * m1[3][0],
        m0[3][0] * m1[0][1] + m0[3][1] * m1[1][1] + m0[3][2] * m1[2][1] + m0[3][3] * m1[3][1],
        m0[3][0] * m1[0][2] + m0[3][1] * m1[1][2] + m0[3][2] * m1[2][2] + m0[3][3] * m1[3][2],
        m0[3][0] * m1[0][3] + m0[3][1] * m1[1][3] + m0[3][2] * m1[2][3] + m0[3][3] * m1[3][3]
    );
}

}
