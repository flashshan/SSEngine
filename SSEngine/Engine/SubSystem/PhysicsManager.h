#pragma once

#include "PhysicsObject.h"
#include "Core\Template\List.h"

class PhysicsManager
{
public:
	static FORCEINLINE PhysicsManager *GetInstance();
	FORCEINLINE ~PhysicsManager();

	void PhysicsUpdate() const;
	StrongPtr<PhysicsObject>& AddPhysicsObject(const StrongPtr<GameObject> &i_gameObject, const float i_mass);
	void RemoveFromList(PhysicsObject &i_physicsObject);

private:
	FORCEINLINE PhysicsManager();

private:
	LinkedList<StrongPtr<PhysicsObject>> physicsObjectList_;
};






// implement physics

FORCEINLINE PhysicsManager *PhysicsManager::GetInstance()
{
	static PhysicsManager *globalInstance;
	if (globalInstance == nullptr)
	{
		globalInstance = new PhysicsManager();
	}
	return globalInstance;
}

FORCEINLINE PhysicsManager::PhysicsManager()
{
}

FORCEINLINE PhysicsManager::~PhysicsManager()
{
	physicsObjectList_.Clear();
}




// for PhysicsObject

FORCEINLINE void PhysicsObject::RemovePhysicsObject()
{
	PhysicsManager::GetInstance()->RemoveFromList(*this);
}