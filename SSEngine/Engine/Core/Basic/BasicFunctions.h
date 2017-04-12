#pragma once

#include "BasicTypes.h"
#include "Assert.h"


namespace Basic {
	template<typename T> FORCEINLINE void Swap(T &i_Left, T &i_Right);

	// reverse between small end and big end machine
	FORCEINLINE int16 SwapInt16(int16 i_value);
	FORCEINLINE uint16 SwapUint16(uint16 i_value);

	FORCEINLINE int32 SwapInt32(int32 i_value);
	FORCEINLINE uint32 SwapUint32(uint32 i_value);

	FORCEINLINE int64 SwapInt64(int64 i_value);
	FORCEINLINE uint64 SwapUint64(uint64 i_value);

	FORCEINLINE float SwapFloat32(float i_value);
	FORCEINLINE double swapFloat64(double i_value);
}


namespace Float {

	FORCEINLINE bool IsNAN(float i_val);
	FORCEINLINE bool IsZero(float i_val);
	FORCEINLINE bool IsFinite(float i_val);

	FORCEINLINE void RandInit(int32 Seed) { srand(Seed); }
	FORCEINLINE int32 Rand() { return rand(); }
	FORCEINLINE float RandWithinOne();
	FORCEINLINE uint32 RandInRange(uint32 i_lowerBound, uint32 i_upperBound);

	// fastest
	FORCEINLINE bool EqualFast(float i_lhs, float i_rhs, float i_maxDiff = SMALL_NUMBER);
	// balanced
	FORCEINLINE bool EqualRelated(float i_lhs, float i_rhs, float i_maxDiff = SMALL_NUMBER);
	// slow but sure
	FORCEINLINE bool EqualAccurate(float i_lhs, float i_rhs, float i_maxDiff, uint32 i_maxULPS = 12);

	static const uint32 allBitsSet = ~0;
}

struct Rotator;

namespace Math {

	FORCEINLINE float DegreesToRadians(float i_Degrees);
	FORCEINLINE float RadiansToDegrees(float i_Radians);

	FORCEINLINE float Fmod(float X, float Y);

	FORCEINLINE int Abs(int i_value);
	FORCEINLINE float AbsF(float i_float);

	FORCEINLINE void SinCos(float* o_scalarSin, float* o_scalarCos, float i_Value);


	FORCEINLINE Rotator Lerp(const Rotator& i_rotator1, const Rotator& i_rotator2, float i_alpha);
	FORCEINLINE Rotator LerpRange(const Rotator& i_rotator1, const Rotator& i_rotator2, float i_alpha);
}





// implement forceinline

namespace Basic {

	template<typename T> FORCEINLINE void Swap(T &i_Left, T &i_Right)
	{
		T temp = i_Left;
		i_Left = i_Right;
		i_Right = temp;
	}

	FORCEINLINE int16 SwapInt16(int16 i_value)
	{
		return ((i_value & 0x00ff) << 8)
			| ((i_value & 0xff00) >> 8);
	}
	FORCEINLINE uint16 SwapUint16(uint16 i_value)
	{
		return ((i_value & 0x00ff) << 8)
			| ((i_value & 0xff00) >> 8);
	}

	FORCEINLINE int32 SwapInt32(int32 i_value)
	{
		return ((i_value & 0x000000ff) << 24)
			| ((i_value & 0x0000ff00) << 8)
			| ((i_value & 0x00ff0000) >> 8)
			| ((i_value & 0xff000000) >> 24);
	}
	FORCEINLINE uint32 SwapUint32(uint32 i_value)
	{
		return ((i_value & 0x000000ff) << 24)
			| ((i_value & 0x0000ff00) << 8)
			| ((i_value & 0x00ff0000) >> 8)
			| ((i_value & 0xff000000) >> 24);
	}

	FORCEINLINE int64 SwapInt64(int64 i_value)
	{
		return ((i_value & 0x00000000000000ff) << 56)
			| ((i_value & 0x000000000000ff00) << 40)
			| ((i_value & 0x0000000000ff0000) << 24)
			| ((i_value & 0x00000000ff000000) << 8)
			| ((i_value & 0x000000ff00000000) >> 8)
			| ((i_value & 0x0000ff0000000000) >> 24)
			| ((i_value & 0x00ff000000000000) >> 40)
			| ((i_value & 0xff00000000000000) >> 56);
	}
	FORCEINLINE uint64 SwapUint64(uint64 i_value)
	{
		return ((i_value & 0x00000000000000ff) << 56)
			| ((i_value & 0x000000000000ff00) << 40)
			| ((i_value & 0x0000000000ff0000) << 24)
			| ((i_value & 0x00000000ff000000) << 8)
			| ((i_value & 0x000000ff00000000) >> 8)
			| ((i_value & 0x0000ff0000000000) >> 24)
			| ((i_value & 0x00ff000000000000) >> 40)
			| ((i_value & 0xff00000000000000) >> 56);
	}

	FORCEINLINE float SwapFloat32(float i_value)
	{
		U32F32 temp;
		temp.asFloat32 = i_value;
		temp.asUint32 = SwapUint32(temp.asUint32);
		return temp.asFloat32;
	}
	FORCEINLINE double swapFloat64(double i_value)
	{
		U64F64 temp;
		temp.asFloat64 = i_value;
		temp.asUint64 = SwapUint64(temp.asUint64);
		return temp.asFloat64;
	}
}


namespace Float {

	FORCEINLINE bool IsNAN(float i_val)
	{
		/*volatile float val = i_val;
		return val != val;*/

		// another way to judge
		return ((*(uint32*)&i_val) & 0x7FFFFFFF) > 0x7F800000;
	}

	FORCEINLINE bool IsZero(float i_val)
	{
		return EqualFast(i_val, static_cast<float>(SMALL_NUMBER));
	}
	FORCEINLINE bool IsFinite(float i_val)
	{
		return ((*(uint32*)&i_val) & 0x7F800000) != 0x7F800000;
	}

	FORCEINLINE float RandWithinOne()
	{
		return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	}
	FORCEINLINE uint32 RandInRange(uint32 i_lowerBound, uint32 i_upperBound)
	{
		ASSERT(i_lowerBound < i_upperBound);
		return i_lowerBound + rand() % (i_upperBound - i_lowerBound);
	}


	FORCEINLINE bool EqualFast(float i_lhs, float i_rhs, float i_maxDiff)
	{
		return fabs(i_lhs - i_rhs) < i_maxDiff;
	}

	FORCEINLINE bool EqualRelated(float i_lhs, float i_rhs, float i_maxDiff)
	{
		if (i_lhs == i_rhs)
			return true;

		float relDiff;

		if (fabs(i_rhs) > fabs(i_lhs))
			relDiff = static_cast<float>(fabs((i_lhs - i_rhs) / i_rhs));
		else
			relDiff = static_cast<float>(fabs((i_lhs - i_rhs) / i_lhs));

		return relDiff <= i_maxDiff;
	}

	// slow but sure
	FORCEINLINE bool EqualAccurate(float i_lhs, float i_rhs, float i_maxDiff, uint32 i_maxULPS)
	{
		ASSERT(sizeof(float) == sizeof(int32));

		if (i_lhs == i_rhs)
			return true;

		float diff = static_cast<float>(fabs(i_lhs - i_rhs));
		if (diff <= i_maxDiff)
			return true;

		uint32 intDiff = abs(*reinterpret_cast<int *>(&i_lhs) - *reinterpret_cast<int *>(&i_rhs));
		return intDiff <= i_maxULPS;
	}
}



namespace Math {

	FORCEINLINE float DegreesToRadians(float i_Degrees)
	{
		static const float DegToRad = static_cast<float>(PI) / 180.0f;
		return i_Degrees * DegToRad;
	}

	FORCEINLINE float RadiansToDegrees(float i_Radians)
	{
		static const float RadToDeg = 180.0f / static_cast<float>(PI);
		return i_Radians * RadToDeg;
	}

	// from Unreal
	FORCEINLINE float Fmod(float X, float Y)
	{
		ASSERT(Math::AbsF(Y) > SMALL_NUMBER);

		const float Quotient = static_cast<float>(static_cast<int32>(X / Y));
		float IntPortion = Y * Quotient;

		// Rounding and imprecision could cause IntPortion to exceed X and cause the result to be outside the expected range.
		// For example Fmod(55.8, 9.3) would result in a very small negative value!
		if (Math::AbsF(IntPortion) > Math::AbsF(X))
		{
			IntPortion = X;
		}

		const float Result = X - IntPortion;
		return Result;
	}

	FORCEINLINE int Abs(int i_value)
	{
		return i_value > 0 ? i_value : -i_value;
	}

	FORCEINLINE float AbsF(float i_float)
	{
		return i_float > 0 ? i_float : -i_float;
	}

	FORCEINLINE void SinCos(float* o_scalarSin, float* o_scalarCos, float i_Value)
	{
		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = (INV_PI*0.5f)*i_Value;
		if (i_Value >= 0.0f)
		{
			quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
		}
		float y = i_Value - (2.0f*PI)*quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign;
		if (y > HALF_PI)
		{
			y = PI - y;
			sign = -1.0f;
		}
		else if (y < -HALF_PI)
		{
			y = -PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		*o_scalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*o_scalarCos = sign*p;
	}
}