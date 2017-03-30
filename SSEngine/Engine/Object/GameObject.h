#pragma once

#include "Core\Math\Vector3.h"
#include "Core\Math\Box2D.h"
#include "Component\Transform.h"

class GameObject
{
public:
	FORCEINLINE GameObject();
	explicit FORCEINLINE GameObject(const Vector3 &i_location);
	explicit FORCEINLINE GameObject(const Transform &i_transform);
	FORCEINLINE GameObject(const GameObject &i_other);
	FORCEINLINE GameObject(GameObject &&i_other);
	inline ~GameObject();

	FORCEINLINE GameObject& operator =(const GameObject &i_other);
	FORCEINLINE GameObject& operator =(GameObject &&i_other);

	FORCEINLINE Vector3 GetLocation() const { return transform_.GetLocation(); }
	FORCEINLINE Vector3 GetRotation() const { return transform_.GetRotation(); }
	FORCEINLINE Vector3 GetScale() const { return transform_.GetScale(); }

	FORCEINLINE void SetLocation(const Vector3 &i_vector) { transform_.SetLocation(i_vector); }
	FORCEINLINE void SetRotation(const Vector3 &i_vector) { transform_.SetRotation(i_vector); }
	FORCEINLINE void SetScale(const Vector3 &i_vector) { transform_.SetScale(i_vector); }

	FORCEINLINE void Translate(const Vector3 &i_vector) { transform_.Translate(i_vector); }
	FORCEINLINE void Rotate(const Vector3 &i_vector) { transform_.Rotate(i_vector); }
	FORCEINLINE void Scale(const Vector3 &i_vector) { transform_.Scale(i_vector); }

	FORCEINLINE Vector3 GetVelocity() const { return velocity_; }
	FORCEINLINE Box2D GetBoundingBox() const { return boundingBox_; }

	FORCEINLINE void SetVelocity(const Vector3 i_velocity) { velocity_ = i_velocity; }
	FORCEINLINE void SetBoundingBox(const Box2D i_boundingBox) { boundingBox_ = i_boundingBox; }

	void Update();

private:
	Transform transform_;
	Vector3 velocity_;
	Box2D boundingBox_;
};








// implement forceinline

FORCEINLINE GameObject::GameObject()
	: transform_(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f)), velocity_(0.0f, 0.0f, 0.0f), boundingBox_()
{
}

FORCEINLINE GameObject::GameObject(const Vector3 &i_location)
	: transform_(i_location, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f)), velocity_(0.0f, 0.0f, 0.0f), boundingBox_()
{
}


FORCEINLINE GameObject::GameObject(const Transform &i_transform)
	: transform_(i_transform), velocity_(0.0, 0.0f, 0.0f), boundingBox_()
{
}

FORCEINLINE GameObject::GameObject(const GameObject &i_other)
	: transform_(i_other.transform_), velocity_(i_other.velocity_), boundingBox_(i_other.boundingBox_)
{
}

FORCEINLINE GameObject::GameObject(GameObject &&i_other)
{
	Basic::Swap(transform_, i_other.transform_);
	Basic::Swap(velocity_, i_other.velocity_);
	Basic::Swap(boundingBox_, i_other.boundingBox_);
}

inline GameObject::~GameObject()
{
}

FORCEINLINE GameObject& GameObject::operator =(const GameObject &i_other)
{
	transform_ = i_other.transform_;
	velocity_ = i_other.velocity_;
	boundingBox_ = i_other.boundingBox_;
	return *this;
}

FORCEINLINE GameObject& GameObject::operator =(GameObject &&i_other)
{
	Basic::Swap(transform_, i_other.transform_);
	Basic::Swap(velocity_, i_other.velocity_);
	Basic::Swap(boundingBox_, i_other.boundingBox_);
}

