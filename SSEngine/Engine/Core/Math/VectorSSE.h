#pragma once

#include "Vector4.h"

struct Matrix;

// an encapsulation of __m128
ALIGN(16) struct VectorSSE {
public:
	VectorSSE() {}
	explicit FORCEINLINE VectorSSE(const float *i_array);
	FORCEINLINE VectorSSE(float i_x, float i_y, float i_z, float i_w = 1.0f);                  // assign by 4 values
	explicit FORCEINLINE VectorSSE(__m128 i_xyzw);
	explicit FORCEINLINE VectorSSE(const Vector2 & i_vector, float i_z = 0.0f, float i_w = 1.0f);
	explicit FORCEINLINE VectorSSE(const Vector3 & i_vector, float i_w = 1.0f);
	explicit FORCEINLINE VectorSSE(const Vector4 &i_vector);

	FORCEINLINE VectorSSE(const VectorSSE &i_other);
	FORCEINLINE VectorSSE& operator=(const VectorSSE& i_other);

	static FORCEINLINE VectorSSE CreateFromUint32(uint32 i_x, uint32 i_y, uint32 i_z, uint32 i_w);        // assign by 32 * 4 bits

	static FORCEINLINE VectorSSE Zero();
	static FORCEINLINE VectorSSE One();
	static FORCEINLINE VectorSSE SignBit();
	static FORCEINLINE VectorSSE SignMask();

	FORCEINLINE void GetFloatArray(float *o_array) const;
	FORCEINLINE VectorSSE Replicate(uint32 i_index) const;
	
	FORCEINLINE VectorSSE LengthSquare3() const;
	FORCEINLINE VectorSSE LengthSquare4() const;
	FORCEINLINE VectorSSE Length3() const;
	FORCEINLINE VectorSSE Length4() const;
	FORCEINLINE VectorSSE& Normalize3(const float i_tolerance = SMALL_NUMBER);
	FORCEINLINE VectorSSE& Normalize4(const float i_tolerance = SMALL_NUMBER);

	FORCEINLINE VectorSSE GetAbs() const;
	FORCEINLINE VectorSSE Negate() const;

	static FORCEINLINE VectorSSE Add(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Add(const VectorSSE &i_vector, float i_float);
	static FORCEINLINE VectorSSE Subtract(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Subtract(const VectorSSE &i_vector, float i_float);
	static FORCEINLINE VectorSSE Multiply(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Multiply(const VectorSSE &i_vector, float i_float);
	static FORCEINLINE VectorSSE Divide(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Divide(const VectorSSE &i_vector, float i_float);
	
	static FORCEINLINE VectorSSE MultiplyAdd(const VectorSSE &i_vector1, const VectorSSE &i_vector2, const VectorSSE &i_vector3);

	static FORCEINLINE VectorSSE Dot3(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Dot4(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Cross3(const VectorSSE &i_vector1, const VectorSSE &i_vector2);

	static FORCEINLINE VectorSSE VectorSelect(const VectorSSE &i_mask, const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE BitwiseOr(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE BitwiseAnd(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE BitwiseXor(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	
	static FORCEINLINE VectorSSE CompareEQ(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE CompareNE(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE CompareGT(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE CompareGE(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE CompareLT(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE CompareLE(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE int32 CompareAnyGT(const VectorSSE &i_vector1, const VectorSSE &i_vector2);


	static FORCEINLINE VectorSSE Min(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Max(const VectorSSE &i_vector1, const VectorSSE &i_vector2);
	static FORCEINLINE VectorSSE Mod(const VectorSSE &i_vector1, const VectorSSE &i_vector2);

	FORCEINLINE VectorSSE Reciprocal() const;
	FORCEINLINE VectorSSE Sqrt() const;
	FORCEINLINE VectorSSE ReciprocalSqrt() const;

	FORCEINLINE VectorSSE Truncate() const;
	FORCEINLINE VectorSSE Fractional() const;
	FORCEINLINE VectorSSE Ceil() const;
	FORCEINLINE VectorSSE Floor() const;

	FORCEINLINE float operator[](uint32 i_index) const;

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

FORCEINLINE VectorSSE::VectorSSE(float i_x, float i_y, float i_z, float i_w)
	: XYZW(_mm_setr_ps(i_x, i_y, i_z, i_w))
{
}

FORCEINLINE VectorSSE::VectorSSE(__m128 i_xyzw)
	: XYZW(i_xyzw)
{
}

FORCEINLINE VectorSSE::VectorSSE(const Vector2 & i_vector, float i_z, float i_w)
	: XYZW(_mm_setr_ps(i_vector.X, i_vector.Y, i_z, i_w))
{
}

FORCEINLINE VectorSSE::VectorSSE(const Vector3 & i_vector, float i_w)
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


FORCEINLINE VectorSSE VectorSSE::CreateFromUint32(uint32 i_x, uint32 i_y, uint32 i_z, uint32 i_w)
{
	U32F32 x, y, z, w;
	x.asUint32 = i_x;
	y.asUint32 = i_y;
	z.asUint32 = i_z;
	w.asUint32 = i_w;
	return VectorSSE(x.asFloat32, y.asFloat32, z.asFloat32, w.asFloat32);
}

FORCEINLINE VectorSSE VectorSSE::Zero()
{
	return VectorSSE(_mm_setzero_ps());
}

FORCEINLINE VectorSSE VectorSSE::One()
{
	return VectorSSE(1.0f, 1.0f, 1.0f, 1.0f);
}

FORCEINLINE VectorSSE VectorSSE::SignBit()
{
	return VectorSSE::CreateFromUint32(static_cast<uint32>(SIGN_BIT), static_cast<uint32>(SIGN_BIT), static_cast<uint32>(SIGN_BIT), static_cast<uint32>(SIGN_BIT));
}

FORCEINLINE VectorSSE VectorSSE::SignMask()
{
	return VectorSSE::CreateFromUint32(static_cast<uint32>(~SIGN_BIT), static_cast<uint32>(~SIGN_BIT), static_cast<uint32>(~SIGN_BIT), static_cast<uint32>(~SIGN_BIT));
}

// o_array should be 16 bit aligned
FORCEINLINE void VectorSSE::GetFloatArray(float *o_array) const
{
	_mm_store_ps(o_array, XYZW);
}


FORCEINLINE VectorSSE VectorSSE::Replicate(uint32 i_index) const
{
	switch (i_index)
	{
	case 0:
		return VectorSSE(_mm_shuffle_ps(XYZW, XYZW, SHUFFLE_MASK(0, 0, 0, 0)));
	case 1:
		return VectorSSE(_mm_shuffle_ps(XYZW, XYZW, SHUFFLE_MASK(1, 1, 1, 1)));
	case 2:
		return VectorSSE(_mm_shuffle_ps(XYZW, XYZW, SHUFFLE_MASK(2, 2, 2, 2)));
	case 3:
		return VectorSSE(_mm_shuffle_ps(XYZW, XYZW, SHUFFLE_MASK(3, 3, 3, 3)));
	default:
		ASSERT(0);
		return VectorSSE();
	}
}

FORCEINLINE VectorSSE VectorSSE::LengthSquare3() const
{
	return Dot3(*this, *this);
}

FORCEINLINE VectorSSE VectorSSE::LengthSquare4() const
{
	return Dot4(*this, *this);
}


FORCEINLINE VectorSSE VectorSSE::Length3() const
{
	return Dot3(*this, *this).Sqrt();
}
FORCEINLINE VectorSSE VectorSSE::Length4() const
{
	return Dot4(*this, *this).Sqrt();
}

// normalize the first 3, the last is illegal
FORCEINLINE VectorSSE& VectorSSE::Normalize3(float i_tolerance)
{
	VectorSSE sqrtDot3 = Dot3(*this, *this).Sqrt();
	ASSERT(sqrtDot3[0] > i_tolerance);
	return Divide(*this, sqrtDot3);
}

FORCEINLINE VectorSSE& VectorSSE::Normalize4(float i_tolerance)
{
	VectorSSE sqrtDot4 = Dot4(*this, *this).Sqrt();
	ASSERT(sqrtDot4[0] > i_tolerance);
	return Divide(*this, sqrtDot4);
}

FORCEINLINE VectorSSE VectorSSE::GetAbs() const
{
	return VectorSSE(_mm_and_ps(XYZW, VectorSSE::SignMask().XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Negate() const
{
	return VectorSSE(_mm_sub_ps(_mm_setzero_ps(), XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Add(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_add_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::Add(const VectorSSE &i_vector, float i_float)
{
	return VectorSSE(_mm_add_ps(i_vector.XYZW, _mm_set_ps(i_float, i_float, i_float, i_float)));
}

FORCEINLINE VectorSSE VectorSSE::Subtract(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_sub_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::Subtract(const VectorSSE &i_vector, float i_float)
{
	return VectorSSE(_mm_sub_ps(i_vector.XYZW, _mm_set_ps(i_float, i_float, i_float, i_float)));
}

FORCEINLINE VectorSSE VectorSSE::Multiply(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_mul_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::Multiply(const VectorSSE &i_vector, float i_float)
{
	return VectorSSE(_mm_mul_ps(i_vector.XYZW, _mm_set_ps(i_float, i_float, i_float, i_float)));
}

FORCEINLINE VectorSSE VectorSSE::Divide(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_div_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::Divide(const VectorSSE &i_vector, float i_float)
{
	return VectorSSE(_mm_div_ps(i_vector.XYZW, _mm_set_ps(i_float, i_float, i_float, i_float)));
}

FORCEINLINE VectorSSE VectorSSE::MultiplyAdd(const VectorSSE &i_vector1, const VectorSSE &i_vector2, const VectorSSE &i_vector3)
{
	return VectorSSE(_mm_add_ps(_mm_mul_ps(i_vector1.XYZW, i_vector2.XYZW), i_vector3.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Dot3(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	VectorSSE temp = Multiply(i_vector1, i_vector2);
	return Add(temp.Replicate(0), Add(temp.Replicate(1), temp.Replicate(2)));
}

FORCEINLINE VectorSSE VectorSSE::Dot4(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	VectorSSE temp1, temp2;
	temp1 = Multiply(i_vector1, i_vector2);
	temp2 = VectorSSE(_mm_shuffle_ps(temp1.XYZW, temp1.XYZW, SHUFFLE_MASK(2, 3, 0, 1)));				// (Z,W,X,Y).
	temp1 = Add(temp1, temp2);																			// (X*X + Z*Z, Y*Y + W*W, Z*Z + X*X, W*W + Y*Y)
	temp2 = VectorSSE(_mm_shuffle_ps(temp1.XYZW, temp1.XYZW, SHUFFLE_MASK(1, 2, 3, 0)));				// Rotate left 4 bytes (Y,Z,W,X).
	return Add(temp1, temp2);																			// (X*X + Z*Z + Y*Y + W*W, Y*Y + W*W + Z*Z + X*X, Z*Z + X*X + W*W + Y*Y, W*W + Y*Y + X*X + Z*Z)
}

FORCEINLINE VectorSSE VectorSSE::Cross3(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	VectorSSE A_YZXW = VectorSSE(_mm_shuffle_ps(i_vector1.XYZW, i_vector1.XYZW, SHUFFLE_MASK(1, 2, 0, 3)));
	VectorSSE B_ZXYW = VectorSSE(_mm_shuffle_ps(i_vector2.XYZW, i_vector2.XYZW, SHUFFLE_MASK(2, 0, 1, 3)));
	VectorSSE A_ZXYW = VectorSSE(_mm_shuffle_ps(i_vector1.XYZW, i_vector1.XYZW, SHUFFLE_MASK(2, 0, 1, 3)));
	VectorSSE B_YZXW = VectorSSE(_mm_shuffle_ps(i_vector2.XYZW, i_vector2.XYZW, SHUFFLE_MASK(1, 2, 0, 3)));
	return Subtract(Multiply(A_YZXW, B_ZXYW), Multiply(A_ZXYW, B_YZXW));
}

FORCEINLINE VectorSSE VectorSSE::VectorSelect(const VectorSSE &i_mask, const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_xor_ps(i_vector2.XYZW, _mm_and_ps(i_mask.XYZW, _mm_xor_ps(i_vector1.XYZW, i_vector2.XYZW))));
}

FORCEINLINE VectorSSE VectorSSE::BitwiseOr(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_or_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::BitwiseAnd(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_and_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::BitwiseXor(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_xor_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::CompareEQ(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmpeq_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::CompareNE(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmpneq_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::CompareGT(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmpgt_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::CompareGE(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmpge_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::CompareLT(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmplt_ps(i_vector1.XYZW, i_vector2.XYZW));
}
FORCEINLINE VectorSSE VectorSSE::CompareLE(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_cmple_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE int32 VectorSSE::CompareAnyGT(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return _mm_movemask_ps(_mm_cmpgt_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Min(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_min_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Max(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	return VectorSSE(_mm_max_ps(i_vector1.XYZW, i_vector2.XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Mod(const VectorSSE &i_vector1, const VectorSSE &i_vector2)
{
	VectorSSE temp = Divide(i_vector1, i_vector2).Truncate();
	return Subtract(i_vector1, Multiply(i_vector2, temp));
}

FORCEINLINE VectorSSE VectorSSE::Reciprocal() const
{
	return VectorSSE(_mm_rcp_ps(XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Sqrt() const
{
	return VectorSSE(_mm_sqrt_ps(XYZW));
}

FORCEINLINE VectorSSE VectorSSE::ReciprocalSqrt() const
{
	return VectorSSE(_mm_rsqrt_ps(XYZW));
}

FORCEINLINE VectorSSE VectorSSE::Truncate() const
{
	return VectorSSE(_mm_cvtepi32_ps(_mm_cvttps_epi32(XYZW)));
}

FORCEINLINE VectorSSE VectorSSE::Fractional() const
{
	return VectorSSE(Subtract(*this, Truncate()));
}

FORCEINLINE VectorSSE VectorSSE::Ceil() const
{
	VectorSSE trunc = Truncate();
	VectorSSE posMask = CompareGE(*this, Zero());
	VectorSSE add = VectorSelect(posMask, One(), Zero());
	return Add(trunc, add);
}
FORCEINLINE VectorSSE VectorSSE::Floor() const
{
	VectorSSE trunc = Truncate();
	VectorSSE posMask = CompareGE(*this, Zero());
	VectorSSE sub = VectorSelect(posMask, Zero(), One());
	return Subtract(trunc, sub);
}


FORCEINLINE float VectorSSE::operator[](uint32 i_index) const
{
	ASSERT(i_index < 4);
	return (reinterpret_cast<const float*>(&(XYZW))[i_index]);
}


// for vector2, 3 and 4

FORCEINLINE Vector2::Vector2(const VectorSSE &i_vectorSSE)
	: X(i_vectorSSE[0]), Y(i_vectorSSE[1])
{
}

FORCEINLINE Vector3::Vector3(const VectorSSE &i_vectorSSE)
	: X(i_vectorSSE[0]), Y(i_vectorSSE[1]), Z(i_vectorSSE[2])
{	
}

FORCEINLINE Vector4::Vector4(const VectorSSE &i_vectorSSE)
	: X(i_vectorSSE[0]), Y(i_vectorSSE[1]), Z(i_vectorSSE[2]), W(i_vectorSSE[3])
{
}
