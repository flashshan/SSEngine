#pragma once

#include "Glib\Glib.h"

#include "Core\Template\Pointers.h"
#include "Object\GameObject.h"

class RenderObject {
public:
	FORCEINLINE RenderObject(const WeakPtr<GameObject> &i_gameOject, GLib::Sprites::Sprite *i_sprite, uint32 i_priority);
	inline ~RenderObject();

	FORCEINLINE bool HasSprite() const;
	FORCEINLINE const GLib::Sprites::Sprite *GetSprite() const;
	FORCEINLINE void SetSprite(GLib::Sprites::Sprite *i_sprite);
	FORCEINLINE uint32 GetPriority() const;

	FORCEINLINE bool IsValid() const;
	FORCEINLINE void DoRender();

	FORCEINLINE bool operator<(const RenderObject &i_other);
private:
	// No copy constructor
	FORCEINLINE RenderObject(const RenderObject &i_other) {}
	FORCEINLINE RenderObject& operator=(const RenderObject &i_other) {}

private:
	WeakPtr<GameObject> gameObject_;
	GLib::Sprites::Sprite *sprite_;
	uint32 priority_;
};








// implement forceinline

FORCEINLINE RenderObject::RenderObject(const WeakPtr<GameObject> &i_gameObject, GLib::Sprites::Sprite *i_sprite, uint32 i_priority)
	: gameObject_(i_gameObject), sprite_(i_sprite), priority_(i_priority)
{
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

FORCEINLINE void RenderObject::DoRender()
{
	if (gameObject_ && gameObject_->GetActive())
	{	
		if (sprite_ != nullptr)
		{
			/*gameObject_.Prefetch();*/
			GLib::Sprites::RenderSprite(*sprite_, GLib::Point2D((*gameObject_).GetLocation().X, (*gameObject_).GetLocation().Y), 0.0f);
		}
	}
}


inline RenderObject::~RenderObject()
{
}

FORCEINLINE bool RenderObject::operator<(const RenderObject &i_other)
{
	return priority_ < i_other.priority_;
}
