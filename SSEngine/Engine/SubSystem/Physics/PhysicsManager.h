#pragma once

#include <Windows.h>
#include <vector>

#include "Core\Template\Array.h"
#include "Core\Memory\New.h"
#include "PhysicsObject.h"

// hold a strongPointer and a PhysicsObject it point to, for cache cohorency
struct PhysicsElement {
	FORCEINLINE PhysicsElement(const PhysicsObject &i_physicsObject);
	FORCEINLINE PhysicsElement(const PhysicsElement &i_other);
	FORCEINLINE PhysicsElement &operator=(const PhysicsElement &i_other);

	StrongPtr<PhysicsObject> Pointer;
private:
	PhysicsObject Object;
};


// singleton class
class PhysicsManager
{
public:
	static const int32 maxPhysicsObjects = 10;

	static FORCEINLINE PhysicsManager *CreateInstance();
	static FORCEINLINE PhysicsManager *GetInstance();
	static FORCEINLINE void DestroyInstance();
	inline ~PhysicsManager();

	void PhysicsUpdate();
	FORCEINLINE WeakPtr<PhysicsObject> AddPhysicsObject(const PhysicsObject &i_physicsObject);
	void Remove(const WeakPtr<PhysicsObject> &i_physicsObject);
	FORCEINLINE void RemoveByIndex(size_t i_index);

private:
	FORCEINLINE PhysicsManager();
	FORCEINLINE PhysicsManager(PhysicsManager &i_other) {}
	FORCEINLINE PhysicsManager& operator=(PhysicsManager &i_othre) {}

	static PhysicsManager* globalInstance_;

private:
	Array<PhysicsElement> physicsElements_;
	
	CRITICAL_SECTION criticalSection;
};







// implement physics

FORCEINLINE PhysicsElement::PhysicsElement(const PhysicsObject &i_physicsObject)
	: Object(i_physicsObject), Pointer(&Object)
{
}

FORCEINLINE PhysicsElement::PhysicsElement(const PhysicsElement &i_other)
	: Object(i_other.Object), Pointer(&Object)
{

}

FORCEINLINE PhysicsElement &PhysicsElement::operator=(const PhysicsElement &i_other)
{
	Object = i_other.Object;
	Pointer = &Object;
	return *this;
}





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
	physicsElements_.Reserve(maxPhysicsObjects);
}

inline PhysicsManager::~PhysicsManager()
{
	physicsElements_.Clear();
}

FORCEINLINE void PhysicsManager::RemoveByIndex(size_t i_index)
{
	EnterCriticalSection(&criticalSection);
	physicsElements_.NoOrderRemove(i_index);
	LeaveCriticalSection(&criticalSection);
}

FORCEINLINE WeakPtr<PhysicsObject> PhysicsManager::AddPhysicsObject(const PhysicsObject &i_physicsObject)
{
	EnterCriticalSection(&criticalSection);
	physicsElements_.Add(PhysicsElement(i_physicsObject));
	LeaveCriticalSection(&criticalSection);
	return WeakPtr<PhysicsObject>(physicsElements_.Back().Pointer);
}