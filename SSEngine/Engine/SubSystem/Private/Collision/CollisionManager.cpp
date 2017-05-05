#include "SubSystem\Collision\CollisionManager.h"
#include "Object\Entity\Actor.h"

CollisionManager *CollisionManager::globalInstance_ = nullptr;

void CollisionManager::CollisionUpdate()
{
	size_t count = collisionElements_.Size();

	for (size_t i = 0; i<count;++i)
	{
		collisionElements_[i].Pointer->ResetCollideinfo();
	}
	for (size_t i = 0; i<collisionElements_.Size();++i)
	{
		if (collisionElements_[i].Pointer->IsValid())
		{
			collisionElements_[i].Pointer->DoCollision(i, collisionElements_);
		}
		else
		{
			RemoveByIndex(i);
			--i;
		}
	}
}


// go through all elements to delete a certain element, seldomly used
void CollisionManager::Remove(const WeakPtr<CollisionObject> &i_collisionObject)
{
	if (!i_collisionObject) return;

	EnterCriticalSection(&criticalSection_);
	size_t count = collisionElements_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		if (&(*collisionElements_[i].Pointer) == &(*i_collisionObject))
		{
			collisionElements_.NoOrderRemove(i);
			break;
		}
	}
	LeaveCriticalSection(&criticalSection_);
}

FORCEINLINE void CollisionManager::RemoveByIndex(size_t i_index)
{
	EnterCriticalSection(&criticalSection_);
	collisionElements_.NoOrderRemove(i_index);
	LeaveCriticalSection(&criticalSection_);
}