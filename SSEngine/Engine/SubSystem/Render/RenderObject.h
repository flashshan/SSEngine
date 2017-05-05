#pragma once

#include "Glib\Glib.h"

#include "Core\Template\Pointers.h"
#include "Object\Entity\GameObject.h"
#include "Manager\ResourceManagere.h"

class RenderObject {
public:
	FORCEINLINE RenderObject(const WeakPtr<GameObject> &i_gameOject, const char *i_filePath, uint32 i_priority);
	FORCEINLINE RenderObject(const RenderObject &i_other);
	FORCEINLINE RenderObject& operator=(const RenderObject &i_other);
	inline ~RenderObject();


	FORCEINLINE bool HasSprite() const;
	FORCEINLINE const GLib::Sprites::Sprite *GetSprite() const;
	FORCEINLINE void SetSprite(GLib::Sprites::Sprite *i_sprite);
	FORCEINLINE uint32 GetPriority() const;

	FORCEINLINE bool IsValid() const;
	FORCEINLINE void DoRender() const;

	FORCEINLINE bool operator<(const RenderObject &i_other) const;

private:
	WeakPtr<GameObject> gameObject_;
	GLib::Sprites::Sprite *sprite_;
	uint32 priority_;
};








// implement forceinline

FORCEINLINE RenderObject::RenderObject(const WeakPtr<GameObject> &i_gameObject, const char *i_filePath, uint32 i_priority)
	: gameObject_(i_gameObject), sprite_(ResourceManager::GetInstance()->AddSprite(i_filePath)), priority_(i_priority)
{
}

FORCEINLINE RenderObject::RenderObject(const RenderObject &i_other)
	: gameObject_(i_other.gameObject_), sprite_(i_other.sprite_), priority_(i_other.priority_) 
{
}

FORCEINLINE RenderObject& RenderObject::operator=(const RenderObject &i_other)
{
	gameObject_ = i_other.gameObject_;
	sprite_ = i_other.sprite_;
	priority_ = i_other.priority_;
	return *this;
}


FORCEINLINE bool RenderObject::HasSprite() const
{
	return sprite_ != nullptr;
}

FORCEINLINE const GLib::Sprites::Sprite *RenderObject::GetSprite() const 
{
	return sprite_; 
}

FORCEINLINE void RenderObject::SetSprite(GLib::Sprites::Sprite *i_sprite)
{
	sprite_ = i_sprite;
}

FORCEINLINE uint32 RenderObject::GetPriority() const 
{ 
	return priority_; 
}

FORCEINLINE bool RenderObject::IsValid() const
{
	return gameObject_;
}

FORCEINLINE void RenderObject::DoRender() const
{
	if (gameObject_->GetActive())
	{	
		if (sprite_ != nullptr)
		{
			/*gameObject_.Prefetch();*/
			GLib::Sprites::RenderSprite(*sprite_, GLib::Point2D((*gameObject_).GetLocation().X, (*gameObject_).GetLocation().Y), Math::DegreesToRadians((*gameObject_).GetRotation().Roll));
		}
	}
}


inline RenderObject::~RenderObject()
{
}

FORCEINLINE bool RenderObject::operator<(const RenderObject &i_other) const
{
	return priority_ < i_other.priority_;
}


// for strong and weak pointer
// RenderObject will be delete by array container
template<> FORCEINLINE void StrongPtr<RenderObject>::ClearPointer()
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

template<> FORCEINLINE void WeakPtr<RenderObject>::ClearPointer()
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