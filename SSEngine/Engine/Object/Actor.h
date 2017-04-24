#pragma once

#include "Core\Template\Array.h"
#include "SubSystem\Render\RenderManager.h"
#include "SubSystem\Physics\PhysicsManager.h"
#include "SubSystem\Collision\CollisionManager.h"
#include "Component\IComponent.h"
#include "Core\String\StringPool.h"

#define NAME_LENGTH  50

class Actor
{
public:
	inline Actor();
	explicit inline Actor(const Transform &i_transform, const char *i_name = "defaultName", const char *i_type = "defaultType", bool i_static = false);
	inline Actor(const Actor &i_other);
	inline Actor(Actor &&i_other);
	virtual ~Actor();

	inline Actor& operator =(const Actor &i_other);
	inline Actor& operator =(Actor &&i_other);

	void InitCalculation();

	FORCEINLINE const char *GetName() const { return name_; }
	FORCEINLINE HashedString GetType() const { return type_; }
	FORCEINLINE WeakPtr<GameObject> GetGameObject() const { return gameObject_; }

	FORCEINLINE void SetName(const char *i_name) { name_ = i_name ? StringPool::GetInstance()->add(i_name) : StringPool::GetInstance()->add(""); }
	FORCEINLINE void SetType(const char *i_type) { type_ = i_type ? StringPool::GetInstance()->add(i_type) : StringPool::GetInstance()->add(""); }
	FORCEINLINE bool IsType(const char *i_type) { return (strcmp(type_, i_type) == 0); }
	
	FORCEINLINE bool GetCanCollide() const;
	FORCEINLINE void EnableCollision(bool i_value);
	FORCEINLINE bool GetActive() const { return isActive_; }
	FORCEINLINE void SetActive(bool i_value);

	FORCEINLINE Vector3 GetActorLocation() const { return (*gameObject_).GetLocation(); }
	FORCEINLINE Rotator GetActorRotation() const { return (*gameObject_).GetRotation(); }
	FORCEINLINE Vector3 GetActorScale() const { return (*gameObject_).GetScale(); }
	FORCEINLINE Vector3 GetActorVelocity() const { return (*gameObject_).GetVelocity(); }
	FORCEINLINE Box2D GetBoundingBox() const { return (*gameObject_).GetBoundingBox(); }

	FORCEINLINE void SetActorLocation(const Vector3 &i_location) { (*gameObject_).SetLocation(i_location); }
	FORCEINLINE void SetActorRotation(const Rotator &i_rotation) { (*gameObject_).SetRotation(i_rotation); }
	FORCEINLINE void SetActorScale(const Vector3 &i_scale) { (*gameObject_).SetScale(i_scale); }
	FORCEINLINE void SetActorVelocity(const Vector3 &i_velocity) { (*gameObject_).SetVelocity(i_velocity); }
	FORCEINLINE void SetBoundingBox(const Box2D &i_boundingBox) { (*gameObject_).SetBoundingBox(i_boundingBox); }

	FORCEINLINE void SetLocation2D(float i_X, float i_Y) { (*gameObject_).SetLocation(Vector3(i_X, i_Y, 0.0f)); }
	FORCEINLINE void SetRotation2D(float i_angle) { (*gameObject_).SetRotation(Rotator(0.0f, 0.0f, i_angle)); }
	FORCEINLINE void SetScale2D(float i_X, float i_Y) { (*gameObject_).SetScale(Vector3(i_X, i_Y, 1.0f)); }

	FORCEINLINE void AddForce(const Vector3 &i_force);

	void PreCalculation();
	virtual void EarlyUpdate() {}    // TO DO
	virtual void Update() {}
	void ActualUpdate();
	virtual void LateUpdate() {}     // TO DO

	FORCEINLINE void AddRenderObject(const RenderObject &i_renderObject);
	FORCEINLINE void RemoveRenderObject();
	FORCEINLINE WeakPtr<RenderObject> GetRenderObject() { return renderObject_; }

	FORCEINLINE void AddPhysicsObject(const PhysicsObject &i_physicsObject);
	FORCEINLINE void RemovePhysicsObject();
	FORCEINLINE WeakPtr<PhysicsObject> GetPhysicsObject() { return physicsObject_; }

	FORCEINLINE void AddCollisionObject(const CollisionObject &i_collisionObject);
	FORCEINLINE void RemoveCollisionObject();
	FORCEINLINE WeakPtr<CollisionObject> GetCollisionObject() { return collisionObject_; }
private:

	const char *name_;
	const char *type_;
	bool isActive_;
	//uint32 guid_;

private:
	StrongPtr<GameObject> gameObject_;
	WeakPtr<RenderObject> renderObject_;
	WeakPtr<PhysicsObject> physicsObject_;
	WeakPtr<CollisionObject> collisionObject_;

	Array<IComponent *> components_;
};







// implement forceinline
// all actors are created with active = false
inline Actor::Actor()
	: name_(StringPool::GetInstance()->add("defaultName")), type_(StringPool::GetInstance()->add("defaultType")), isActive_(false),
	gameObject_(new TRACK_NEW GameObject()), renderObject_(nullptr), physicsObject_(nullptr), collisionObject_(nullptr)
{
	gameObject_->SetOwner(this);
	if (gameObject_->GetStatic())
	{
		InitCalculation();
	}
}

inline Actor::Actor(const Transform &i_transform, const char *i_name, const char *i_type, bool i_static)
	: name_(StringPool::GetInstance()->add(i_name)), type_(StringPool::GetInstance()->add(i_type)), isActive_(false),
	gameObject_(new TRACK_NEW GameObject(i_transform, Box2D(), i_static)), renderObject_(nullptr), physicsObject_(nullptr), collisionObject_(nullptr)
{
	gameObject_->SetOwner(this);
	if (gameObject_->GetStatic())
	{
		InitCalculation();
	}
}

inline Actor::Actor(const Actor &i_other)
	: name_(i_other.name_), type_(i_other.type_), isActive_(false),
	gameObject_(i_other.gameObject_), renderObject_(i_other.renderObject_), collisionObject_(i_other.collisionObject_), components_(i_other.components_)
{
	gameObject_->SetOwner(this);
}

inline Actor::Actor(Actor &&i_other)
	: isActive_(false)
{
	name_ = i_other.name_;
	type_ = i_other.type_;
	Basic::Swap(gameObject_, i_other.gameObject_);
	Basic::Swap(renderObject_, i_other.renderObject_);
	Basic::Swap(collisionObject_, i_other.collisionObject_);
	Basic::Swap(components_, i_other.components_);
}



inline Actor& Actor::operator =(const Actor &i_other)
{
	name_ = i_other.name_; 
	type_ = i_other.type_;
	isActive_ = false;
	gameObject_ = i_other.gameObject_; 
	renderObject_ = i_other.renderObject_;
	collisionObject_ = i_other.collisionObject_;
	components_ = i_other.components_;
	return *this;
}

inline Actor& Actor::operator =(Actor &&i_other)
{
	name_ = i_other.name_;
	type_ = i_other.type_;
	isActive_ = false;
	Basic::Swap(gameObject_, i_other.gameObject_);
	Basic::Swap(renderObject_, i_other.renderObject_);
	Basic::Swap(collisionObject_, i_other.collisionObject_);
	Basic::Swap(components_, i_other.components_);
	return *this;
}


FORCEINLINE void Actor::SetActive(bool i_value)
{
	isActive_ = i_value;
	gameObject_->SetActive(i_value);
}

FORCEINLINE bool Actor::GetCanCollide() const
{
	if (collisionObject_)
	{
		return collisionObject_->GetActive();
	}
	else
	{
		return false;
	}
}

FORCEINLINE void Actor::EnableCollision(bool i_value)
{
	if (collisionObject_)
	{
		collisionObject_->SetActive(i_value);
	}
}


FORCEINLINE void Actor::AddRenderObject(const RenderObject &i_renderObject)
{
	if (renderObject_)
	{
		RemoveRenderObject();			// keep only single render object
	}
	renderObject_ = RenderManager::GetInstance()->AddRenderObject(i_renderObject);
}

FORCEINLINE void Actor::RemoveRenderObject()
{
	if (renderObject_)
	{
		RenderManager::GetInstance()->Remove(renderObject_);
	}
}

FORCEINLINE void Actor::AddPhysicsObject(const PhysicsObject &i_physicsObject)
{
	if (physicsObject_)
	{
		RemovePhysicsObject();			// keep only single physics object
	}
	physicsObject_ = PhysicsManager::GetInstance()->AddPhysicsObject(i_physicsObject);
}

FORCEINLINE void Actor::RemovePhysicsObject()
{
	if (physicsObject_)
	{
		PhysicsManager::GetInstance()->Remove(physicsObject_);
	}
}

FORCEINLINE void Actor::AddCollisionObject(const CollisionObject &i_collisionObject)
{
	if (collisionObject_)
	{
		RemoveCollisionObject();
	}
	collisionObject_ = CollisionManager::GetInstance()->AddCollisionObject(i_collisionObject);
}

FORCEINLINE void Actor::RemoveCollisionObject()
{
	if (collisionObject_)
	{
		CollisionManager::GetInstance()->Remove(collisionObject_);
	}
}

FORCEINLINE void Actor::AddForce(const Vector3 &i_force)
{
	if (physicsObject_)
	{
		physicsObject_->SetForce(i_force);
	}
}


