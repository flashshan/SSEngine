#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Math/Vector4.h"

class Transform
{
	FORCEINLINE Transform();
	FORCEINLINE Transform(const Vector3 &i_location, const Vector3 &i_rotation, const Vector3 &i_scale);
	FORCEINLINE Transform(const Transform &i_other);
	FORCEINLINE ~Transform();

	FORCEINLINE Transform &operator =(const Transform &i_other);

	FORCEINLINE Vector3 GetLocation() const { return location_; }
	FORCEINLINE Vector3 GetRotation() const { return rotation_; }
	FORCEINLINE Vector3 GetScale() const { return scale_; }

	FORCEINLINE void SetLocation(const Vector3 &i_vector) { location_ += i_vector; }
	FORCEINLINE void SetRotation(const Vector3 &i_vector) { rotation_ += i_vector; }
	FORCEINLINE void SetScale(const Vector3 &i_vector) { scale_ *= i_vector; }

	FORCEINLINE void Translate(const Vector3 &i_vector);
	FORCEINLINE void Rotate(const Vector3 &i_vector);
	FORCEINLINE void Scale(const Vector3 &i_vector);

private:
	Vector3 location_;
	Vector3 rotation_;
	Vector3 scale_;
};




// implement forceinline

FORCEINLINE Transform::Transform()
	:location_(0.0f, 0.0f, 0.0f)
{
}

FORCEINLINE Transform::Transform(const Vector3 &i_location, const Vector3 &i_rotation, const Vector3 &i_scale)
	: location_(i_location), rotation_(i_rotation), scale_(i_scale)
{
}

FORCEINLINE Transform::Transform(const Transform &i_other)
	: location_(i_other.GetLocation()), rotation_(i_other.GetRotation()), scale_(i_other.GetScale())
{
}

FORCEINLINE Transform::~Transform()
{
}

FORCEINLINE Transform &Transform::operator =(const Transform &i_other)
{
	location_ = i_other.GetLocation();
	rotation_ = i_other.GetRotation();
	scale_ = i_other.GetScale();
	return *this;
}

FORCEINLINE void Transform::Translate(const Vector3 &i_vector)
{
	location_ += i_vector;
}

FORCEINLINE void Transform::Rotate(const Vector3 &i_vector)
{
	rotation_ += i_vector;
}

FORCEINLINE void Transform::Scale(const Vector3 &i_vector)
{
	scale_ *= i_vector;
}

// implement forceinline
