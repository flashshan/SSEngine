#pragma once

#include <Windows.h>
#include <vector>

#include "Core\Template\Array.h"
#include "Core\Memory\New.h"
#include "PhysicsObject.h"

// singleton class
class PhysicsManager
{
public:
	static FORCEINLINE PhysicsManager *CreateInstance();
	static FORCEINLINE PhysicsManager *GetInstance();
	static FORCEINLINE void DestroyInstance();
	inline ~PhysicsManager();

	void PhysicsUpdate();
	WeakPtr<PhysicsObject> AddPhysicsObject(const WeakPtr<GameObject> &i_gameObject, float i_mass, float i_drag);
	void Remove(const WeakPtr<PhysicsObject> &i_physicsObject);
	FORCEINLINE void RemoveByIndex(size_t i_index);

private:
	FORCEINLINE PhysicsManager();
	FORCEINLINE PhysicsManager(PhysicsManager &i_other) {}
	FORCEINLINE PhysicsManager& operator=(PhysicsManager &i_othre) {}

	static PhysicsManager* globalInstance_;

private:
	Array<StrongPtr<PhysicsObject>> physicsObjects_;
	
	CRITICAL_SECTION criticalSection;
};






// implement physics

FORCEINLINE PhysicsManager *PhysicsManager::CreateInstance()
{
	ASSERT(PhysicsManager::globalInstance_ == nullptr);
	PhysicsManager::globalInstance_ = new TRACK_NEW PhysicsManager();
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
	PhysicsManager::globalInstance_ = nullptr;
}

FORCEINLINE PhysicsManager::PhysicsManager()
{
	InitializeCriticalSection(&criticalSection);
}

inline PhysicsManager::~PhysicsManager()
{
	physicsObjects_.Clear();
}

FORCEINLINE void PhysicsManager::RemoveByIndex(size_t i_index)
{
	EnterCriticalSection(&criticalSection);
	physicsObjects_.NoOrderRemove(i_index);
	LeaveCriticalSection(&criticalSection);
}

