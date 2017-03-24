#pragma once

#include "SubSystem\RenderManager.h"
#include "SubSystem\PhysicsManager.h"
#include "Core\String\HashedString.h"
//#include ""

#define NAME_LENGTH  50

class Actor
{
public:
	FORCEINLINE Actor();
	explicit FORCEINLINE Actor(const Vector3 &i_Position, const char *i_name = "defaultName", const char *i_type = "defaultType");
	FORCEINLINE Actor(const Actor &i_other);
	FORCEINLINE Actor(const Actor &&i_other);
	inline virtual ~Actor();

	FORCEINLINE void operator =(const GameObject &i_other);
	FORCEINLINE void operator =(const GameObject &&i_other);

	FORCEINLINE char *GetName() const { return name_; }

	FORCEINLINE void SetName(const char *i_name) { name_ = strdup(i_name); }

	FORCEINLINE void AddRenderObject(RenderObject *i_renderObject);
	FORCEINLINE void RemoveRenderObject();


private:
	char *name_;
	HashedString type_;
	uint32 guid_;

private:
	WeakPtr<RenderObject> renderObject_;
	//StrongPtr<PhysicsObject> physicsObject_;

};








// implement forceinline
//
//FORCEINLINE GameObject::GameObject()
//	: position_(0.0f, 0.0f, 0.0f), name_("default")
//{
//}
//
//FORCEINLINE GameObject::GameObject(const Vector3 &i_Position, const char *i_name)
//	: position_(i_Position), name_(strdup(i_name))
//{
//}
//
//FORCEINLINE GameObject::GameObject(const GameObject &i_other)
//	: position_(i_other.GetPosition()), name_(strdup(i_other.GetName()))
//{
//}
//
//inline GameObject::~GameObject()
//{
//	RemoveRenderObject();
//	delete name_;
//}
//
//FORCEINLINE void GameObject::operator =(const GameObject &i_other)
//{
//	position_ = i_other.GetPosition();
//
//}
//
//FORCEINLINE void GameObject::operator =(const GameObject &&i_other)
//{
//
//}
//
//FORCEINLINE void GameObject::AddRenderObject(RenderObject *i_renderObject)
//{
//	RemoveRenderObject();
//
//	ASSERT(i_renderObject != nullptr);
//	renderObject_ = WeakPtr<RenderObject>(RenderManager::GetInstance()->AddToRenderList(i_renderObject));
//}
//
//FORCEINLINE void GameObject::RemoveRenderObject()
//{
//	if (renderObject_)
//	{
//		RenderManager::GetInstance()->RemoveFromList(renderObject_.Acquire());
//		renderObject_.ClearPointer();
//	}
//}