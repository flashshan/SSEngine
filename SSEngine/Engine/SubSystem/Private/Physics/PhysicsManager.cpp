#include "SubSystem\Physics\PhysicsManager.h"


PhysicsManager *PhysicsManager::globalInstance_ = nullptr;

void PhysicsManager::PhysicsUpdate()
{
	size_t count = physicsElements_.Size();
	for(size_t i=0; i<count;++i)
	{
		if (physicsElements_[i].Pointer->IsValid())
		{
			physicsElements_[i].Pointer->DoPhysics();
		}
		else
		{
			RemoveByIndex(i);
			--i;
		}
	}
}


// go through array to delete certain element, seldomly used
void PhysicsManager::Remove(const WeakPtr<PhysicsObject> &i_physicsObject)
{
	if (!i_physicsObject) return;

	EnterCriticalSection(&criticalSection);
	size_t count = physicsElements_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		if (&(*physicsElements_[i].Pointer) == &(*i_physicsObject))
		{
			physicsElements_.NoOrderRemove(i);
			break;
		}
	}
	LeaveCriticalSection(&criticalSection);
}

