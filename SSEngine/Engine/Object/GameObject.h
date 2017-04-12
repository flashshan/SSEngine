#pragma once

#include "Core\Math\Vector3.h"
#include "Core\Math\Box2D.h"
#include "Component\Transform.h"


class GameObject
{
public:
	inline GameObject();
	explicit inline GameObject(const Transform &i_transform);
	inline GameObject(const Transform &i_transform, const Box2D &i_boundingBox);
	inline GameObject(const GameObject &i_other);
	inline GameObject(GameObject &&i_other);
	inline ~GameObject();

	inline GameObject& operator =(const GameObject &i_other);
	inline GameObject& operator =(GameObject &&i_other);

	FORCEINLINE bool GetActive() const { return isActive_; }
	FORCEINLINE void SetActive(bool i_value) { isActive_ = i_value; }

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
	FORCEINLINE void SetVelocity(const Vector3& i_velocity) { velocity_ = i_velocity; }

	FORCEINLINE Box2D GetBoundingBox() const { return boundingBox_; }
	FORCEINLINE void SetBoundingBox(const Box2D& i_boundingBox) { boundingBox_ = i_boundingBox; }

	FORCEINLINE Matrix GetObjectToWorld() const { return objectToWorld_; }
	FORCEINLINE void SetObjectToWorld(const Matrix& i_matrix) { objectToWorld_ = i_matrix; }
	FORCEINLINE Matrix GetWorldToObject() const { return worldToObject_; }
	FORCEINLINE void SetWorldToObject(const Matrix& i_matrix) { worldToObject_ = i_matrix; }
	void Update();

private:
	Matrix objectToWorld_;
	Matrix worldToObject_;
	Transform transform_;
	Vector3 velocity_;
	Box2D boundingBox_;
	bool isActive_;
};








// implement forceinline
// all gameObject are created with active = false
inline GameObject::GameObject()
	: transform_(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f)), velocity_(0.0f, 0.0f, 0.0f), boundingBox_(), isActive_(false)
{
}

inline GameObject::GameObject(const Transform &i_transform)
	: transform_(i_transform), velocity_(0.0, 0.0f, 0.0f), boundingBox_(), isActive_(false)
{
}

inline GameObject::GameObject(const Transform &i_transform, const Box2D &i_boundingBox)
	: transform_(i_transform), velocity_(0.0f, 0.0f, 0.0f), boundingBox_(i_boundingBox), isActive_(false)
{
}


inline GameObject::GameObject(const GameObject &i_other)
	: objectToWorld_(i_other.objectToWorld_), worldToObject_(i_other.worldToObject_), 
	transform_(i_other.transform_), velocity_(i_other.velocity_), boundingBox_(i_other.boundingBox_), isActive_(false)
{
}

// for value objects, copy is faster than swap
inline GameObject::GameObject(GameObject &&i_other)
	: isActive_(false)
{
	objectToWorld_ = i_other.objectToWorld_;
	worldToObject_ = i_other.worldToObject_;
	transform_ = i_other.transform_;
	velocity_ = i_other.velocity_;
	boundingBox_ = i_other.boundingBox_;
}

inline GameObject::~GameObject()
{
}

inline GameObject& GameObject::operator =(const GameObject &i_other)
{
	objectToWorld_ = i_other.objectToWorld_;
	worldToObject_ = i_other.worldToObject_;
	transform_ = i_other.transform_;
	velocity_ = i_other.velocity_;
	boundingBox_ = i_other.boundingBox_;
	isActive_ = false;
	return *this;
}

inline GameObject& GameObject::operator =(GameObject &&i_other)
{
	objectToWorld_ = i_other.objectToWorld_;
	worldToObject_ = i_other.worldToObject_;
	transform_ = i_other.transform_;
	velocity_ = i_other.velocity_;
	boundingBox_ = i_other.boundingBox_;
	isActive_ = false;
}

