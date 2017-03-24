#pragma once

#include "Core\Math\Vector3.h"

class GameObject
{
public:
	FORCEINLINE GameObject();
	explicit FORCEINLINE GameObject(const Vector3 &i_Position);
	FORCEINLINE GameObject(const GameObject &i_other);
	FORCEINLINE GameObject(GameObject &&i_other);
	inline ~GameObject();

	FORCEINLINE void operator =(const GameObject &i_other);
	FORCEINLINE void operator =(GameObject &&i_other);

	FORCEINLINE Vector3 GetPosition() const { return position_; }
	FORCEINLINE void SetPosition(const Vector3 &i_Position) { position_ = i_Position; }

private:
	Vector3 position_;
};








// implement forceinline

FORCEINLINE GameObject::GameObject()
	: position_(0.0f, 0.0f, 0.0f)
{
}

FORCEINLINE GameObject::GameObject(const Vector3 &i_Position)
	: position_(i_Position)
{
}

FORCEINLINE GameObject::GameObject(const GameObject &i_other)
	: position_(i_other.GetPosition())
{
}

FORCEINLINE GameObject::GameObject(GameObject &&i_other)
{
	Basic::Swap(position_, i_other.position_);
}

inline GameObject::~GameObject()
{
}

FORCEINLINE void GameObject::operator =(const GameObject &i_other)
{
	position_ = i_other.GetPosition();
}

FORCEINLINE void GameObject::operator =(GameObject &&i_other)
{
	Basic::Swap(position_, i_other.position_);
}