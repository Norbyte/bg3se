////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_MATH_H__
#define __CORE_MATH_H__


#include <NsCore/Noesis.h>
#include <float.h>


namespace Noesis
{

union IntToFloat
{
    uint32_t u;
    float f;
};

#define FLT_INF (Noesis::IntToFloat{0x7f800000}.f)
#define FLT_NAN (Noesis::IntToFloat{0x7fc00000}.f)

union IntToDouble
{
    uint64_t u;
    double f;
};

#define DBL_INF (Noesis::IntToDouble{0x7ff0000000000000}.f)
#define DBL_NAN (Noesis::IntToDouble{0x7ff8000000000000}.f)

const float Pi = 3.14159265358979323846264338327950288419716939937511f;
const float OneOverPi = 1.0f / Pi;
const float E = 2.71828182845904523536028747135266249775724709369996f;
const float TwoPi = 2.0f * Pi;
const float FourPi = 4.0f * Pi;
const float HalfPi = Pi / 2.0f;
const float SqrtPi = 1.77245385090551602729816748334114518279754945612239f;
const float DegToRad = Pi / 180;
const float RadToDeg = 180 / Pi;

/// Tests if a number is a power of 2
constexpr bool IsPow2(uint32_t x);

/// Gets next power of 2
constexpr uint32_t NextPow2(uint32_t x);

/// Gets previous power of 2
constexpr uint32_t PrevPow2(uint32_t x);

/// Gets the integer floor log base 2
uint32_t Log2(uint32_t x);

/// Returns whether the specified value is close to 1 within the order of epsilon
bool IsOne(float val, float epsilon = 10.0f * FLT_EPSILON);

/// Returns whether the specified value is close to 0 within the order of epsilon 
bool IsZero(float val, float epsilon = 10.0f * FLT_EPSILON);

/// Returns whether or not two floats are within epsilon of each other
bool AreClose(float a, float b, float epsilon = 10.0f * FLT_EPSILON);

/// Returns whether or not the first number is greater than the second number
bool GreaterThan(float a, float b, float epsilon = 10.0f * FLT_EPSILON);

/// Returns whether or not the first number is greater than or close to the second number
bool GreaterThanOrClose(float a, float b, float epsilon = 10.0f * FLT_EPSILON);

/// Returns whether or not the first number is less than the second number
bool LessThan(float a, float b, float epsilon = 10.0f * FLT_EPSILON);

/// Returns whether or not the first number is less than or close to the second number
bool LessThanOrClose(float a, float b, float epsilon = 10.0f * FLT_EPSILON);

/// Tests if float is NaN
bool IsNaN(float val);

/// Tests if double is NaN
bool IsNaN(double val);

/// Tests if float is positive infinity or negative infinity
bool IsInfinity(float val);

/// Tests if double is positive infinity or negative infinity
bool IsInfinity(double val);

/// Tests if float is positive infinity
bool IsPositiveInfinity(float val);

/// Tests if double is positive infinity
bool IsPositiveInfinity(double val);

/// Tests if float is negative infinity
bool IsNegativeInfinity(float val);

/// Tests if double is negative infinity
bool IsNegativeInfinity(double val);

/// Float to Signed Int conversion with truncation towards zero
int Trunc(float val);

/// Float to Signed Int conversion rounding to nearest integer
int Round(float val);

/// Calculates the floor of a value
float Floor(float val);

/// Calculates the ceil of a value
float Ceil(float val);

/// Max returns the greater of its two arguments
template<class T> constexpr const T& Max(const T& a, const T& b);

/// Min returns the lesser of its two arguments
template<class T> constexpr const T& Min(const T& a, const T& b);

/// Clips a value between a minimum and a maximum
template<class T> const T& Clip(const T& val, const T& minVal, const T& maxVal);

/// Returns a linear interpolation between x and y (0 <= t <= 1)
float Lerp(float x, float y, float t);

}

#include <NsCore/Math.inl>

#endif
