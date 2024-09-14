#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>

/// <lua_module>Math</lua_module>
BEGIN_NS(lua::math)

template <class T>
struct TryOpOrFail
{
	static bool Do(lua_State* L, ...)
	{
		return false;
	}

	template <class T1>
	static __forceinline auto Do(lua_State* L, T1 const& a) -> decltype((void)(T::Do(L, a)), void())
	{
		T::Do(L, a);
	}

	template <class T1, class T2>
	static __forceinline auto Do(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)(T::Do(L, a, b)), void())
	{
		T::Do(L, a, b);
	}

	static bool DoInPlace(lua_State* L, ...)
	{
		return false;
	}

	template <class T1>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a) -> decltype((void)(T::DoInPlace(L, a)), void())
	{
		T::DoInPlace(L, a);
	}

	template <class T1, class T2>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)(T::DoInPlace(L, a, b)), void())
	{
		T::DoInPlace(L, a, b);
	}
};

template <class Operation, bool InPlace, class T1>
__forceinline bool TryCallPolymorphicFunc(lua_State* L, T1 const& a)
{
	if constexpr (InPlace) {
		if constexpr (std::is_same_v<decltype(TryOpOrFail<Operation>::DoInPlace(L, a)), bool>) {
			return false;
		} else {
			TryOpOrFail<Operation>::DoInPlace(L, a);
			return true;
		}
	} else {
		if constexpr (std::is_same_v<decltype(TryOpOrFail<Operation>::Do(L, a)), bool>) {
			return false;
		} else {
			TryOpOrFail<Operation>::Do(L, a);
			return true;
		}
	}
}

template <class Operation, bool InPlace, class T1, class T2>
__forceinline bool TryCallPolymorphicFunc(lua_State* L, T1 const& a, T2 const& b)
{
	if constexpr (InPlace) {
		if constexpr (std::is_same_v<decltype(TryOpOrFail<Operation>::DoInPlace(L, a, b)), bool>) {
			return false;
		} else {
			TryOpOrFail<Operation>::DoInPlace(L, a, b);
			return true;
		}
	} else {
		if constexpr (std::is_same_v<decltype(TryOpOrFail<Operation>::Do(L, a, b)), bool>) {
			return false;
		} else {
			TryOpOrFail<Operation>::Do(L, a, b);
			return true;
		}
	}
}

template <class Fun, bool InPlace, bool Vector, bool Matrix>
__forceinline void CallPolymorphicFunc(lua_State* L, MathParam const& a)
{
	bool handled{ false };
	switch (a.Arity) {
	case 1: 
		if constexpr (Vector) {
			handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.f);
		}
		break;

	case 3: 
		if constexpr (Vector) {
			handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.vec3);
		}
		break;

	case 4: 
		if constexpr (Vector) {
			handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.vec4);
		}
		break;

	case 9: 
		if constexpr (Matrix) {
			handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.mat3);
		}
		break;

	case 16:
		if constexpr (Matrix) {
			handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.mat4);
		}
		break;
	}

	if (!handled) {
		luaL_error(L, "Unsupported argument arity: %d", a.Arity);
	}
}

template <class Fun, bool InPlace, bool Scalar, bool Vector, bool Matrix>
__forceinline void CallPolymorphicFunc(lua_State* L, MathParam const& a, MathParam const& b)
{
	bool handled{ false };
	switch (a.Arity) {
	case 1: {
		if constexpr (Scalar) {
			switch (b.Arity) {
			case 1: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.f, b.f); break;
			case 3: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.f, b.vec3); break;
			case 4: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.f, b.vec4); break;
			case 9: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.f, b.mat3); break;
			case 16: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.f, b.mat4); break;
			}
		}
		break;
	}

	case 3: {
		if constexpr (Vector) {
			switch (b.Arity) {
			case 1: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.vec3, b.f); break;
			case 3: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.vec3, b.vec3); break;
			case 9: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.vec3, b.mat3); break;
			}
		}
		break;
	}

	case 4: {
		if constexpr (Vector) {
			switch (b.Arity) {
			case 1: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.vec4, b.f); break;
			case 4: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.vec4, b.vec4); break;
			case 16: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.vec4, b.mat4); break;
			}
		}
		break;
	}

	case 9: {
		if constexpr (Matrix) {
			switch (b.Arity) {
			case 1: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.mat3, b.f); break;
			case 3: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.mat3, b.vec3); break;
			case 9: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.mat3, b.mat3); break;
			}
		}
		break;
	}

	case 16: {
		if constexpr (Matrix) {
			switch (b.Arity) {
			case 1: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.mat4, b.f); break;
			case 4: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.mat4, b.vec4); break;
			case 16: handled = TryCallPolymorphicFunc<Fun, InPlace>(L, a.mat4, b.mat4); break;
			}
		}
		break;
	}
	}

	if (!handled) {
		luaL_error(L, "Unsupported argument arities: %d, %d", a.Arity, b.Arity);
	}
}

template <class Fun, bool SupportsInPlace, bool Vector = true, bool Matrix = true>
__forceinline int CallFunc(lua_State* L, MathParam const& a)
{
	if constexpr (SupportsInPlace) {
		if (lua_gettop(L) > 1) {
			CallPolymorphicFunc<Fun, true, Vector, Matrix>(L, a);
			return 0;
		}
	}

	CallPolymorphicFunc<Fun, false, Vector, Matrix>(L, a);
	return 1;
}

template <class Fun, bool Scalar = true, bool Vector = true, bool Matrix = true>
__forceinline int CallFunc(lua_State* L, MathParam const& a, MathParam const& b)
{
	if (lua_gettop(L) > 2) {
		CallPolymorphicFunc<Fun, true, Scalar, Vector, Matrix>(L, a, b);
		return 0;
	} else {
		CallPolymorphicFunc<Fun, false, Scalar, Vector, Matrix>(L, a, b);
		return 1;
	}
}


struct AddOp
{
	template <class T1, class T2>
	static __forceinline auto Do(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)(a + b), void())
	{
		push(L, a + b);
	}

	template <class T1, class T2>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)assign(L, 3, a + b), void())
	{
		assign(L, 3, a + b);
	}
};

UserReturn Add(lua_State* L, MathParam const& a, MathParam const& b)
{
	return CallFunc<AddOp>(L, a, b);
}


struct SubtractOp
{
	template <class T1, class T2>
	static __forceinline auto Do(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)(a + b), void())
	{
		push(L, a - b);
	}

	template <class T1, class T2>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)assign(L, 3, a - b), void())
	{
		assign(L, 3, a - b);
	}
};

UserReturn Sub(lua_State* L, MathParam const& a, MathParam const& b)
{
	return CallFunc<SubtractOp>(L, a, b);
}


struct MultiplyOp
{
	template <class T1, class T2>
	static __forceinline auto Do(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)(a * b), void())
	{
		push(L, a * b);
	}

	template <class T1, class T2>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)assign(L, 3, a * b), void())
	{
		assign(L, 3, a * b);
	}
};

UserReturn Mul(lua_State* L, MathParam const& a, MathParam const& b)
{
	return CallFunc<MultiplyOp>(L, a, b);
}


struct DivideOp
{
	template <class T1, class T2>
	static __forceinline auto Do(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)(a / b), void())
	{
		push(L, a / b);
	}

	template <class T1, class T2>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)assign(L, 3, a / b), void())
	{
		assign(L, 3, a / b);
	}
};

UserReturn Div(lua_State* L, MathParam const& a, MathParam const& b)
{
	return CallFunc<DivideOp>(L, a, b);
}


struct ReflectOp
{
	template <class T1, class T2>
	static __forceinline auto Do(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)(glm::reflect(a, b)), void())
	{
		push(L, glm::reflect(a, b));
	}

	template <class T1, class T2>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)assign(L, 3, glm::reflect(a, b)), void())
	{
		assign(L, 3, glm::reflect(a, b));
	}
};

/// <summary>
/// For the incident vector `I` and surface orientation `N`, returns the reflection direction: `result = I - 2.0 * dot(N, I) * N`.
/// </summary>
UserReturn Reflect(lua_State* L, MathParam const& i, MathParam const& n)
{
	return CallFunc<ReflectOp, false, true, false>(L, i, n);
}


struct AngleOp
{
	template <class T1, class T2>
	static __forceinline auto Do(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)(glm::angle(a, b)), void())
	{
		push(L, glm::angle(a, b));
	}

	template <class T1, class T2>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)assign(L, 3, glm::angle(a, b)), void())
	{
		assign(L, 3, glm::angle(a, b));
	}
};


/// <summary>
/// Returns the absolute angle between two vectors.
/// Parameters need to be normalized.
/// </summary>
UserReturn Angle(lua_State* L, MathParam const& a, MathParam const& b)
{
	return CallFunc<AngleOp, false, true, false>(L, a, b);
}


/// <summary>
/// Returns the cross product of x and y.
/// </summary>
UserReturn Cross(lua_State* L, glm::vec3 const& x, glm::vec3 const& y)
{
	if (lua_gettop(L) > 2) {
		assign(L, 3, glm::cross(x, y));
		return 0;
	} else {
		push(L, glm::cross(x, y));
		return 1;
	}
}


/// <summary>
/// Returns the distance between p0 and p1, i.e., `length(p0 - p1)`.
/// </summary>
float Distance(lua_State* L, glm::vec3 const& p0, glm::vec3 const& p1)
{
	return glm::distance(p0, p1);
}


/// <summary>
/// Returns the dot product of x and y, i.e., `result = x * y`.
/// </summary>
float Dot(lua_State* L, glm::vec3 const& x, glm::vec3 const& y)
{
	return glm::dot(x, y);
}


struct LengthOp
{
	template <class T1>
	static __forceinline auto Do(lua_State* L, T1 const& a) -> decltype((void)(glm::length(a)), void())
	{
		push(L, glm::length(a));
	}
};

/// <summary>
/// Returns the length of x, i.e., `sqrt(x * x)`.
/// </summary>
UserReturn Length(lua_State* L, MathParam const& x)
{
	return CallFunc<LengthOp, false, true, false>(L, x);
}


struct NormalizeOp
{
	template <class T1>
	static __forceinline auto Do(lua_State* L, T1 const& a) -> decltype((void)(glm::normalize(a)), void())
	{
		push(L, glm::normalize(a));
	}

	template <class T1>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a) -> decltype((void)assign(L, 3, glm::normalize(a)), void())
	{
		assign(L, 3, glm::normalize(a));
	}
};

/// <summary>
/// Returns a vector in the same direction as x but with length of 1.
/// </summary>
UserReturn Normalize(lua_State* L, MathParam const& x)
{
	return CallFunc<NormalizeOp, true>(L, x);
}


struct DeterminantOp
{
	template <class T1>
	static __forceinline auto Do(lua_State* L, T1 const& a) -> decltype((void)(glm::determinant(a)), void())
	{
		push(L, glm::determinant(a));
	}
};

/// <summary>
/// Return the determinant of a mat3/mat4 matrix.
/// </summary>
UserReturn Determinant(lua_State* L, MathParam const& a)
{
	return CallFunc<DeterminantOp, false, false, true>(L, a);
}


struct InverseOp
{
	template <class T1>
	static __forceinline auto Do(lua_State* L, T1 const& a) -> decltype((void)(glm::inverse(a)), void())
	{
		push(L, glm::inverse(a));
	}

	template <class T1>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a) -> decltype((void)assign(L, 3, glm::inverse(a)), void())
	{
		assign(L, 3, glm::inverse(a));
	}
};

/// <summary>
/// Return the inverse of a mat3/mat4 matrix.
/// </summary>
UserReturn Inverse(lua_State* L, MathParam const& a)
{
	return CallFunc<InverseOp, true, false, true>(L, a);
}


struct TransposeOp
{
	template <class T1>
	static __forceinline auto Do(lua_State* L, T1 const& a) -> decltype((void)(glm::transpose(a)), void())
	{
		push(L, glm::transpose(a));
	}

	template <class T1>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a) -> decltype((void)assign(L, 3, glm::transpose(a)), void())
	{
		assign(L, 3, glm::transpose(a));
	}
};

/// <summary>
/// Returns the transposed matrix of `x`.
/// </summary>
UserReturn Transpose(lua_State* L, MathParam const& a)
{
	return CallFunc<TransposeOp, true, false, true>(L, a);
}


struct OuterProductOp
{
	template <class T1, class T2>
	static __forceinline auto Do(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)(glm::outerProduct(a, b)), void())
	{
		push(L, glm::outerProduct(a, b));
	}

	template <class T1, class T2>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)assign(L, 3, glm::outerProduct(a, b)), void())
	{
		assign(L, 3, glm::outerProduct(a, b));
	}
};

/// <summary>
/// Treats the first parameter `c` as a column vector and the second parameter `r` as a row vector and does a linear algebraic matrix multiply `c * r`.
/// </summary>
UserReturn OuterProduct(lua_State* L, MathParam const& c, MathParam const& r)
{
	return CallFunc<OuterProductOp>(L, c, r);
}

/// <summary>
/// Builds a rotation matrix created from an axis of 3 scalars and an angle expressed in radians.
/// </summary>
void Rotate(lua_State* L, MathParam const& m, float angle, glm::vec3 const& axis)
{
	switch (m.Arity) {
	case 16:
	{
		glm::rotate(m.mat4, angle, axis);
		assign(L, 1, m.mat4);
	}

	case 9:
	{
		glm::mat4 m4{ m.mat3 };
		m4[3][3] = 1.0f;
		glm::rotate(m4, angle, axis);
		assign(L, 1, glm::mat3(m4));
	}

	default:
		luaL_error(L, "Expected a 3x3 or 4x4 matrix");
	}
}

/// <summary>
/// Transforms a matrix with a translation 4 * 4 matrix created from a vector of 3 components.
/// </summary>
void Translate(lua_State* L, glm::mat4 const& m, glm::vec3 const& translation)
{
	glm::translate(m, translation);
	assign(L, 1, m);
}

/// <summary>
/// Transforms a matrix with a scale 4 * 4 matrix created from 3 scalars.
/// </summary>
void Scale(lua_State* L, glm::mat4 const& m, glm::vec3 const& scale)
{
	glm::scale(m, scale);
	assign(L, 1, m);
}

/// <summary>
/// Builds a rotation 4 * 4 matrix created from an axis of 3 scalars and an angle expressed in radians.
/// </summary>
glm::mat4 BuildRotation4(glm::vec3 const& v, float angle)
{
	return glm::rotate(angle, v);
}

/// <summary>
/// Builds a rotation 3 * 3 matrix created from an axis of 3 scalars and an angle expressed in radians.
/// </summary>
glm::mat3 BuildRotation3(glm::vec3 const& v, float angle)
{
	return glm::mat3(glm::rotate(angle, v));
}

/// <summary>
/// Builds a translation 4 * 4 matrix created from a vector of 3 components.
/// </summary>
glm::mat4 BuildTranslation(glm::vec3 const& v)
{
	return glm::translate(v);
}

/// <summary>
/// Builds a scale 4 * 4 matrix created from 3 scalars.
/// </summary>
glm::mat4 BuildScale(glm::vec3 const& v)
{
	return glm::scale(v);
}

glm::quat QuatFromEuler(glm::vec3 const& e)
{
	return glm::quat(e);
}

glm::quat QuatFromToRotation(glm::vec3 const& a, glm::vec3 const& b)
{
	return glm::quat(a, b);
}

float QuatDot(glm::quat const& a, glm::quat const& b)
{
	return glm::dot(a, b);
}

glm::quat QuatSlerp(glm::quat const& a, glm::quat const& b, float alpha)
{
	return glm::mix(a, b, alpha);
}

glm::mat3 QuatToMat3(glm::quat const& a)
{
	return glm::mat3_cast(a);
}

glm::mat4 QuatToMat4(glm::quat const& a)
{
	return glm::mat4_cast(a);
}

glm::quat Mat3ToQuat(glm::mat3 const& a)
{
	return glm::quat_cast(a);
}

glm::quat Mat4ToQuat(glm::mat4 const& a)
{
	return glm::quat_cast(a);
}

glm::quat QuatNormalize(glm::quat const& a)
{
	return glm::normalize(a);
}

glm::quat QuatInverse(glm::quat const& a)
{
	return glm::inverse(a);
}

float QuatLength(glm::quat const& a)
{
	return glm::length(a);
}

UserReturn QuatRotate(lua_State* L, glm::quat const& a, MathParam const& b)
{
	if (b.Arity == 4) {
		push(L, glm::rotate(a, b.vec4));
		return 1;
	}

	if (b.Arity == 3) {
		push(L, glm::rotate(a, b.vec3));
		return 1;
	}

	return luaL_error(L, "Expected a vec3 or vec4 value");
}

glm::quat QuatRotateAxisAngle(glm::quat const& a, glm::vec3 const& axis, float angle)
{
	return glm::rotate(a, angle, axis);
}

UserReturn QuatMul(lua_State* L, MathParam const& a, MathParam const& b)
{
	if (a.Arity == 3 && b.Arity == 4) {
		push(L, a.vec3 * b.quat);
		return 1;
	}

	if (a.Arity == 4 && b.Arity == 4) {
		push(L, a.quat * b.quat);
		return 1;
	}

	if (a.Arity == 4 && b.Arity == 3) {
		push(L, a.quat * b.vec3);
		return 1;
	}

	return luaL_error(L, "Expected a vec3 or quat value");
}

/// <summary>
/// Extracts the `(X * Y * Z)` Euler angles from the rotation matrix M.
/// </summary>
glm::vec3 ExtractEulerAngles(lua_State* L, MathParam const& m)
{
	switch (m.Arity) {
	case 16:
	{
		glm::vec3 angle;
		glm::extractEulerAngleXYZ(m.mat4, angle.x, angle.y, angle.z);
		return angle;
	}

	case 9:
	{
		glm::vec3 angle;
		glm::extractEulerAngleXYZ(glm::mat4(m.mat3), angle.x, angle.y, angle.z);
		return angle;
	}

	default:
		luaL_error(L, "Expected a 3x3 or 4x4 matrix");
		return glm::vec3();
	}
}

/// <summary>
/// Creates a 3D 4 * 4 homogeneous rotation matrix from euler angles `(X * Y * Z)`.
/// </summary>
glm::mat4 BuildFromEulerAngles4(lua_State* L, glm::vec3 const& angle)
{
	return glm::eulerAngleYXZ(angle.x, angle.y, angle.z);
}

/// <summary>
/// Creates a 3D 3 * 3 homogeneous rotation matrix from euler angles `(X * Y * Z)`.
/// </summary>
glm::mat3 BuildFromEulerAngles3(lua_State* L, glm::vec3 const& angle)
{
	return glm::mat3(glm::eulerAngleYXZ(angle.x, angle.y, angle.z));
}

/// <summary>
/// Decomposes a model matrix to translations, rotation and scale components.
/// </summary>
void Decompose(lua_State* L, glm::mat4 const& m, glm::vec3 const& scale_, glm::vec3 const& yawPitchRoll, glm::vec3 const& translation_)
{
	glm::quat orientation;
	glm::vec3 scale, translation, skew;
	glm::vec4 perspective;
	glm::decompose(m, scale, orientation, translation, skew, perspective);
	glm::vec3 rotation{
		glm::yaw(orientation),
		glm::pitch(orientation),
		glm::roll(orientation),
	};
	assign(L, 2, scale);
	assign(L, 3, rotation);
	assign(L, 4, translation);
}

/// <summary>
/// Get the axis and angle of the rotation from a matrix.
/// </summary>
float ExtractAxisAngle(lua_State* L, MathParam const& m, glm::vec3 const& axis_)
{
	switch (m.Arity) {
	case 16:
	{
		glm::vec3 axis;
		float angle;
		glm::axisAngle(m.mat4, axis, angle);
		assign(L, 2, axis);
		return angle;
	}

	case 9:
	{
		glm::vec3 axis;
		float angle;
		glm::mat4 m4{ m.mat3 };
		m4[3][3] = 1.0f;
		glm::axisAngle(m4, axis, angle);
		assign(L, 2, axis);
		return angle;
	}

	default:
		luaL_error(L, "Expected a 3x3 or 4x4 matrix");
		return 0.0f;
	}
}

/// <summary>
/// Build a matrix from axis and angle.
/// </summary>
glm::mat3 BuildFromAxisAngle3(lua_State* L, glm::vec3 const& axis, float angle)
{
	return glm::mat3(glm::axisAngleMatrix(axis, angle));
}

/// <summary>
/// Build a matrix from axis and angle.
/// </summary>
glm::mat4 BuildFromAxisAngle4(lua_State* L, glm::vec3 const& axis, float angle)
{
	return glm::axisAngleMatrix(axis, angle);
}


struct PerpendicularOp
{
	template <class T1, class T2>
	static __forceinline auto Do(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)(glm::perp(a, b)), void())
	{
		push(L, glm::perp(a, b));
	}

	template <class T1, class T2>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)assign(L, 3, glm::perp(a, b)), void())
	{
		assign(L, 3, glm::perp(a, b));
	}
};

/// <summary>
/// Projects `x` on a perpendicular axis of `normal`.
/// </summary>
UserReturn Perpendicular(lua_State* L, MathParam const& x, MathParam const& normal)
{
	return CallFunc<PerpendicularOp, false, true, false>(L, x, normal);
}


struct ProjectOp
{
	template <class T1, class T2>
	static __forceinline auto Do(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)(glm::proj(a, b)), void())
	{
		push(L, glm::proj(a, b));
	}

	template <class T1, class T2>
	static __forceinline auto DoInPlace(lua_State* L, T1 const& a, T2 const& b) -> decltype((void)assign(L, 3, glm::proj(a, b)), void())
	{
		assign(L, 3, glm::proj(a, b));
	}
};

/// <summary>
/// Projects `x` on `normal`.
/// </summary>
UserReturn Project(lua_State* L, MathParam const& x, MathParam const& normal)
{
	return CallFunc<ProjectOp, false, true, false>(L, x, normal);
}

/// <summary>
/// Return x - floor(x).
/// </summary>
float Fract(float val)
{
	return glm::fract(val);
}

/// <summary>
/// Returns a value equal to the nearest integer to x whose absolute value is not larger than the absolute value of x.
/// </summary>
float Trunc(float val)
{
	return glm::trunc(val);
}

/// <summary>
/// Returns 1.0 if x > 0, 0.0 if x == 0, or -1.0 if x < 0.
/// </summary>
float Sign(float x)
{
	return glm::sign(x);
}

/// <summary>
/// Returns `min(max(x, minVal), maxVal)` for each component in x using the floating-point values minVal and maxVal.
/// </summary>
float Clamp(float val, float min, float max)
{
	return glm::clamp(val, min, max);
}

/// <summary>
/// Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth Hermite interpolation between 0 and 1 when edge0 < x < edge1.
/// </summary>
float Smoothstep(float edge0, float edge1, float x)
{
	return glm::smoothstep(edge0, edge1, x);
}

/// <summary>
/// Returns x * (1.0 - a) + y * a, i.e., the linear blend of x and y using the floating-point value a.
/// </summary>
float Lerp(float x, float y, float a)
{
	return glm::mix(x, y, a);
}

/// <summary>
/// Arc cosine. Returns an angle whose sine is x.
/// </summary>
float Acos(float x)
{
	return glm::acos(x);
}

/// <summary>
/// Arc sine. Returns an angle whose sine is x.
/// </summary>
float Asin(float x)
{
	return glm::asin(x);
}

/// <summary>
/// Arc tangent.
/// Returns an angle whose tangent is y_over_x.
/// </summary>
float Atan(float y_over_x)
{
	return glm::atan(y_over_x);
}

/// <summary>
/// Arc tangent.
/// Returns an angle whose tangent is y / x.
/// The signs of x and y are used to determine what quadrant the angle is in.
/// </summary>
float Atan2(float x, float y)
{
	return glm::atan(x, y);
}

/// <summary>
/// Returns true if x holds a NaN (not a number) representation.
/// </summary>
bool IsNaN(lua_Number x)
{
	return glm::isnan(x);
}

/// <summary>
/// Returns true if x holds a positive infinity or negative infinity representation.
/// </summary>
bool IsInf(lua_Number x)
{
	return glm::isinf(x);
}

// Variation of Lua builtin math_random() with custom RNG
UserReturn Random(lua_State *L)
{
	auto& state = ExtensionStateBase::FromLua(L);

	lua_Integer low, up;
	switch (lua_gettop(L)) {  /* check number of arguments */
	case 0: {  /* no arguments */
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		push(L, (lua_Number)dist(state.OsiRng));  /* Number between 0 and 1 */
		return 1;
	}
	case 1: {  /* only upper limit */
		low = 1;
		up = luaL_checkinteger(L, 1);
		break;
	}
	case 2: {  /* lower and upper limits */
		low = luaL_checkinteger(L, 1);
		up = luaL_checkinteger(L, 2);
		break;
	}
	default: return luaL_error(L, "wrong number of arguments");
	}
	/* random integer in the interval [low, up] */
	luaL_argcheck(L, low <= up, 1, "interval is empty");
#if LUA_VERSION_NUM > 501
	luaL_argcheck(L, low >= 0 || up <= LUA_MAXINTEGER + low, 1,
		"interval too large");
#endif

	std::uniform_int_distribution<int64_t> dist(low, up);
	push(L, dist(state.OsiRng));
	return 1;
}

int64_t Round(double val)
{
	return (int64_t)round(val);
}

void RegisterMathLib()
{
	DECLARE_MODULE(Math, Both)
	BEGIN_MODULE()
	MODULE_FUNCTION(Add)
	MODULE_FUNCTION(Sub)
	MODULE_FUNCTION(Mul)
	MODULE_FUNCTION(Div)
	MODULE_FUNCTION(Reflect)
	MODULE_FUNCTION(Angle)
	MODULE_FUNCTION(Cross)
	MODULE_FUNCTION(Distance)
	MODULE_FUNCTION(Dot)
	MODULE_FUNCTION(Length)
	MODULE_FUNCTION(Normalize)
	MODULE_FUNCTION(Perpendicular)
	MODULE_FUNCTION(Project)

	MODULE_FUNCTION(Determinant)
	MODULE_FUNCTION(Inverse)
	MODULE_FUNCTION(Transpose)
	MODULE_FUNCTION(OuterProduct)
	MODULE_FUNCTION(Rotate)
	MODULE_FUNCTION(Translate)
	MODULE_FUNCTION(Scale)
	MODULE_FUNCTION(ExtractEulerAngles)
	MODULE_FUNCTION(BuildFromEulerAngles3)
	MODULE_FUNCTION(BuildFromEulerAngles4)
	MODULE_FUNCTION(Decompose)
	MODULE_FUNCTION(ExtractAxisAngle)
	MODULE_FUNCTION(BuildFromAxisAngle3)
	MODULE_FUNCTION(BuildFromAxisAngle4)
	MODULE_FUNCTION(BuildRotation3)
	MODULE_FUNCTION(BuildRotation4)
	MODULE_FUNCTION(BuildTranslation)
	MODULE_FUNCTION(BuildScale)

	MODULE_FUNCTION(QuatFromEuler)
	MODULE_FUNCTION(QuatFromToRotation)
	MODULE_FUNCTION(QuatDot)
	MODULE_FUNCTION(QuatSlerp)
	MODULE_FUNCTION(QuatToMat3)
	MODULE_FUNCTION(QuatToMat4)
	MODULE_FUNCTION(Mat3ToQuat)
	MODULE_FUNCTION(Mat4ToQuat)
	MODULE_FUNCTION(QuatNormalize)
	MODULE_FUNCTION(QuatInverse)
	MODULE_FUNCTION(QuatRotate)
	MODULE_FUNCTION(QuatRotateAxisAngle)
	MODULE_FUNCTION(QuatLength)
	MODULE_FUNCTION(QuatMul)

	MODULE_FUNCTION(Random)
	MODULE_FUNCTION(Round)
	MODULE_FUNCTION(Fract)
	MODULE_FUNCTION(Trunc)
	MODULE_FUNCTION(Sign)
	MODULE_FUNCTION(Clamp)
	MODULE_FUNCTION(Smoothstep)
	MODULE_FUNCTION(Lerp)
	MODULE_FUNCTION(Asin)
	MODULE_FUNCTION(Acos)
	MODULE_FUNCTION(Atan)
	MODULE_FUNCTION(Atan2)
	MODULE_FUNCTION(IsNaN)
	MODULE_FUNCTION(IsInf)

	END_MODULE()
}

END_NS()
