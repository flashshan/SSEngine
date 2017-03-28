#pragma once

#include "Glib\Glib.h"

#include "Core\Template\Pointers.h"
#include "Object\GameObject.h"

class RenderObject {
public:
	FORCEINLINE RenderObject(const StrongPtr<GameObject> &i_gameOject, GLib::Sprites::Sprite *i_sprite);
	inline ~RenderObject();

	FORCEINLINE bool HasSprite() const;
	FORCEINLINE void SetSprite(GLib::Sprites::Sprite *i_sprite);

	FORCEINLINE bool IsValid() const;
	FORCEINLINE void DoRender();

	FORCEINLINE void RemoveRenderObject();
private:
	// No copy constructor
	FORCEINLINE RenderObject(const RenderObject &i_other) {}

private:
	WeakPtr<GameObject> gameObject_;
	GLib::Sprites::Sprite *sprite_;
};








// implement forceinline

FORCEINLINE RenderObject::RenderObject(const StrongPtr<GameObject> &i_gameObject, GLib::Sprites::Sprite *i_sprite)
	: gameObject_(i_gameObject), sprite_(i_sprite)
{
}

FORCEINLINE bool RenderObject::HasSprite() const
{
	return sprite_ != nullptr;
}

FORCEINLINE void RenderObject::SetSprite(GLib::Sprites::Sprite *i_sprite)
{
	sprite_ = i_sprite;
}

FORCEINLINE bool RenderObject::IsValid() const
{
	return gameObject_;
}

FORCEINLINE void RenderObject::DoRender()
{
	if (gameObject_)
	{
		if (sprite_ != nullptr)
		{
			GLib::Sprites::RenderSprite(*sprite_, GLib::Point2D((*gameObject_).GetLocation().X, (*gameObject_).GetLocation().Y), 0.0f);
		}
	}
	else
	{
		RemoveRenderObject();
	}
}


inline RenderObject::~RenderObject()
{
}