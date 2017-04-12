#include "SubSystem\Physics\PhysicsManager.h"


PhysicsManager *PhysicsManager::globalInstance_ = nullptr;

void PhysicsManager::PhysicsUpdate()
{
	size_t count = physicsObjects_.Size();
	for(size_t i=0; i<count;++i)
	{
		if (physicsObjects_[i]->IsValid())
		{
			physicsObjects_[i]->UpdatePhysics();
		}
		else
		{
			RemoveByIndex(i);
			--i;
		}
	}
}

WeakPtr<PhysicsObject> PhysicsManager::AddPhysicsObject(const WeakPtr<GameObject> &i_gameObject, float i_mass, float i_drag)
{
	StrongPtr<PhysicsObject> newPhysicsObject = new TRACK_NEW PhysicsObject(i_gameObject, i_mass, i_drag);
	ASSERT(newPhysicsObject);
	EnterCriticalSection(&criticalSection);
	physicsObjects_.Add(newPhysicsObject);
	LeaveCriticalSection(&criticalSection);
	return WeakPtr<PhysicsObject>(physicsObjects_.Back());
}

void PhysicsManager::Remove(const WeakPtr<PhysicsObject> &i_physicsObject)
{
	if (!i_physicsObject) return;

	EnterCriticalSection(&criticalSection);
	size_t count = physicsObjects_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		if (&(*physicsObjects_[i]) == &(*i_physicsObject))
		{
			physicsObjects_.NoOrderRemove(i);
			break;
		}
	}
	LeaveCriticalSection(&criticalSection);
}

