#pragma once

#include "Core/CoreMinimal.h"
#include "Core/Math/Rotator.h"


enum class Mobility : uint8{
	EMovable,
	EStationary,
	EStatic
};

class Transform
{
public:
	FORCEINLINE Transform();
	FORCEINLINE Transform(const Vector3 &i_location, const Rotator &i_rotation = Rotator(0.0f, 0.0f, 0.0f), const Vector3 &i_scale = Vector3(1.0f, 1.0f, 1.0f), Mobility i_mobility = Mobility::EMovable);
	FORCEINLINE ~Transform();

	FORCEINLINE Vector3 GetLocation() const { return location_; }
	FORCEINLINE Rotator GetRotation() const { return rotation_; }
	FORCEINLINE Vector3 GetScale() const { return scale_; }
	FORCEINLINE Mobility GetMobility() const { return mobility_; }

	FORCEINLINE void SetLocation(const Vector3 &i_vector) { location_ += i_vector; }
	FORCEINLINE void SetRotation(const Rotator &i_rotator) { rotation_ += i_rotator; }
	FORCEINLINE void SetScale(const Vector3 &i_vector) { scale_ *= i_vector; }
	FORCEINLINE void SetMobility(Mobility i_mobility) { mobility_ = i_mobility; }

	FORCEINLINE void Translate(const Vector3 &i_vector);
	FORCEINLINE void Rotate(const Rotator &i_rotator);
	FORCEINLINE void Scale(const Vector3 &i_vector);

private:
	Vector3 location_;
	Rotator rotation_;
	Vector3 scale_;
	Mobility mobility_;
};




// implement forceinline

FORCEINLINE Transform::Transform()
	: location_(0.0f, 0.0f, 0.0f), rotation_(0.0f, 0.0f, 0.0f), scale_(1.0f, 1.0f, 1.0f), mobility_(Mobility::EMovable)
{
}

FORCEINLINE Transform::Transform(const Vector3 &i_location, const Rotator &i_rotation, const Vector3 &i_scale, Mobility i_mobility)
	: location_(i_location), rotation_(i_rotation), scale_(i_scale), mobility_(i_mobility)
{
}

FORCEINLINE Transform::~Transform()
{
}

FORCEINLINE void Transform::Translate(const Vector3 &i_vector)
{
	location_ += i_vector;
}

FORCEINLINE void Transform::Rotate(const Rotator &i_rotator)
{
	rotation_ += i_rotator;
}

FORCEINLINE void Transform::Scale(const Vector3 &i_vector)
{
	scale_ *= i_vector;
}

// implement forceinline
