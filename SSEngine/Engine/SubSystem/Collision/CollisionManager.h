#pragma once

#include <Windows.h>

#include "Core\Memory\New.h"
#include "CollisionObject.h"


// singleton class
class CollisionManager
{
public:
	static const int32 maxCollisionObjects = 100;

	static FORCEINLINE CollisionManager *CreateInstance();
	static FORCEINLINE CollisionManager *GetInstance();
	static FORCEINLINE void DestroyInstance();
	inline ~CollisionManager();

	void CollisionUpdate();
	FORCEINLINE WeakPtr<CollisionObject> AddCollisionObject(const CollisionObject &i_collisionObject);
	void Remove(const WeakPtr<CollisionObject> &i_collisionObject);
	FORCEINLINE void RemoveByIndex(size_t i_index);

private:
	FORCEINLINE CollisionManager();
	FORCEINLINE CollisionManager(CollisionManager &i_other) {}
	FORCEINLINE CollisionManager& operator=(CollisionManager &i_othre) {}

	static CollisionManager* globalInstance_;

private:
	Array<CollisionElement> collisionElements_;

	CRITICAL_SECTION criticalSection_;
};







// implement physics

FORCEINLINE CollisionManager *CollisionManager::CreateInstance()
{
	ASSERT(CollisionManager::globalInstance_ == nullptr);
	CollisionManager::globalInstance_ = new TRACK_NEW CollisionManager();
	return CollisionManager::globalInstance_;
}

FORCEINLINE CollisionManager *CollisionManager::GetInstance()
{
	ASSERT(CollisionManager::globalInstance_ != nullptr);
	return CollisionManager::globalInstance_;
}

FORCEINLINE void CollisionManager::DestroyInstance()
{
	ASSERT(CollisionManager::globalInstance_ != nullptr);
	delete CollisionManager::globalInstance_;
	CollisionManager::globalInstance_ = nullptr;
}

FORCEINLINE CollisionManager::CollisionManager()
{
	InitializeCriticalSection(&criticalSection_);
	collisionElements_.Reserve(CollisionManager::maxCollisionObjects);
}

inline CollisionManager::~CollisionManager()
{
	collisionElements_.Clear();
}

FORCEINLINE void CollisionManager::RemoveByIndex(size_t i_index)
{
	EnterCriticalSection(&criticalSection_);
	collisionElements_.NoOrderRemove(i_index);
	LeaveCriticalSection(&criticalSection_);
}

FORCEINLINE WeakPtr<CollisionObject> CollisionManager::AddCollisionObject(const CollisionObject &i_collisionObject)
{
	EnterCriticalSection(&criticalSection_);
	collisionElements_.Add(CollisionElement(i_collisionObject));
	LeaveCriticalSection(&criticalSection_);

	return WeakPtr<CollisionObject>(collisionElements_.Back().Pointer);
}