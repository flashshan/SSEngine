#pragma once

#include "Matrix.h"
#include "Vector3.h"

class Box {
public:
	FORCEINLINE Box();
	FORCEINLINE Box(const Vector3 &i_min, const Vector3 &i_max);
	FORCEINLINE Box(const Box &i_other);

	FORCEINLINE void Transform(const Matrix& i_matrix);

private:
	Vector3 minVertex_, maxVertex_;
	bool isValid_;
};




// implement forceinline

FORCEINLINE Box::Box()
	: minVertex_(0.0f, 0.0f, 0.0f), maxVertex_(0.0f, 0.0f, 0.0f), isValid_(false)
{
}

FORCEINLINE Box::Box(const Vector3 &i_min, const Vector3 &i_max)
	: minVertex_(i_min), maxVertex_(i_max), isValid_(true)
{
}

FORCEINLINE Box::Box(const Box &i_other)
	: minVertex_(i_other.minVertex_), maxVertex_(i_other.maxVertex_), isValid_(i_other.isValid_)
{
}

FORCEINLINE void Box::Transform(const Matrix& i_matrix)
{
	minVertex_ = Vector3(i_matrix.MultiplyLeft(Vector4(minVertex_, 1.0f)));
	maxVertex_ = Vector3(i_matrix.MultiplyLeft(Vector4(maxVertex_, 1.0f)));

}


