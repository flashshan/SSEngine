#pragma once

#include "Core\CoreMinimal.h"

#include "Vector4.h"
#include "Matrix.h"

class Triangle {
public:
	FORCEINLINE Triangle(const Vector3 &i_a, const Vector3 &i_b, const Vector3 &i_c);
	FORCEINLINE Triangle(const Vector4 &i_a, const Vector4 &i_b, const Vector4 &i_c);
	FORCEINLINE Triangle(const Triangle & i_other);

	// get
	FORCEINLINE Vector4 A() const { return V_[0]; }
	FORCEINLINE Vector4 B() const { return V_[1]; }
	FORCEINLINE Vector4 C() const { return V_[2]; }
	FORCEINLINE Vector4 OriginA() const { return originV_[0]; }
	FORCEINLINE Vector4 OriginB() const { return originV_[1]; }
	FORCEINLINE Vector4 OriginC() const { return originV_[2]; }

	FORCEINLINE static Triangle Identity();

	void Transform(const Matrix &i_Matrix);
	
	FORCEINLINE void ChangeOrigin();

public:
	Vector4 V_[3];
	Vector4 originV_[3];
};






// implement forceinline

FORCEINLINE Triangle::Triangle(const Vector3 &i_a, const Vector3 &i_b, const Vector3 &i_c)
	: V_{ Vector4(i_a, 1.0f), Vector4(i_b, 1.0f), Vector4(i_c, 1.0f) },
	originV_{ Vector4(i_a, 1.0f), Vector4(i_b, 1.0f), Vector4(i_c, 1.0f) } 
{
}

FORCEINLINE Triangle::Triangle(const Vector4 &i_a, const Vector4 &i_b, const Vector4 &i_c)
	: V_{ i_a, i_b, i_c }, originV_{ i_a, i_b, i_c } 
{
}

FORCEINLINE Triangle::Triangle(const Triangle & i_other)
	: V_{ i_other.A(), i_other.B(), i_other.C() }, originV_{ i_other.OriginA(), i_other.OriginB(), i_other.OriginC() } {}

FORCEINLINE Triangle Triangle::Identity()
{
	return Triangle(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
}


FORCEINLINE void Triangle::ChangeOrigin()
{
	originV_[0] = V_[0];
	originV_[1] = V_[1];
	originV_[2] = V_[2];
}