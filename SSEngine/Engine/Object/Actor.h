#pragma once

#include "SubSystem\RenderManager.h"
#include "SubSystem\PhysicsManager.h"
#include "Core\String\HashedString.h"
#include "Component\IComponent.h"
#include "Core\String\StringPool.h"

#define NAME_LENGTH  50

class Actor
{
public:
	FORCEINLINE Actor();
	explicit FORCEINLINE Actor(const Transform &i_transform, const char *i_name = "defaultName", const char *i_type = "defaultType");
	FORCEINLINE Actor(const Actor &i_other);
	FORCEINLINE Actor(Actor &&i_other);
	virtual ~Actor();

	FORCEINLINE Actor& operator =(const Actor &i_other);
	FORCEINLINE Actor& operator =(Actor &&i_other);

	FORCEINLINE char *GetName() const { return name_; }
	FORCEINLINE HashedString GetType() const { return type_; }
	FORCEINLINE GameObject &GetGameObject() const { return *gameObject_; }

	FORCEINLINE void SetName(const char *i_name) { name_ = _strdup(i_name); }
	FORCEINLINE void SetType(const char *i_type) { type_ = HashedString(i_type); }
	FORCEINLINE bool IsType(const char *i_type)	{ return type_ == HashedString(i_type); }
	
	FORCEINLINE Vector3 GetActorLocation() const { return (*gameObject_).GetLocation(); }
	FORCEINLINE Vector3 GetActorRotation() const { return (*gameObject_).GetRotation(); }
	FORCEINLINE Vector3 GetActorScale() const { return (*gameObject_).GetScale(); }
	FORCEINLINE Vector3 GetActorVelocity() const { return (*gameObject_).GetVelocity(); }

	FORCEINLINE void SetActorLocation(const Vector3 &i_location) { (*gameObject_).SetLocation(i_location); }
	FORCEINLINE void SetActorRotation(const Vector3 &i_rotation) { (*gameObject_).SetRotation(i_rotation); }
	FORCEINLINE void SetActorScale(const Vector3 &i_scale) { (*gameObject_).SetScale(i_scale); }
	FORCEINLINE void SetActorVelocity(const Vector3 &i_velocity) { (*gameObject_).SetVelocity(i_velocity); }
	
	FORCEINLINE void SetLocation2D(const float i_X, const float i_Y) { (*gameObject_).SetLocation(Vector3(i_X, i_Y, 0.0f)); }
	FORCEINLINE void SetRotation2D(const float i_angle) { (*gameObject_).SetRotation(Vector3(0.0f, 0.0f, i_angle)); }
	FORCEINLINE void SetScale2D(const float i_X, const float i_Y) { (*gameObject_).SetScale(Vector3(i_X, i_Y, 1.0f)); }

	FORCEINLINE void AddForce(const Vector3 &i_force);

	virtual void EarlyUpdate() {}    // TO DO
	virtual void Update();
	virtual void LateUpdate() {}     // TO DO

	FORCEINLINE void AddRenderObject(const char *i_filePath);
	FORCEINLINE void RemoveRenderObject();

	FORCEINLINE void AddPhysicsObject(float i_mass);
	FORCEINLINE void RemovePhysicsObject();

private:
	char *name_;
	HashedString type_;
	//uint32 guid_;

private:
	StrongPtr<GameObject> gameObject_;
	WeakPtr<RenderObject> renderObject_;
	WeakPtr<PhysicsObject> physicsObject_;

	std::vector<IComponent *> components_;
};







// implement forceinline

FORCEINLINE Actor::Actor()
	: name_(StringPool::GetInstance()->add("defaultName")), type_(HashedString("defaultType")), 
	gameObject_(new GameObject()), renderObject_(nullptr), physicsObject_(nullptr)
{
}

FORCEINLINE Actor::Actor(const Transform &i_transform, const char *i_name, const char *i_type)
	: name_(StringPool::GetInstance()->add(i_name)), type_(HashedString(i_type)), 
	gameObject_(new GameObject(i_transform)), renderObject_(nullptr), physicsObject_(nullptr)
{
}

FORCEINLINE Actor::Actor(const Actor &i_other)
	: name_(i_other.name_), type_(i_other.type_), gameObject_(i_other.gameObject_), renderObject_(i_other.renderObject_)
{
	components_ = i_other.components_;
}

FORCEINLINE Actor::Actor(Actor &&i_other)
{
	Basic::Swap(name_, i_other.name_);
	Basic::Swap(type_, i_other.type_);
	Basic::Swap(gameObject_, i_other.gameObject_);
	Basic::Swap(renderObject_, i_other.renderObject_);
	Basic::Swap(components_, i_other.components_);
}



FORCEINLINE Actor& Actor::operator =(const Actor &i_other)
{
	name_ = i_other.name_; 
	type_ = i_other.type_;
	gameObject_ = i_other.gameObject_; 
	renderObject_ = i_other.renderObject_;
	components_ = i_other.components_;
	return *this;
}

FORCEINLINE Actor& Actor::operator =(Actor &&i_other)
{
	Basic::Swap(name_, i_other.name_);
	Basic::Swap(type_, i_other.type_);
	Basic::Swap(gameObject_, i_other.gameObject_);
	Basic::Swap(renderObject_, i_other.renderObject_);
	Basic::Swap(components_, i_other.components_);
	return *this;
}


FORCEINLINE void Actor::AddRenderObject(const char *i_filePath)
{
	if (renderObject_)
	{
		RemoveRenderObject();			// keep only single render object
	}
	renderObject_ = RenderManager::GetInstance()->AddRenderObject(gameObject_, i_filePath);
}

FORCEINLINE void Actor::RemoveRenderObject()
{
	if (renderObject_)
	{
		RenderManager::GetInstance()->RemoveFromList(*renderObject_);
	}
}

FORCEINLINE void Actor::AddPhysicsObject(float i_mass)
{
	if (physicsObject_)
	{
		RemovePhysicsObject();			// keep only single physics object
	}
	physicsObject_ = PhysicsManager::GetInstance()->AddPhysicsObject(gameObject_, i_mass);
}

FORCEINLINE void Actor::RemovePhysicsObject()
{
	if (physicsObject_)
	{
		PhysicsManager::GetInstance()->RemoveFromList(*physicsObject_);
	}
}

FORCEINLINE void Actor::AddForce(const Vector3 &i_force)
{
	if (physicsObject_)
	{
		physicsObject_->SetForce(i_force);
	}
}


