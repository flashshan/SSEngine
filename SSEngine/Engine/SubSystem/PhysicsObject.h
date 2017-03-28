#pragma once

#include "Core\Template\Pointers.h"
#include "Object\GameObject.h"

class PhysicsObject {
public:
	explicit FORCEINLINE PhysicsObject(const StrongPtr<GameObject> &i_gameOject, const float i_mass = 1.0f);
	FORCEINLINE ~PhysicsObject();

	FORCEINLINE void UpdatePhysics();

	FORCEINLINE float GetMass() const { return mass_; }
	FORCEINLINE Vector3 GetForce() const { return force_; }

	FORCEINLINE void SetMass(float i_mass) { mass_ = i_mass; }
	FORCEINLINE void SetForce(const Vector3 &i_force) { force_ = i_force; }
	FORCEINLINE bool IsValid() const;

	FORCEINLINE void RemovePhysicsObject();
private:
	// No copy constructor
	FORCEINLINE PhysicsObject(const PhysicsObject &i_other) {}

private:
	WeakPtr<GameObject> gameObject_;

	float mass_;
	Vector3 force_;
};



FORCEINLINE PhysicsObject::PhysicsObject(const StrongPtr<GameObject> &i_gameOject, const float i_mass)
	: gameObject_(i_gameOject), mass_(i_mass), force_(Vector3(0.0f, 0.0f, 0.0f))
{
}

FORCEINLINE PhysicsObject::~PhysicsObject()
{
}

FORCEINLINE void PhysicsObject::UpdatePhysics()
{
	if (gameObject_)
	{
		ASSERT(!Float::IsZero(mass_));
		(*gameObject_).SetVelocity((*gameObject_).GetVelocity() + (force_ / mass_));
		force_ = Vector3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		RemovePhysicsObject();
	}
}

FORCEINLINE bool PhysicsObject::IsValid() const
{
	return gameObject_;
}
