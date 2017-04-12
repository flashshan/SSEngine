#pragma once

#include "Core\Template\Pointers.h"
#include "Object\GameObject.h"

class PhysicsObject {
public:
	explicit FORCEINLINE PhysicsObject(const WeakPtr<GameObject> &i_gameOject, float i_mass = 1.0f, float i_drag = 0.1f);
	FORCEINLINE ~PhysicsObject();

	void UpdatePhysics();

	FORCEINLINE float GetMass() const { return mass_; }
	FORCEINLINE Vector3 GetForce() const { return force_; }

	FORCEINLINE void SetMass(float i_mass) { mass_ = i_mass; }
	FORCEINLINE void SetForce(const Vector3 &i_force) { force_ = i_force; }
	FORCEINLINE bool IsValid() const;
private:
	// No copy constructor
	FORCEINLINE PhysicsObject(const PhysicsObject &i_other) {}
	FORCEINLINE PhysicsObject& operator=(const PhysicsObject &i_other) {}

private:
	WeakPtr<GameObject> gameObject_;
	float mass_;
	float drag_;

	Vector3 force_;
};



FORCEINLINE PhysicsObject::PhysicsObject(const WeakPtr<GameObject> &i_gameOject, const float i_mass, float i_drag)
	: gameObject_(i_gameOject), mass_(i_mass), drag_(i_drag), force_(Vector3(0.0f, 0.0f, 0.0f))
{
}

FORCEINLINE PhysicsObject::~PhysicsObject()
{
}

FORCEINLINE bool PhysicsObject::IsValid() const
{
	return gameObject_;
}


