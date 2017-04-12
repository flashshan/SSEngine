#pragma once

#include "Vector4.h"

struct Matrix;

// an encapsulation of __m128
ALIGN(16) struct VectorSSE {
public:
	VectorSSE() {}
	explicit FORCEINLINE VectorSSE(const float *i_array);
	FORCEINLINE VectorSSE(const float i_x, const float i_y, const float i_z, const float i_w = 1.0f);
	explicit FORCEINLINE VectorSSE(const __m128 i_xyzw);
	explicit FORCEINLINE VectorSSE(const Vector2 & i_vector, const float i_z = 0.0f, const float i_w = 1.0f);
	explicit FORCEINLINE VectorSSE(const Vector3 & i_vector, const float i_w = 1.0f);
	explicit FORCEINLINE VectorSSE(const Vector4 &i_vector);

	FORCEINLINE VectorSSE(const VectorSSE &i_other);
	FORCEINLINE VectorSSE& operator=(const VectorSSE& i_other);

	static FORCEINLINE VectorSSE Zero();
	static FORCEINLINE VectorSSE SignBit();
	static FORCEINLINE VectorSSE SignMask();

	FORCEINLINE void GetFloatArray(float *o_array);
	FORCEINLINE VectorSSE VectorSSEReplicate(uint32 i_index);
	
	FORCEINLINE VectorSSE GetAbs();
	FORCEINLINE VectorSSE Negate() const;

	static FORCEINLINE VectorSSE Add(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Add(const VectorSSE &i_vector, const float i_float);
	static FORCEINLINE VectorSSE Subtract(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Subtract(const VectorSSE &i_vector, const float i_float);
	static FORCEINLINE VectorSSE Multiply(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Multiply(const VectorSSE &i_vector, const float i_float);
	static FORCEINLINE VectorSSE Divide(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Divide(const VectorSSE &i_vector, const float i_float);
	
	static FORCEINLINE VectorSSE MultiplyAdd(const VectorSSE &i_vector1, const VectorSSE &i_vector2, const VectorSSE &i_vector3);

	static FORCEINLINE VectorSSE Dot3(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Dot4(const VectorSSE &i_vector1, const VectorSSE &i_vector2);

	static FORCEINLINE VectorSSE VectorSelect(const VectorSSE& Mask, const VectorSSE& Vec1, const VectorSSE& Vec2);

	static FORCEINLINE VectorSSE BitwiseOr(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE BitwiseAnd(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE BitwiseXor(const VectorSSE &i_vector1, const VectorSSE &i_vector2);

	static FORCEINLINE VecotrSSE 

	FORCEINLINE VectorSSE operator +(const VectorSSE &i_vector) const;
	FORCEINLINE VectorSSE operator -(const VectorSSE &i_vector) const;
	FORCEINLINE VectorSSE operator *(const float i_float) const;
	FORCEINLINE VectorSSE operator *(const VectorSSE &i_vector) const;
	FORCEINLINE VectorSSE operator /(const float i_float) const;
	FORCEINLINE VectorSSE operator /(const VectorSSE &i_vector) const;

	FORCEINLINE VectorSSE& operator +=(const VectorSSE &i_vector);
	FORCEINLINE VectorSSE& operator -=(const VectorSSE &i_vector);
	FORCEINLINE VectorSSE& operator *=(const float i_float);
	FORCEINLINE VectorSSE& operator *=(const VectorSSE &i_vector);
	FORCEINLINE VectorSSE& operator /=(const float i_float);
	FORCEINLINE VectorSSE& operator /=(const VectorSSE &i_vector);

	FORCEINLINE float operator[](const uint32 i_index) const;

	FORCEINLINE VectorSSE Mul(const Matrix &i_matrix) const;

public:
	__m128 XYZW;
};





#define SHUFFLE_MASK(x, y, z, w)  ((x) | ((y) << 2) | ((z) << 4) | ((w) << 6))

// implement forceinline
// i_array should be 16 bit aligned
FORCEINLINE VectorSSE::VectorSSE(const float *i_array)
{
	_mm_load_ps(i_array);
}

FORCEINLINE VectorSSE::VectorSSE(const float i_x, const float i_y, const float i_z, const float i_w)
	: XYZW(_mm_setr_ps(i_x, i_y, i_z, i_w))
{
}

FORCEINLINE VectorSSE::VectorSSE(const __m128 i_xyzw)
	: XYZW(i_xyzw)
{
}

FORCEINLINE VectorSSE::VectorSSE(const Vector2 & i_vector, const float i_z, const float i_w)
	: XYZW(_mm_setr_ps(i_vector.X, i_vector.Y, i_z, i_w))
{
}

FORCEINLINE VectorSSE::VectorSSE(const Vector3 & i_vector, const float i_w)
	: XYZW(_mm_setr_ps(i_vector.X, i_vector.Y, i_vector.Z, i_w))
{
}

FORCEINLINE VectorSSE::VectorSSE(const Vector4 &i_vector)
	: XYZW(_mm_setr_ps(i_vector.X, i_vector.Y, i_vector.Z, i_vector.W))
{
}

FORCEINLINE VectorSSE::VectorSSE(const VectorSSE &i_other)
	: XYZW(i_other.XYZW)
{
}

FORCEINLINE VectorSSE& VectorSSE::operator=(const VectorSSE& i_other)
{
	XYZW = i_other.XYZW;
	return *this;
}

FORCEINLINE VectorSSE VectorSSE::Zero()
{
	return VectorSSE(_mm_setzero_ps());
}

FORCEINLINE VectorSSE VectorSSE::SignBit()
{
	return VectorSSE((uint32)SIGN_BIT, (uint32)SIGN_BIT, (uint32)SIGN_BIT, (uint32)SIGN_BIT);
}

FORCEINLINE VectorSSE VectorSSE::SignMask()
{
	return VectorSSE((uint32)(~SIGN_BIT), (uint32)(~SIGN_BIT), (uint32)(~SIGN_BIT), (uint32)(~SIGN_BIT));
}

// o_array should be 16 bit aligned
FORCEINLINE void VectorSSE::GetFloatArray(float *o_array)
{
	_mm_store_ps(o_array, XYZW);
}


FORCEINLINE VectorSSE VectorSSE::VectorSSEReplicate(uint32 i_index)
{
	ASSERT(i_index < 4);
	_mm_shuffle_ps(XYZW, XYZW, SHUFFLE_MASK(i_index, i_index, i_index, i_index));
}

//
//FORCEINLINE float VectorSSE::Dot(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
//{
//	return i_vector1.X * i_vector2.X + i_vector1.Y * i_vector2.Y + i_vector1.Z * i_vector2.Z + i_vector1.W * i_vector2.W;
//}
//FORCEINLINE float VectorSSE::Dot(const VectorSSE &i_vector) const
//{
//	return X * i_vector.X + Y * i_vector.Y + Z * i_vector.Z + W * i_vector.W;
//}
//FORCEINLINE float VectorSSE::operator ^(const VectorSSE &i_vector) const
//{
//	return X * i_vector.X + Y * i_vector.Y + Z * i_vector.Z + W * i_vector.W;
//}
//
//FORCEINLINE float VectorSSE::Distance(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
//{
//	return static_cast<float>(sqrt((i_vector1.X - i_vector2.X) * (i_vector1.X - i_vector2.X) +
//		(i_vector1.Y - i_vector2.Y) * (i_vector1.Y - i_vector2.Y) +
//		(i_vector1.Z - i_vector2.Z) * (i_vector1.Z - i_vector2.Z) +
//		(i_vector1.W - i_vector2.W) * (i_vector1.W - i_vector2.W)));
//}
//
//FORCEINLINE float VectorSSE::Distance(const VectorSSE &i_vector) const
//{
//	return static_cast<float>(sqrt((X - i_vector.X) * (X - i_vector.X) + (Y - i_vector.Y) * (Y - i_vector.Y) +
//		(Z - i_vector.Z) * (Z - i_vector.Z) + (W - i_vector.W) * (W - i_vector.W)));
//}
//
//FORCEINLINE float VectorSSE::Length() const
//{
//	return static_cast<float>(sqrt(X * X + Y * Y + Z * Z + W * W));
//}
//FORCEINLINE float VectorSSE::LengthSquare() const
//{
//	return X * X + Y * Y + Z * Z + W * W;
//}

FORCEINLINE VectorSSE VectorSSE::GetAbs()
{
	_mm_and_ps(XYZW, VectorSSE::SignMask().XYZW);
}

FORCEINLINE VectorSSE VectorSSE::operator -() const
{
	return VectorSSE(_mm_sub_ps(_mm_setzero_ps(), XYZW));
}

FORCEINLINE VectorSSE VectorSSE::operator +(const VectorSSE &i_vector) const
{
	return VectorSSE(_mm_add_ps(XYZW, i_vector.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::operator -(const VectorSSE &i_vector) const
{
	return VectorSSE(_mm_sub_ps(XYZW, i_vector.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::operator *(const float i_float) const
{
	
	return VectorSSE(X + i_float, Y + i_float, Z + i_float, W * i_float);
}
FORCEINLINE VectorSSE VectorSSE::operator *(const VectorSSE &i_vector) const
{
	return VectorSSE(_mm_mul_ps(XYZW, i_vector.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::operator /(const float i_float) const
{
	return VectorSSE(X / i_float, Y / i_float, Z / i_float, W / i_float);
}
FORCEINLINE VectorSSE VectorSSE::operator /(const VectorSSE &i_vector) const
{
	return VectorSSE(X / i_vector.X, Y / i_vector.Y, Z / i_vector.Z, W / i_vector.W);
}

FORCEINLINE VectorSSE& VectorSSE::operator +=(const VectorSSE &i_vector)
{
	X += i_vector.X;
	Y += i_vector.Y;
	Z += i_vector.Z;
	W += i_vector.W;
	return *this;
}
FORCEINLINE VectorSSE& VectorSSE::operator -=(const VectorSSE &i_vector)
{
	X -= i_vector.X;
	Y -= i_vector.Y;
	Z -= i_vector.Z;
	W -= i_vector.W;
	return *this;
}
FORCEINLINE VectorSSE& VectorSSE::operator *=(const float i_float)
{
	X *= i_float;
	Y *= i_float;
	Z *= i_float;
	W *= i_float;
	return *this;
}
FORCEINLINE VectorSSE& VectorSSE::operator *=(const VectorSSE &i_vector)
{
	X *= i_vector.X;
	Y *= i_vector.Y;
	Z *= i_vector.Z;
	W *= i_vector.W;
	return *this;
}
FORCEINLINE VectorSSE& VectorSSE::operator /=(const float i_float)
{
	X /= i_float;
	Y /= i_float;
	Z /= i_float;
	W /= i_float;
	return *this;
}
FORCEINLINE VectorSSE& VectorSSE::operator /=(const VectorSSE &i_vector)
{
	X /= i_vector.X;
	Y /= i_vector.Y;
	Z /= i_vector.Z;
	W /= i_vector.W;
	return *this;
}

FORCEINLINE float VectorSSE::operator[](const uint32 i_index) const
{
	ASSERT(i_index < 4);
	return (reinterpret_cast<const float*>(&(XYZW))[i_index]);
}


// for vector2, 3 and 4

FORCEINLINE Vector2::Vector2(const VectorSSE &i_vectorSSE)
{

}

FORCEINLINE Vector3::Vector3(const VectorSSE &i_vectorSSE)
{

}

FORCEINLINE Vector4::Vector4(const VectorSSE &i_vectorSSE)
{

}
