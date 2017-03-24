#pragma once

#include "Glib\Glib.h"

#include "Core\CoreMinimal.h"
#include "Object\GameObject.h"

void * LoadFile(const char * i_pFilename, size_t & o_sizeFile);

GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename);

class RenderObject {
public:
	explicit FORCEINLINE RenderObject(GameObject *i_other, const char *i_fileName = nullptr);
	explicit FORCEINLINE RenderObject(const StrongPtr<GameObject> &i_strongPointer, const char *i_fileName = nullptr);
	
	FORCEINLINE bool HasSprite() const;
	FORCEINLINE void SetSprite(const char *i_fileName);
	FORCEINLINE void DoRender() const;

private:
	WeakPtr<GameObject> gameObject_;
	GLib::Sprites::Sprite *sprite_;
};







// implement forceinline

FORCEINLINE RenderObject::RenderObject(GameObject *i_other, const char *i_fileName)
{
	ASSERT(i_other != nullptr);
	StrongPtr<GameObject> strongPointer(i_other);
	gameObject_ = WeakPtr<GameObject>(strongPointer);
	if (i_fileName == nullptr)
	{
		sprite_ = nullptr;
	}
	else
	{
		sprite_ = CreateSprite(i_fileName);
	}
}


FORCEINLINE RenderObject::RenderObject(const StrongPtr<GameObject> &i_strongPointer, const char *i_fileName)
{
	gameObject_ = WeakPtr<GameObject>(i_strongPointer);
	if (i_fileName == nullptr)
	{
		sprite_ = nullptr;
	}
	else
	{
		sprite_ = CreateSprite(i_fileName);
	}
}


FORCEINLINE bool RenderObject::HasSprite() const
{
	return sprite_ == nullptr;
}

FORCEINLINE void RenderObject::SetSprite(const char *i_fileName)
{
	if (i_fileName == nullptr)
	{
		sprite_ = nullptr;
	}
	else
	{
		sprite_ = CreateSprite(i_fileName);
	}
}

FORCEINLINE void RenderObject::DoRender() const 
{
	GameObject tempObject = *gameObject_.Acquire();
	GLib::Sprites::RenderSprite(*sprite_, GLib::Point2D(tempObject.GetPosition().X, tempObject.GetPosition().Y), 0.0f);
}