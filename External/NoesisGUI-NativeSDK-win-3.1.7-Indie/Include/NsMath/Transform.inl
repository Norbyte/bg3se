////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Math.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2::Transform2(float v00, float v01, float v10, float v11, float v20, float v21)
{
    mVal[0] = Vector2(v00, v01);
    mVal[1] = Vector2(v10, v11);
    mVal[2] = Vector2(v20, v21);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2::Transform2(const Vector2& v0, const Vector2& v1, const Vector2& v2)
{
    mVal[0] = v0;
    mVal[1] = v1;
    mVal[2] = v2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2::Transform2(const float* values)
{
    NS_ASSERT(values != nullptr);
    mVal[0] = Vector2(values[0], values[1]);
    mVal[1] = Vector2(values[2], values[3]);
    mVal[2] = Vector2(values[4], values[5]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Vector2& Transform2::operator[](uint32_t i)
{
    NS_ASSERT(i < 3);
    return mVal[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector2& Transform2::operator[](uint32_t i) const
{
    NS_ASSERT(i < 3);
    return mVal[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const float* Transform2::GetData() const
{
    return mVal[0].GetData();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix2& Transform2::Rotation()
{
    return *(Matrix2*)(mVal[0].GetData());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix2& Transform2::Rotation() const
{
    return *(const Matrix2*)(mVal[0].GetData());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Vector2& Transform2::Translation()
{
    return mVal[2];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector2& Transform2::Translation() const
{
    return mVal[2];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix3 Transform2::ToMatrix3() const
{
    return Matrix3(Vector3(mVal[0], 0.0f), Vector3(mVal[1], 0.0f), Vector3(mVal[2], 1.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2& Transform2::operator*=(const Transform2& m)
{
    mVal[0] = Vector2(mVal[0][0] * m[0][0] + mVal[0][1] * m[1][0],
        mVal[0][0] * m[0][1] + mVal[0][1] * m[1][1]);

    mVal[1] = Vector2(mVal[1][0] * m[0][0] + mVal[1][1] * m[1][0],
        mVal[1][0] * m[0][1] + mVal[1][1] * m[1][1]);

    mVal[2] = Vector2(mVal[2][0] * m[0][0] + mVal[2][1] * m[1][0] + m[2][0],
        mVal[2][0] * m[0][1] + mVal[2][1] * m[1][1] + m[2][1]);

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2& Transform2::operator*=(float v)
{
    mVal[0] *= v;
    mVal[1] *= v;
    mVal[2] *= v;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2& Transform2::operator/=(float v)
{
    mVal[0] /= v;
    mVal[1] /= v;
    mVal[2] /= v;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Transform2::RotateAt(float radians, float centerX, float centerY)
{
    *this = PostTrans(*this, -centerX, -centerY);
    *this = PostRot(*this, radians);
    *this = PostTrans(*this, centerX, centerY);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Transform2::ScaleAt(float scaleX, float scaleY, float centerX, float centerY)
{
    *this = PostTrans(*this, -centerX, -centerY);
    *this = PostScale(*this, scaleX, scaleY);
    *this = PostTrans(*this, centerX, centerY);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Transform2::Translate(float dx, float dy)
{
    *this = PostTrans(*this, dx, dy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Transform2::operator==(const Transform2& m) const
{
    return mVal[0] == m[0] && mVal[1] == m[1] && mVal[2] == m[2];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Transform2::operator!=(const Transform2& m) const
{
    return !(operator==(m));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 Transform2::Identity()
{
    return Transform2(Vector2(1.0f, 0.0f), Vector2(0.0f, 1.0f), Vector2(0.0f, 0.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 Transform2::Scale(float scaleX, float scaleY)
{
    return Transform2(Vector2(scaleX, 0.0f), Vector2(0.0f, scaleY), Vector2(0.0f, 0.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 Transform2::Trans(float transX, float transY)
{
    return Transform2(Vector2(1.0f, 0.0f), Vector2(0.0f, 1.0f), Vector2(transX, transY));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 Transform2::Rot(float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Transform2(Vector2(cs, sn), Vector2(-sn, cs), Vector2(0.0f, 0.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 Transform2::ShearXY(float shear)
{
    return Transform2(Vector2(1.0f, 0.0f), Vector2(shear, 1.0f), Vector2(0.0f, 0.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 Transform2::ShearYX(float shear)
{
    return Transform2(Vector2(1.0f, shear), Vector2(0.0f, 1.0f), Vector2(0.0f, 0.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 Transform2::Skew(float radiansX, float radiansY)
{
    float cosX = cosf(radiansX);
    float cosY = cosf(radiansY);
    float skewX = IsZero(cosX) ? FLT_INF : sinf(radiansX) / cosX;
    float skewY = IsZero(cosY) ? FLT_INF : sinf(radiansY) / cosY;

    return Transform2(Vector2(1.0f, skewY), Vector2(skewX, 1.0f), Vector2(0.0f, 0.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 Inverse(const Transform2& m)
{
    float determinant = m[0][0] * m[1][1] - m[0][1] * m[1][0];

    float xx = m[1][1] / determinant;
    float xy = -m[0][1] / determinant;

    float yx = -m[1][0] / determinant;
    float yy = m[0][0] / determinant;

    float zx = -m[2][0] * xx - m[2][1] * yx;
    float zy = -m[2][0] * xy - m[2][1] * yy;

    return Transform2(xx, xy, yx, yy, zx, zy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 Inverse(const Transform2& m, float determinant)
{
    NS_ASSERT(!IsZero(determinant));

    float xx = m[1][1] / determinant;
    float xy = -m[0][1] / determinant;

    float yx = -m[1][0] / determinant;
    float yy = m[0][0] / determinant;

    float zx = -m[2][0] * xx - m[2][1] * yx;
    float zy = -m[2][0] * xy - m[2][1] * yy;

    return Transform2(xx, xy, yx, yy, zx, zy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 PreScale(float scaleX, float scaleY, const Transform2& m)
{
    return Transform2
    (
        m[0][0] * scaleX, m[0][1] * scaleX,
        m[1][0] * scaleY, m[1][1] * scaleY,
        m[2][0], m[2][1]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 PreTrans(float dx, float dy, const Transform2& m)
{
    return Transform2
    (
        m[0][0], m[0][1],
        m[1][0], m[1][1],
        m[2][0] + m[0][0] * dx + m[1][0] * dy, m[2][1] + m[0][1] * dx + m[1][1] * dy
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 PreRot(float radians, const Transform2& m)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Transform2
    (
        m[0][0] * cs + m[1][0] * sn, m[0][1] * cs + m[1][1] * sn,
        m[1][0] * cs - m[0][0] * sn, m[1][1] * cs - m[0][1] * sn,
        m[2][0], m[2][1]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 PreSkew(float radiansX, float radiansY, const Transform2& m)
{
    float cosX = cosf(radiansX);
    float cosY = cosf(radiansY);
    float skewX = IsZero(cosX) ? FLT_INF : sinf(radiansX) / cosX;
    float skewY = IsZero(cosY) ? FLT_INF : sinf(radiansY) / cosY;

    return Transform2
    (
        m[0][0] + m[1][0] * skewY, m[0][1] + m[1][1] * skewY,
        m[1][0] + m[0][0] * skewX, m[1][1] + m[0][1] * skewX,
        m[2][0], m[2][1]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 PostScale(const Transform2& m, float scaleX, float scaleY)
{
    return Transform2
    (
        m[0][0] * scaleX, m[0][1] * scaleY,
        m[1][0] * scaleX, m[1][1] * scaleY,
        m[2][0] * scaleX, m[2][1] * scaleY
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 PostTrans(const Transform2& m, float transX, float transY)
{
    return Transform2
    (
        m[0][0], m[0][1],
        m[1][0], m[1][1],
        m[2][0] + transX, m[2][1] + transY
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 PostRot(const Transform2& m, float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Transform2
    (
        m[0][0] * cs - m[0][1] * sn, m[0][1] * cs + m[0][0] * sn,
        m[1][0] * cs - m[1][1] * sn, m[1][1] * cs + m[1][0] * sn,
        m[2][0] * cs - m[2][1] * sn, m[2][1] * cs + m[2][0] * sn
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform2 PostSkew(const Transform2& m, float radiansX, float radiansY)
{
    float cosX = cosf(radiansX);
    float cosY = cosf(radiansY);
    float skewX = IsZero(cosX) ? FLT_INF : sinf(radiansX) / cosX;
    float skewY = IsZero(cosY) ? FLT_INF : sinf(radiansY) / cosY;

    return Transform2
    (
        m[0][0] + m[0][1] * skewX, m[0][1] + m[0][0] * skewY,
        m[1][0] + m[1][1] * skewX, m[1][1] + m[1][0] * skewY,
        m[2][0] + m[2][1] * skewX, m[2][1] + m[2][0] * skewY
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector2 PointTransform(const Vector2& v, const Transform2& m)
{
    return Vector2
    (
        v[0] * m[0][0] + v[1] * m[1][0] + m[2][0],
        v[0] * m[0][1] + v[1] * m[1][1] + m[2][1]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector2 VectorTransform(const Vector2& v, const Transform2& m)
{
    return Vector2(v[0] * m[0][0] + v[1] * m[1][0], v[0] * m[0][1] + v[1] * m[1][1]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline float ScaleX(const Transform2& m)
{
    return Length(m[0]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline float ScaleY(const Transform2& m)
{
    return Length(m[1]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline float TransX(const Transform2& m)
{
    return m[2][0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline float TransY(const Transform2& m)
{
    return m[2][1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline float MaxScale(const Transform2& m)
{
    return Max(Length(m[0]), Length(m[1]));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool IsRotating(const Transform2& m)
{
    // Note that this is not very precise when transform has shearing 
    return !IsZero(m[0][1]) || !IsZero(m[1][0]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector2 operator*(const Vector2& v, const Transform2& m)
{
    return PointTransform(v, m);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Transform2 operator*(const Transform2& m, float v)
{
    return Transform2(m[0] * v, m[1] * v, m[2] * v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Transform2 operator*(float v, const Transform2& m)
{
    return Transform2(m[0] * v, m[1] * v, m[2] * v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Transform2 operator/(const Transform2& m, float v)
{
    return Transform2(m[0] / v, m[1] / v, m[2] / v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector3 operator*(const Vector3& v, const Transform2& m)
{
    return Vector3(v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0],
        v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1], v[2]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Transform2 operator*(const Transform2& m0, const Transform2& m1)
{
    return Transform2
    (
        m0[0][0] * m1[0][0] + m0[0][1] * m1[1][0],
        m0[0][0] * m1[0][1] + m0[0][1] * m1[1][1],
        m0[1][0] * m1[0][0] + m0[1][1] * m1[1][0],
        m0[1][0] * m1[0][1] + m0[1][1] * m1[1][1],
        m0[2][0] * m1[0][0] + m0[2][1] * m1[1][0] + m1[2][0],
        m0[2][0] * m1[0][1] + m0[2][1] * m1[1][1] + m1[2][1]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix3 operator*(const Transform2& m0, const Matrix3& m1)
{
    return Matrix3
    (
        m0[0][0] * m1[0][0] + m0[0][1] * m1[1][0],
        m0[0][0] * m1[0][1] + m0[0][1] * m1[1][1],
        m0[0][0] * m1[0][2] + m0[0][1] * m1[1][2],
        m0[1][0] * m1[0][0] + m0[1][1] * m1[1][0],
        m0[1][0] * m1[0][1] + m0[1][1] * m1[1][1],
        m0[1][0] * m1[0][2] + m0[1][1] * m1[1][2],
        m0[2][0] * m1[0][0] + m0[2][1] * m1[1][0] + m1[2][0],
        m0[2][0] * m1[0][1] + m0[2][1] * m1[1][1] + m1[2][1],
        m0[2][0] * m1[0][2] + m0[2][1] * m1[1][2] + m1[2][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix3 operator*(const Matrix3& m0, const Transform2& m1)
{
    return Matrix3
    (
        m0[0][0] * m1[0][0] + m0[0][1] * m1[1][0] + m0[0][2] * m1[2][0],
        m0[0][0] * m1[0][1] + m0[0][1] * m1[1][1] + m0[0][2] * m1[2][1],
        m0[0][2],
        m0[1][0] * m1[0][0] + m0[1][1] * m1[1][0] + m0[1][2] * m1[2][0],
        m0[1][0] * m1[0][1] + m0[1][1] * m1[1][1] + m0[1][2] * m1[2][1],
        m0[1][2],
        m0[2][0] * m1[0][0] + m0[2][1] * m1[1][0] + m0[2][2] * m1[2][0],
        m0[2][0] * m1[0][1] + m0[2][1] * m1[1][1] + m0[2][2] * m1[2][1],
        m0[2][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3::Transform3(float v00, float v01, float v02, float v10, float v11, float v12,
    float v20, float v21, float v22, float v30, float v31, float v32)
{
    mVal[0] = Vector3(v00, v01, v02);
    mVal[1] = Vector3(v10, v11, v12);
    mVal[2] = Vector3(v20, v21, v22);
    mVal[3] = Vector3(v30, v31, v32);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3::Transform3(const Vector3& v0, const Vector3& v1, const Vector3& v2, 
    const Vector3& v3)
{
    mVal[0] = v0;
    mVal[1] = v1;
    mVal[2] = v2;
    mVal[3] = v3;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3::Transform3(const float* values)
{
    NS_ASSERT(values != nullptr);
    mVal[0] = Vector3(values[0], values[1], values[2]);
    mVal[1] = Vector3(values[3], values[4], values[5]);
    mVal[2] = Vector3(values[6], values[7], values[8]);
    mVal[3] = Vector3(values[9], values[10], values[11]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Vector3& Transform3::operator[](uint32_t i)
{
    NS_ASSERT(i < 4);
    return mVal[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector3& Transform3::operator[](uint32_t i) const
{
    NS_ASSERT(i < 4);
    return mVal[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const float* Transform3::GetData() const
{
    return mVal[0].GetData();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Matrix3& Transform3::Rotation()
{
    return *(Matrix3*)(mVal[0].GetData());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix3& Transform3::Rotation() const
{
    return *(const Matrix3*)(mVal[0].GetData());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Vector3& Transform3::Translation()
{
    return mVal[3];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector3& Transform3::Translation() const
{
    return mVal[3];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix4 Transform3::ToMatrix4() const
{
    return Matrix4
    (
        Vector4(mVal[0], 0.0f),
        Vector4(mVal[1], 0.0f),
        Vector4(mVal[2], 0.0f),
        Vector4(mVal[3], 1.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3& Transform3::operator*=(const Transform3& m)
{
    mVal[0] = Vector3(mVal[0][0] * m[0][0] + mVal[0][1] * m[1][0] + mVal[0][2] * m[2][0],
        mVal[0][0] * m[0][1] + mVal[0][1] * m[1][1] + mVal[0][2] * m[2][1],
        mVal[0][0] * m[0][2] + mVal[0][1] * m[1][2] + mVal[0][2] * m[2][2]);

    mVal[1] = Vector3(mVal[1][0] * m[0][0] + mVal[1][1] * m[1][0] + mVal[1][2] * m[2][0],
        mVal[1][0] * m[0][1] + mVal[1][1] * m[1][1] + mVal[1][2] * m[2][1],
        mVal[1][0] * m[0][2] + mVal[1][1] * m[1][2] + mVal[1][2] * m[2][2]);

    mVal[2] = Vector3(mVal[2][0] * m[0][0] + mVal[2][1] * m[1][0] + mVal[2][2] * m[2][0],
        mVal[2][0] * m[0][1] + mVal[2][1] * m[1][1] + mVal[2][2] * m[2][1],
        mVal[2][0] * m[0][2] + mVal[2][1] * m[1][2] + mVal[2][2] * m[2][2]);

    mVal[3] = Vector3(mVal[3][0] * m[0][0] + mVal[3][1] * m[1][0] + mVal[3][2] * m[2][0] + m[3][0],
        mVal[3][0] * m[0][1] + mVal[3][1] * m[1][1] + mVal[3][2] * m[2][1] + m[3][1],
        mVal[3][0] * m[0][2] + mVal[3][1] * m[1][2] + mVal[3][2] * m[2][2] + m[3][2]
    );

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3& Transform3::operator*=(float v)
{
    mVal[0] *= v;
    mVal[1] *= v;
    mVal[2] *= v;
    mVal[3] *= v;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3& Transform3::operator/=(float v)
{
    mVal[0] /= v;
    mVal[1] /= v;
    mVal[2] /= v;
    mVal[3] /= v;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Transform3::operator==(const Transform3& m) const
{
    return mVal[0] == m[0] && mVal[1] == m[1] && mVal[2] == m[2] && mVal[3] == m[3];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Transform3::operator!=(const Transform3& m) const
{
    return !(operator==(m));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 Transform3::Identity()
{
    return Transform3
    (
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 0.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 Transform3::Scale(float scaleX, float scaleY, float scaleZ)
{
    return Transform3
    (
        Vector3(scaleX, 0.0f, 0.0f),
        Vector3(0.0f, scaleY, 0.0f),
        Vector3(0.0f, 0.0f, scaleZ),
        Vector3(0.0f, 0.0f, 0.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 Transform3::Trans(float transX, float transY, float transZ)
{
    return Transform3
    (
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(transX, transY, transZ)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 Transform3::RotX(float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);
        
    return Transform3
    (
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(0.0f, cs, sn),
        Vector3(0.0f, -sn, cs),
        Vector3(0.0f, 0.0f, 0.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 Transform3::RotY(float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);
        
    return Transform3
    (
        Vector3(cs, 0.0f, sn), 
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(-sn, 0.0f, cs),
        Vector3(0.0f, 0.0f, 0.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 Transform3::RotZ(float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);
        
    return Transform3
    (
        Vector3(cs, sn, 0.0f), 
        Vector3(-sn, cs, 0.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 0.0f)
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 Inverse(const Transform3& m)
{
    Matrix3 rotInv = Inverse(Matrix3(m[0], m[1], m[2]));

    return Transform3
    (
        rotInv[0][0], rotInv[0][1], rotInv[0][2],
        rotInv[1][0], rotInv[1][1], rotInv[1][2],
        rotInv[2][0], rotInv[2][1], rotInv[2][2],
        -m[3][0] * rotInv[0][0] - m[3][1] * rotInv[1][0] - m[3][2] * rotInv[2][0],
        -m[3][0] * rotInv[0][1] - m[3][1] * rotInv[1][1] - m[3][2] * rotInv[2][1],
        -m[3][0] * rotInv[0][2] - m[3][1] * rotInv[1][2] - m[3][2] * rotInv[2][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 PreScale(float scaleX, float scaleY, float scaleZ, const Transform3& m)
{
    return Transform3
    (
        m[0][0] * scaleX, m[0][1] * scaleX, m[0][2] * scaleX,
        m[1][0] * scaleY, m[1][1] * scaleY, m[1][2] * scaleY,
        m[2][0] * scaleZ, m[2][1] * scaleZ, m[2][2] * scaleZ,
        m[3][0], m[3][1], m[3][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 PreTrans(float transX, float transY, float transZ, const Transform3& m)
{
    return Transform3
    (
        m[0][0], m[0][1], m[0][2],
        m[1][0], m[1][1], m[1][2],
        m[2][0], m[2][1], m[2][2],
        m[3][0] + m[0][0] * transX + m[1][0] * transY + m[2][0] * transZ,
        m[3][1] + m[0][1] * transX + m[1][1] * transY + m[2][1] * transZ,
        m[3][2] + m[0][2] * transX + m[1][2] * transY + m[2][2] * transZ
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 PreRotX(float radians, const Transform3& m)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Transform3
    (
        m[0][0], m[0][1], m[0][2],
        m[1][0] * cs + m[2][0] * sn, m[1][1] * cs + m[2][1] * sn, m[1][2] * cs + m[2][2] * sn,
        m[2][0] * cs - m[1][0] * sn, m[2][1] * cs - m[1][1] * sn, m[2][2] * cs - m[1][2] * sn,
        m[3][0], m[3][1], m[3][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 PreRotY(float radians, const Transform3& m)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Transform3
    (
        m[0][0] * cs + m[2][0] * sn, m[0][1] * cs + m[2][1] * sn, m[0][2] * cs + m[2][2] * sn,
        m[1][0], m[1][1], m[1][2],
        m[2][0] * cs - m[0][0] * sn, m[2][1] * cs - m[0][1] * sn, m[2][2] * cs - m[0][2] * sn,
        m[3][0], m[3][1], m[3][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 PreRotZ(float radians, const Transform3& m)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Transform3
    (
        m[0][0] * cs + m[1][0] * sn, m[0][1] * cs + m[1][1] * sn, m[0][2] * cs + m[1][2] * sn,
        m[1][0] * cs - m[0][0] * sn, m[1][1] * cs - m[0][1] * sn, m[1][2] * cs - m[0][2] * sn,
        m[2][0], m[2][1], m[2][2],
        m[3][0], m[3][1], m[3][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 PostScale(const Transform3& m, float scaleX, float scaleY, float scaleZ)
{
    return Transform3
    (
       m[0][0] * scaleX, m[0][1] * scaleY, m[0][2] * scaleZ,
       m[1][0] * scaleX, m[1][1] * scaleY, m[1][2] * scaleZ,
       m[2][0] * scaleX, m[2][1] * scaleY, m[2][2] * scaleZ,
       m[3][0] * scaleX, m[3][1] * scaleY, m[3][2] * scaleZ
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 PostTrans(const Transform3& m, float transX, float transY, float transZ)
{
    return Transform3
    (
        m[0][0], m[0][1], m[0][2],
        m[1][0], m[1][1], m[1][2],
        m[2][0], m[2][1], m[2][2],
        m[3][0] + transX, m[3][1] + transY, m[3][2] + transZ
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 PostRotX(const Transform3& m, float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Transform3
    (
       m[0][0], m[0][1] * cs - m[0][2] * sn, m[0][2] * cs + m[0][1] * sn,
       m[1][0], m[1][1] * cs - m[1][2] * sn, m[1][2] * cs + m[1][1] * sn,
       m[2][0], m[2][1] * cs - m[2][2] * sn, m[2][2] * cs + m[2][1] * sn,
       m[3][0], m[3][1] * cs - m[3][2] * sn, m[3][2] * cs + m[3][1] * sn
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 PostRotY(const Transform3& m, float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Transform3
    (
        m[0][0] * cs - m[0][2] * sn, m[0][1], m[0][2] * cs + m[0][0] * sn,
        m[1][0] * cs - m[1][2] * sn, m[1][1], m[1][2] * cs + m[1][0] * sn,
        m[2][0] * cs - m[2][2] * sn, m[2][1], m[2][2] * cs + m[2][0] * sn,
        m[3][0] * cs - m[3][2] * sn, m[3][1], m[3][2] * cs + m[3][0] * sn
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline Transform3 PostRotZ(const Transform3& m, float radians)
{
    float cs = cosf(radians);
    float sn = sinf(radians);

    return Transform3
    (
        m[0][0] * cs - m[0][1] * sn, m[0][1] * cs + m[0][0] * sn, m[0][2],
        m[1][0] * cs - m[1][1] * sn, m[1][1] * cs + m[1][0] * sn, m[1][2],
        m[2][0] * cs - m[2][1] * sn, m[2][1] * cs + m[2][0] * sn, m[2][2],
        m[3][0] * cs - m[3][1] * sn, m[3][1] * cs + m[3][0] * sn, m[3][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector3 PointTransform(const Vector3& v, const Transform3& m)
{
    return Vector3
    (
        v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0] + m[3][0],
        v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1] + m[3][1],
        v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] + m[3][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector3 VectorTransform(const Vector3& v, const Transform3& m)
{
    return Vector3
    (
        v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0],
        v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1],
        v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector3 operator*(const Vector3& v, const Transform3& m)
{
    return PointTransform(v, m);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Transform3 operator*(const Transform3& m, float v)
{
    return Transform3(m[0] * v, m[1] * v, m[2] * v, m[3] * v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Transform3 operator*(float v, const Transform3& m)
{
    return Transform3(m[0] * v, m[1] * v, m[2] * v, m[3] * v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Transform3 operator/(const Transform3& m, float v)
{
    return Transform3(m[0] / v, m[1] / v, m[2] / v, m[3] / v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Vector4 operator*(const Vector4& v, const Transform3& m)
{
    return Vector4
    (
        v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0] + v[3] * m[3][0],
        v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1] + v[3] * m[3][1],
        v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] + v[3] * m[3][2], v[3]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Transform3 operator*(const Transform3& m0, const Transform3& m1)
{
    return Transform3
    (
        m0[0][0] * m1[0][0] + m0[0][1] * m1[1][0] + m0[0][2] * m1[2][0],
        m0[0][0] * m1[0][1] + m0[0][1] * m1[1][1] + m0[0][2] * m1[2][1],
        m0[0][0] * m1[0][2] + m0[0][1] * m1[1][2] + m0[0][2] * m1[2][2],

        m0[1][0] * m1[0][0] + m0[1][1] * m1[1][0] + m0[1][2] * m1[2][0],
        m0[1][0] * m1[0][1] + m0[1][1] * m1[1][1] + m0[1][2] * m1[2][1],
        m0[1][0] * m1[0][2] + m0[1][1] * m1[1][2] + m0[1][2] * m1[2][2],

        m0[2][0] * m1[0][0] + m0[2][1] * m1[1][0] + m0[2][2] * m1[2][0],
        m0[2][0] * m1[0][1] + m0[2][1] * m1[1][1] + m0[2][2] * m1[2][1],
        m0[2][0] * m1[0][2] + m0[2][1] * m1[1][2] + m0[2][2] * m1[2][2],

        m0[3][0] * m1[0][0] + m0[3][1] * m1[1][0] + m0[3][2] * m1[2][0] + m1[3][0],
        m0[3][0] * m1[0][1] + m0[3][1] * m1[1][1] + m0[3][2] * m1[2][1] + m1[3][1],
        m0[3][0] * m1[0][2] + m0[3][1] * m1[1][2] + m0[3][2] * m1[2][2] + m1[3][2]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix4 operator*(const Transform3& m0, const Matrix4& m1)
{
    return Matrix4
    (
        m0[0][0] * m1[0][0] + m0[0][1] * m1[1][0] + m0[0][2] * m1[2][0],
        m0[0][0] * m1[0][1] + m0[0][1] * m1[1][1] + m0[0][2] * m1[2][1],
        m0[0][0] * m1[0][2] + m0[0][1] * m1[1][2] + m0[0][2] * m1[2][2],
        m0[0][0] * m1[0][3] + m0[0][1] * m1[1][3] + m0[0][2] * m1[2][3],

        m0[1][0] * m1[0][0] + m0[1][1] * m1[1][0] + m0[1][2] * m1[2][0],
        m0[1][0] * m1[0][1] + m0[1][1] * m1[1][1] + m0[1][2] * m1[2][1],
        m0[1][0] * m1[0][2] + m0[1][1] * m1[1][2] + m0[1][2] * m1[2][2],
        m0[1][0] * m1[0][3] + m0[1][1] * m1[1][3] + m0[1][2] * m1[2][3],

        m0[2][0] * m1[0][0] + m0[2][1] * m1[1][0] + m0[2][2] * m1[2][0],
        m0[2][0] * m1[0][1] + m0[2][1] * m1[1][1] + m0[2][2] * m1[2][1],
        m0[2][0] * m1[0][2] + m0[2][1] * m1[1][2] + m0[2][2] * m1[2][2],
        m0[2][0] * m1[0][3] + m0[2][1] * m1[1][3] + m0[2][2] * m1[2][3],

        m0[3][0] * m1[0][0] + m0[3][1] * m1[1][0] + m0[3][2] * m1[2][0] + m1[3][0],
        m0[3][0] * m1[0][1] + m0[3][1] * m1[1][1] + m0[3][2] * m1[2][1] + m1[3][1],
        m0[3][0] * m1[0][2] + m0[3][1] * m1[1][2] + m0[3][2] * m1[2][2] + m1[3][2],
        m0[3][0] * m1[0][3] + m0[3][1] * m1[1][3] + m0[3][2] * m1[2][3] + m1[3][3]
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const Matrix4 operator*(const Matrix4& m0, const Transform3& m1)
{
    return Matrix4
    (
        m0[0][0] * m1[0][0] + m0[0][1] * m1[1][0] + m0[0][2] * m1[2][0] + m0[0][3] * m1[3][0],
        m0[0][0] * m1[0][1] + m0[0][1] * m1[1][1] + m0[0][2] * m1[2][1] + m0[0][3] * m1[3][1],
        m0[0][0] * m1[0][2] + m0[0][1] * m1[1][2] + m0[0][2] * m1[2][2] + m0[0][3] * m1[3][2],
        m0[0][3],

        m0[1][0] * m1[0][0] + m0[1][1] * m1[1][0] + m0[1][2] * m1[2][0] + m0[1][3] * m1[3][0],
        m0[1][0] * m1[0][1] + m0[1][1] * m1[1][1] + m0[1][2] * m1[2][1] + m0[1][3] * m1[3][1],
        m0[1][0] * m1[0][2] + m0[1][1] * m1[1][2] + m0[1][2] * m1[2][2] + m0[1][3] * m1[3][2],
        m0[1][3],

        m0[2][0] * m1[0][0] + m0[2][1] * m1[1][0] + m0[2][2] * m1[2][0] + m0[2][3] * m1[3][0],
        m0[2][0] * m1[0][1] + m0[2][1] * m1[1][1] + m0[2][2] * m1[2][1] + m0[2][3] * m1[3][1],
        m0[2][0] * m1[0][2] + m0[2][1] * m1[1][2] + m0[2][2] * m1[2][2] + m0[2][3] * m1[3][2],
        m0[2][3],

        m0[3][0] * m1[0][0] + m0[3][1] * m1[1][0] + m0[3][2] * m1[2][0] + m0[3][3] * m1[3][0],
        m0[3][0] * m1[0][1] + m0[3][1] * m1[1][1] + m0[3][2] * m1[2][1] + m0[3][3] * m1[3][1],
        m0[3][0] * m1[0][2] + m0[3][1] * m1[1][2] + m0[3][2] * m1[2][2] + m0[3][3] * m1[3][2],
        m0[3][3]
    );
}

}
