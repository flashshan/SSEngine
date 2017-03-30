#pragma once

#include "Core\Template\List.h"
#include "Core\Memory\New.h"

#include "PhysicsObject.h"


class PhysicsManager
{
public:
	static FORCEINLINE PhysicsManager *CreateInstance();
	static FORCEINLINE PhysicsManager *GetInstance();
	static FORCEINLINE void DestroyInstance();
	inline ~PhysicsManager();

	void PhysicsUpdate() const;
	StrongPtr<PhysicsObject>& AddPhysicsObject(const StrongPtr<GameObject> &i_gameObject, const float i_mass);
	void RemoveFromList(PhysicsObject &i_physicsObject);

private:
	FORCEINLINE PhysicsManager();
	FORCEINLINE PhysicsManager(PhysicsManager &i_other) {}

	static PhysicsManager* globalInstance_;

private:
	// for test, change to vector in future
	LinkedList<StrongPtr<PhysicsObject>> physicsObjectList_;
};






// implement physics

FORCEINLINE PhysicsManager *PhysicsManager::CreateInstance()
{
	ASSERT(PhysicsManager::globalInstance_ == nullptr);
	PhysicsManager::globalInstance_ = new PhysicsManager();
	return PhysicsManager::globalInstance_;
}

FORCEINLINE PhysicsManager *PhysicsManager::GetInstance()
{
	ASSERT(PhysicsManager::globalInstance_ != nullptr);
	return PhysicsManager::globalInstance_;
}

FORCEINLINE void PhysicsManager::DestroyInstance()
{
	ASSERT(PhysicsManager::globalInstance_ != nullptr);
	delete PhysicsManager::globalInstance_;
}

FORCEINLINE PhysicsManager::PhysicsManager()
{
}

inline PhysicsManager::~PhysicsManager()
{
	//physicsObjectList_.Clear();
}




// for PhysicsObject

FORCEINLINE void PhysicsObject::RemovePhysicsObject()
{
	PhysicsManager::GetInstance()->RemoveFromList(*this);
}