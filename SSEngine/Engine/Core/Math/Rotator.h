#pragma once

#include "Core\CoreMinimal.h"
#include "Vector3.h"

struct Quaternion;

struct Rotator {
public:
	FORCEINLINE Rotator() {}
	FORCEINLINE Rotator(float i_pitch, float i_yaw, float i_roll);
	explicit FORCEINLINE Rotator(const Quaternion &i_quaternion);
	FORCEINLINE Rotator(const Rotator &i_other);

	FORCEINLINE Rotator& operator=(const Rotator &i_other);

	// check if exactly the same
	FORCEINLINE bool operator==(const Rotator &i_other) const;
	FORCEINLINE bool operator!=(const Rotator &i_other) const;

	FORCEINLINE Rotator operator+(const Rotator &i_other) const;
	FORCEINLINE Rotator operator-(const Rotator &i_other) const;
	FORCEINLINE Rotator operator*(float i_scale) const;
	
	FORCEINLINE Rotator& operator+=(const Rotator &i_other);
	FORCEINLINE Rotator& operator-=(const Rotator &i_other);
	FORCEINLINE Rotator& operator*=(float i_scale);

	FORCEINLINE bool IsZero(float i_tolerance = SMALL_NUMBER) const;
	FORCEINLINE bool Equal(const Rotator& i_other, float i_tolerance = SMALL_NUMBER) const;

	FORCEINLINE Rotator& Inverse();
	FORCEINLINE Rotator GetInverse() const;
	
	inline Vector3 ToVector() const;
	FORCEINLINE Vector3 ToEuler() const;
	//Quaternion ToQuaternion() const;

	FORCEINLINE Vector3 RotateVector(const Vector3 &i_vector) const;
	FORCEINLINE Vector3 UnrotateVector(const Vector3 &i_vector) const;

	// restrict value to [0, 360]
	FORCEINLINE Rotator& Clamp();
	FORCEINLINE Rotator GetClamp() const;
	
	FORCEINLINE Rotator& Normalize();
	FORCEINLINE Rotator GetNormalized() const;
	FORCEINLINE Rotator& Denormalized();
	FORCEINLINE Rotator GetDenormalized() const;

	static FORCEINLINE Rotator Zero();
	static FORCEINLINE float ClampAxis(float i_angle);
	static FORCEINLINE float NormalizeAxis(float i_angle);

public:
	float Pitch, Yaw, Roll;
};







// implement forceinline

FORCEINLINE Rotator::Rotator(float i_pitch, float i_yaw, float i_roll)
	: Pitch(i_pitch), Yaw(i_yaw), Roll(i_roll)
{
	ASSERT(!Float::IsNAN(Pitch));
	ASSERT(!Float::IsNAN(Yaw));
	ASSERT(!Float::IsNAN(Roll));
}

FORCEINLINE Rotator::Rotator(const Quaternion &i_quaternion)
{
	//*this = i_quaternion.ToRotator();
	ASSERT(!Float::IsNAN(Pitch));
	ASSERT(!Float::IsNAN(Yaw));
	ASSERT(!Float::IsNAN(Roll));
}

FORCEINLINE Rotator::Rotator(const Rotator &i_other)
	: Pitch(i_other.Pitch), Yaw(i_other.Yaw), Roll(i_other.Roll)
{
}

FORCEINLINE Rotator& Rotator::operator=(const Rotator &i_other)
{
	Pitch = i_other.Pitch;
	Yaw = i_other.Yaw;
	Roll = i_other.Roll;
	return *this;
}


// check if exactly the same
FORCEINLINE bool Rotator::operator==(const Rotator &i_other) const
{
	return Pitch == i_other.Pitch && Yaw == i_other.Yaw && Roll == i_other.Roll;
}

FORCEINLINE bool Rotator::operator!=(const Rotator &i_other) const
{
	return Pitch != i_other.Pitch || Yaw != i_other.Yaw || Roll != i_other.Roll;
}

FORCEINLINE Rotator Rotator::operator+(const Rotator &i_other) const
{
	return Rotator(Pitch + i_other.Pitch, Yaw + i_other.Yaw, Roll + i_other.Roll);
}

FORCEINLINE Rotator Rotator::operator-(const Rotator &i_other) const
{
	return Rotator(Pitch - i_other.Pitch, Yaw - i_other.Yaw, Roll - i_other.Roll);
}

FORCEINLINE Rotator Rotator::operator*(float i_float) const
{
	return Rotator(Pitch * i_float, Yaw * i_float, Roll * i_float);
}

FORCEINLINE Rotator& Rotator::operator+=(const Rotator &i_other)
{
	Pitch += i_other.Pitch;
	Yaw += i_other.Yaw;
	Roll += i_other.Roll;
	return *this;
}

FORCEINLINE Rotator& Rotator::operator-=(const Rotator &i_other)
{
	Pitch -= i_other.Pitch;
	Yaw -= i_other.Yaw;
	Roll -= i_other.Roll;
	return *this;
}

FORCEINLINE Rotator& Rotator::operator*=(float i_float)
{
	Pitch *= i_float;
	Yaw *= i_float;
	Roll *= i_float;
	return *this;
}

FORCEINLINE bool Rotator::Rotator::IsZero(float i_tolerance) const
{
	return (Math::AbsF(Rotator::NormalizeAxis(Pitch)) < i_tolerance) && (Math::AbsF(Rotator::NormalizeAxis(Yaw)) < i_tolerance) && (Math::AbsF(Rotator::NormalizeAxis(Roll)) < i_tolerance);
}

FORCEINLINE bool Rotator::Equal(const Rotator& i_other, float i_tolerance) const
{
	return (Math::AbsF(Rotator::NormalizeAxis(Pitch - i_other.Pitch)) < i_tolerance) &&
		   (Math::AbsF(Rotator::NormalizeAxis(Yaw - i_other.Yaw)) < i_tolerance) &&
		   (Math::AbsF(Rotator::NormalizeAxis(Roll - i_other.Roll)) < i_tolerance);
}

FORCEINLINE Rotator& Rotator::Inverse()
{
	Pitch = -Pitch;
	Yaw = -Yaw;
	Roll = -Roll;
	return *this;
}

FORCEINLINE Rotator Rotator::GetInverse() const
{
	return Rotator(-Pitch, -Yaw, -Roll);
}

inline Vector3 Rotator::ToVector() const
{
	float CP, SP, CY, SY;
	Math::SinCos(&SP, &CP, Math::DegreesToRadians(Pitch));
	Math::SinCos(&SY, &CY, Math::DegreesToRadians(Yaw));
	return Vector3(CP*CY, CP*SY, SP);
}

FORCEINLINE Vector3 Rotator::ToEuler() const
{
	return Vector3(Roll, Pitch, Yaw);
}

//Quaternion Rotator::ToQuaternion() const
//{
	//const Vector4 Angles = MakeVectorRegister(Pitch, Yaw, Roll, 0.0f);
	//const VectorRegister HalfAngles = VectorMultiply(Angles, GlobalVectorConstants::DEG_TO_RAD_HALF);

	//VectorRegister SinAngles, CosAngles;
	//VectorSinCos(&SinAngles, &CosAngles, &HalfAngles);

	//// Vectorized conversion, measured 20% faster than using scalar version after VectorSinCos.
	//// Indices within VectorRegister (for shuffles): P=0, Y=1, R=2
	//const VectorRegister SR = VectorReplicate(SinAngles, 2);
	//const VectorRegister CR = VectorReplicate(CosAngles, 2);

	//const VectorRegister SY_SY_CY_CY_Temp = VectorShuffle(SinAngles, CosAngles, 1, 1, 1, 1);

	//const VectorRegister SP_SP_CP_CP = VectorShuffle(SinAngles, CosAngles, 0, 0, 0, 0);
	//const VectorRegister SY_CY_SY_CY = VectorShuffle(SY_SY_CY_CY_Temp, SY_SY_CY_CY_Temp, 0, 2, 0, 2);

	//const VectorRegister CP_CP_SP_SP = VectorShuffle(CosAngles, SinAngles, 0, 0, 0, 0);
	//const VectorRegister CY_SY_CY_SY = VectorShuffle(SY_SY_CY_CY_Temp, SY_SY_CY_CY_Temp, 2, 0, 2, 0);

	//const uint32 Neg = uint32(1 << 31);
	//const uint32 Pos = uint32(0);
	//const VectorRegister SignBitsLeft = MakeVectorRegister(Pos, Neg, Pos, Pos);
	//const VectorRegister SignBitsRight = MakeVectorRegister(Neg, Neg, Neg, Pos);
	//const VectorRegister LeftTerm = VectorBitwiseXor(SignBitsLeft, VectorMultiply(CR, VectorMultiply(SP_SP_CP_CP, SY_CY_SY_CY)));
	//const VectorRegister RightTerm = VectorBitwiseXor(SignBitsRight, VectorMultiply(SR, VectorMultiply(CP_CP_SP_SP, CY_SY_CY_SY)));

	//FQuat RotationQuat;
	//const VectorRegister Result = VectorAdd(LeftTerm, RightTerm);
	//VectorStoreAligned(Result, &RotationQuat);



	//const float DEG_TO_RAD = PI / (180.f);
	//const float DIVIDE_BY_2 = DEG_TO_RAD / 2.f;
	//float SP, SY, SR;
	//float CP, CY, CR;

	//FMath::SinCos(&SP, &CP, Pitch*DIVIDE_BY_2);
	//FMath::SinCos(&SY, &CY, Yaw*DIVIDE_BY_2);
	//FMath::SinCos(&SR, &CR, Roll*DIVIDE_BY_2);

	//FQuat RotationQuat;
	//RotationQuat.X = CR*SP*SY - SR*CP*CY;
	//RotationQuat.Y = -CR*SP*CY - SR*CP*SY;
	//RotationQuat.Z = CR*CP*SY - SR*SP*CY;
	//RotationQuat.W = CR*CP*CY + SR*SP*SY;


	//RotationQuat.DiagnosticCheckNaN();

	//return RotationQuat;
//}

FORCEINLINE Vector3 Rotator::RotateVector(const Vector3 &i_vector) const
{
	// TO DO
	return Vector3(0, 0, 0);
}

FORCEINLINE Vector3 Rotator::UnrotateVector(const Vector3 &i_vector) const
{
	// TO DO
	return Vector3(0, 0, 0);
}

// restrict value to [0, 360]
FORCEINLINE Rotator& Rotator::Clamp()
{
	Pitch = ClampAxis(Pitch);
	Yaw = ClampAxis(Yaw);
	Roll = ClampAxis(Roll);
	return *this;
}

FORCEINLINE Rotator Rotator::GetClamp() const
{
	return Rotator(ClampAxis(Pitch), ClampAxis(Yaw), ClampAxis(Roll));
}

FORCEINLINE Rotator& Rotator::Normalize()
{
	Pitch = NormalizeAxis(Pitch);
	Yaw = NormalizeAxis(Yaw);
	Roll = NormalizeAxis(Roll);
	return *this;
}

FORCEINLINE Rotator Rotator::GetNormalized() const
{
	return Rotator(NormalizeAxis(Pitch), NormalizeAxis(Yaw), NormalizeAxis(Roll));
}

FORCEINLINE Rotator& Rotator::Denormalized()
{
	Pitch = ClampAxis(Pitch);
	Yaw = ClampAxis(Yaw);
	Roll = ClampAxis(Roll);
	return *this;
}

FORCEINLINE Rotator Rotator::GetDenormalized() const
{
	return Rotator(ClampAxis(Pitch), ClampAxis(Yaw), ClampAxis(Roll));
}


FORCEINLINE Rotator Rotator::Zero()
{
	return Rotator(0.0f, 0.0f, 0.0f);
}

FORCEINLINE float Rotator::ClampAxis(float i_angle)
{
	float angle = Math::Fmod(i_angle, 360.0f);
	if (angle < 0.0f)
	{
		angle += 360.0f;
	}
	return angle;
}

FORCEINLINE float Rotator::NormalizeAxis(float i_angle)
{
	float angle = ClampAxis(i_angle);

	if (angle > 180.0f)
	{
		angle -= 360.0f;
	}
	return angle;
}





// for math
FORCEINLINE Rotator Math::Lerp(const Rotator& i_rotator1, const Rotator& i_rotator2, float i_alpha)
{
	return i_rotator1 + (i_rotator2 - i_rotator1).GetNormalized() * i_alpha;
}

FORCEINLINE Rotator Math::LerpRange(const Rotator& i_rotator1, const Rotator& i_rotator2, float i_alpha)
{
	// Similar to Lerp, but does not take the shortest path. Allows interpolation over more than 180 degrees.
	return (i_rotator1 * (1 - i_alpha) + i_rotator2 * i_alpha).GetNormalized();
}
