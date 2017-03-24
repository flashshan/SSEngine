#pragma once

#include "Vector3.h"

struct Matrix;

ALIGN(16) struct Vector4 {
public:
	Vector4() {}
	FORCEINLINE Vector4(const Vector4 & i_vector);
	FORCEINLINE Vector4(const float i_x, const float i_y, const float i_z, const float i_w = 0.0f);
	FORCEINLINE explicit Vector4(const Vector2 & i_vector, const float i_z = 0.0f, const float i_w = 0.0f);
	FORCEINLINE explicit Vector4(const Vector3 & i_vector, const float i_w = 0.0f);

	FORCEINLINE void operator =(const Vector4 &i_vector);

	FORCEINLINE static Vector4 Zero();
	FORCEINLINE static Vector4 Unit();

	FORCEINLINE void Normalize(float i_tolerance = 0.0000001);

	FORCEINLINE static float Dot(const Vector4 &i_vector1, const Vector4 &i_vector2);
	FORCEINLINE float Dot(const Vector4 &i_vector) const;
	FORCEINLINE float operator ^(const Vector4 &i_vector) const;

	FORCEINLINE static float Distance(const Vector4 &i_vector1, const Vector4 &i_vector2);
	FORCEINLINE float Distance(const Vector4 &i_vector) const;
	FORCEINLINE float Length() const;
	FORCEINLINE float LengthSquare() const;

	FORCEINLINE void ToVector();
	FORCEINLINE void ToPoint();
	FORCEINLINE bool Equal(const Vector4 &i_vector, float i_tolerance) const;

	FORCEINLINE Vector4 operator -() const;
	FORCEINLINE Vector4 operator +(const Vector4 &i_vector) const;
	FORCEINLINE Vector4 operator -(const Vector4 &i_vector) const;
	FORCEINLINE Vector4 operator *(const float i_float) const;
	FORCEINLINE Vector4 operator *(const Vector4 &i_vector) const;
	FORCEINLINE Vector4 operator /(const float i_float) const;
	FORCEINLINE Vector4 operator /(const Vector4 &i_vector) const;

	FORCEINLINE void operator +=(const Vector4 &i_vector);
	FORCEINLINE void operator -=(const Vector4 &i_vector);
	FORCEINLINE void operator *=(const float i_float);
	FORCEINLINE void operator *=(const Vector4 &i_vector);
	FORCEINLINE void operator /=(const float i_float);
	FORCEINLINE void operator /=(const Vector4 &i_vector);

	FORCEINLINE float operator[](const uint32 i_index) const;

	FORCEINLINE Vector4 Transform(const Matrix &i_matrix) const;
public:
	float X, Y, Z, W;
};








// implement forceinline

FORCEINLINE Vector4::Vector4(const float i_x, const float i_y, const float i_z, const float i_w)
	: X(i_x), Y(i_y), Z(i_z), W(i_w) {}

FORCEINLINE Vector4::Vector4(const Vector4 & i_vector)
	: X(i_vector.X), Y(i_vector.Y), Z(i_vector.Z), W(i_vector.W) {}

FORCEINLINE Vector4::Vector4(const Vector2 & i_vector, const float i_z, const float i_w)
	: X(i_vector.X), Y(i_vector.Y), Z(i_z), W(i_w) {}

FORCEINLINE Vector4::Vector4(const Vector3 & i_vector, const float i_w)
	: X(i_vector.X), Y(i_vector.Y), Z(i_vector.Z), W(i_w) {}

FORCEINLINE void Vector4::operator =(const Vector4 &i_vector)
{
	X = i_vector.X;
	Y = i_vector.Y;
	Z = i_vector.Z;
	W = i_vector.W;
}

FORCEINLINE Vector4 Vector4::Zero()
{
	return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

FORCEINLINE Vector4 Vector4::Unit()
{
	return Vector4(1.0f, 1.0f, 1.0f, 0.0f);
}

FORCEINLINE void Vector4::Normalize(float i_tolerance)
{
	const float magSq = Length();
	ASSERT(magSq > i_tolerance);
	X /= magSq;
	Y /= magSq;
	Z /= magSq;
	W /= magSq;
}

FORCEINLINE float Vector4::Dot(const Vector4 &i_vector1, const Vector4 &i_vector2)
{
	return i_vector1.X * i_vector2.X + i_vector1.Y * i_vector2.Y + i_vector1.Z * i_vector2.Z + i_vector1.W * i_vector2.W;
}
FORCEINLINE float Vector4::Dot(const Vector4 &i_vector) const
{
	return X * i_vector.X + Y * i_vector.Y + Z * i_vector.Z + W * i_vector.W;
}
FORCEINLINE float Vector4::operator ^(const Vector4 &i_vector) const
{
	return X * i_vector.X + Y * i_vector.Y + Z * i_vector.Z + W * i_vector.W;
}

FORCEINLINE float Vector4::Distance(const Vector4 &i_vector1, const Vector4 &i_vector2)
{
	return static_cast<float>(sqrt((i_vector1.X - i_vector2.X) * (i_vector1.X - i_vector2.X) +
		(i_vector1.Y - i_vector2.Y) * (i_vector1.Y - i_vector2.Y) +
		(i_vector1.Z - i_vector2.Z) * (i_vector1.Z - i_vector2.Z) +
		(i_vector1.W - i_vector2.W) * (i_vector1.W - i_vector2.W)));
}

FORCEINLINE float Vector4::Distance(const Vector4 &i_vector) const
{
	return static_cast<float>(sqrt((X - i_vector.X) * (X - i_vector.X) + (Y - i_vector.Y) * (Y - i_vector.Y) +
		(Z - i_vector.Z) * (Z - i_vector.Z) + (W - i_vector.W) * (W - i_vector.W)));
}

FORCEINLINE float Vector4::Length() const
{
	return static_cast<float>(sqrt(X * X + Y * Y + Z * Z + W * W));
}
FORCEINLINE float Vector4::LengthSquare() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

FORCEINLINE void Vector4::ToVector()
{
	W = 0.0f;
}

FORCEINLINE void Vector4::ToPoint()
{
	W = 1.0f;
}

FORCEINLINE bool Vector4::Equal(const Vector4 &i_vector, float i_tolerance) const
{
	return Float::EqualFast(X, i_vector.X, i_tolerance) && Float::EqualFast(Y, i_vector.Y, i_tolerance) &&
		Float::EqualFast(Z, i_vector.Z, i_tolerance) && Float::EqualFast(W, i_vector.W, i_tolerance);
}

FORCEINLINE Vector4 Vector4::operator -() const
{
	return Vector4(-X, -Y, -Z, -W);
}
FORCEINLINE Vector4 Vector4::operator +(const Vector4 &i_vector) const
{
	return Vector4(X + i_vector.X, Y + i_vector.Y, Z + i_vector.Z, W + i_vector.W);
}
FORCEINLINE Vector4 Vector4::operator -(const Vector4 &i_vector) const
{
	return Vector4(X - i_vector.X, Y - i_vector.Y, Z - i_vector.Z, W - i_vector.W);
}
FORCEINLINE Vector4 Vector4::operator *(const float i_float) const
{
	return Vector4(X + i_float, Y + i_float, Z + i_float, W * i_float);
}
FORCEINLINE Vector4 Vector4::operator *(const Vector4 &i_vector) const
{
	return Vector4(X + i_vector.X, Y + i_vector.Y, Z + i_vector.Z, W + i_vector.W);
}
FORCEINLINE Vector4 Vector4::operator /(const float i_float) const
{
	return Vector4(X / i_float, Y / i_float, Z / i_float, W / i_float);
}
FORCEINLINE Vector4 Vector4::operator /(const Vector4 &i_vector) const
{
	return Vector4(X / i_vector.X, Y / i_vector.Y, Z / i_vector.Z, W / i_vector.W);
}

FORCEINLINE void Vector4::operator +=(const Vector4 &i_vector)
{
	X += i_vector.X;
	Y += i_vector.Y;
	Z += i_vector.Z;
	W += i_vector.W;
}
FORCEINLINE void Vector4::operator -=(const Vector4 &i_vector)
{
	X -= i_vector.X;
	Y -= i_vector.Y;
	Z -= i_vector.Z;
	W -= i_vector.W;
}
FORCEINLINE void Vector4::operator *=(const float i_float)
{
	X *= i_float;
	Y *= i_float;
	Z *= i_float;
	W *= i_float;
}
FORCEINLINE void Vector4::operator *=(const Vector4 &i_vector)
{
	X *= i_vector.X;
	Y *= i_vector.Y;
	Z *= i_vector.Z;
	W *= i_vector.W;
}
FORCEINLINE void Vector4::operator /=(const float i_float)
{
	X /= i_float;
	Y /= i_float;
	Z /= i_float;
	W /= i_float;
}
FORCEINLINE void Vector4::operator /=(const Vector4 &i_vector)
{
	X /= i_vector.X;
	Y /= i_vector.Y;
	Z /= i_vector.Z;
	W /= i_vector.W;
}

FORCEINLINE float Vector4::operator[](const uint32 i_index) const
{
	ASSERT(i_index < 4);
	return *(&X + i_index);
}


// for vector2 and vector3

FORCEINLINE Vector2::Vector2(const Vector4 & i_vector)
	: X(i_vector.X), Y(i_vector.Y)
{
}

FORCEINLINE Vector3::Vector3(const Vector4 & i_vector)
	: X(i_vector.X), Y(i_vector.Y), Z(i_vector.Z) {}
