#pragma once

#include "Core\Template\Array.h"
#include "Core\Template\Pointers.h"
#include "Object\GameObject.h"

enum class CollisionType : uint8{
	EBlock,
	EOverlap
};

struct CollisionElement;

class CollisionObject {
public:
	explicit FORCEINLINE CollisionObject(const WeakPtr<GameObject> &i_gameOject, CollisionType i_collideType = CollisionType::EBlock, bool i_active = true);
	FORCEINLINE ~CollisionObject();
	FORCEINLINE CollisionObject(const CollisionObject &i_other);
	FORCEINLINE CollisionObject& operator=(const CollisionObject &i_other);

	void DoCollision(size_t i_index, Array<CollisionElement> &i_collisionElements);

	FORCEINLINE WeakPtr<GameObject> GetGameObject() const { return gameObject_; }
	FORCEINLINE CollisionType GetCollideType() const { return collideType_; }
	FORCEINLINE bool GetActive() const { return active_; }

	FORCEINLINE void SetCollideType(CollisionType i_collideType) { collideType_ = i_collideType; }
	FORCEINLINE void SetActive(bool &i_active) { active_ = i_active; }
	FORCEINLINE bool IsValid() const;

private:
	void handleCollisionWith(const CollisionObject& i_other);

	WeakPtr<GameObject> gameObject_;
	CollisionType collideType_;
	bool active_;
};




// hold a strongPointer and a PhysicsObject it point to, for cache cohorency
struct CollisionElement {
	FORCEINLINE CollisionElement(const CollisionObject &i_collisionObject);
	FORCEINLINE CollisionElement(const CollisionElement &i_other);
	FORCEINLINE CollisionElement &operator=(const CollisionElement &i_other);

	StrongPtr<CollisionObject> Pointer;
private:
	CollisionObject Object;
};






FORCEINLINE CollisionObject::CollisionObject(const WeakPtr<GameObject> &i_gameOject, CollisionType i_collideType, bool i_active)
	: gameObject_(i_gameOject), collideType_(i_collideType), active_(i_active)
{
}

FORCEINLINE CollisionObject::~CollisionObject()
{
}

FORCEINLINE CollisionObject::CollisionObject(const CollisionObject &i_other)
	: gameObject_(i_other.gameObject_), collideType_(i_other.collideType_), active_(i_other.active_)
{
}

FORCEINLINE CollisionObject& CollisionObject::operator=(const CollisionObject &i_other)
{
	gameObject_ = i_other.gameObject_;
	collideType_ = i_other.collideType_;
	active_ = i_other.active_;
	return *this;
}

FORCEINLINE bool CollisionObject::IsValid() const
{
	return gameObject_;
}





FORCEINLINE CollisionElement::CollisionElement(const CollisionObject &i_collisionObject)
	: Object(i_collisionObject), Pointer(&Object)
{
}

FORCEINLINE CollisionElement::CollisionElement(const CollisionElement &i_other)
	: Object(i_other.Object), Pointer(&Object)
{

}

FORCEINLINE CollisionElement &CollisionElement::operator=(const CollisionElement &i_other)
{
	Object = i_other.Object;
	Pointer = &Object;
	return *this;
}



// for strong and weak pointer
// CollisionObject will be delete by array container
template<> FORCEINLINE void StrongPtr<CollisionObject>::ClearPointer()
{
	if (object_ != nullptr)
	{
		if (--(counter_->StrongCounter) == 0 || counter_->WeakCounter == 0)
		{
			delete counter_;
		}
	}
	object_ = nullptr;
	counter_ = nullptr;
}

template<> FORCEINLINE void WeakPtr<CollisionObject>::ClearPointer()
{
	object_ = nullptr;
	if (counter_ != nullptr)
	{
		if ((--counter_->WeakCounter) == 0 && counter_->StrongCounter == 0)
		{
			delete counter_;
		}
	}
	counter_ = nullptr;
}