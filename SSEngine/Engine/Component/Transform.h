#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Math/Vector4.h"

class Transform
{
public:
	FORCEINLINE Transform();
	FORCEINLINE Transform(const Vector3 &i_location, const Vector3 &i_rotation = Vector3(0.0f, 0.0f, 0.0f), const Vector3 &i_scale = Vector3(1.0f, 1.0f, 1.0f));
	FORCEINLINE ~Transform();

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

FORCEINLINE Transform::~Transform()
{
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
