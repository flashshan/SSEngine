#pragma once

#include "Core\Template\List.h"
#include "Core\Template\Pointers.h"
#include "Core\String\HashedString.h"
#include "RenderObject.h"

void * LoadFile(const char * i_pFilename, size_t & o_sizeFile);

GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename);

class RenderManager
{
public:
	static FORCEINLINE RenderManager *GetInstance();
	FORCEINLINE ~RenderManager();

	void RenderUpdate() const;
	StrongPtr<RenderObject>& AddRenderObject(const StrongPtr<GameObject> &i_gameObject, const char *i_filePath);
	void RemoveFromList(RenderObject &i_renderObject);

private:
	FORCEINLINE RenderManager();

private:
	LinkedList<StrongPtr<RenderObject>> renderObjectList_;
	std::unordered_map<uint32, GLib::Sprites::Sprite*> spriteResources_;
};






// implement

FORCEINLINE RenderManager *RenderManager::GetInstance()
{
	static RenderManager *globalInstance;
	if (globalInstance == nullptr)
	{
		globalInstance = new RenderManager();
	}
	return globalInstance;
}


FORCEINLINE RenderManager::RenderManager()
{
	const int32 maxSprites = 1000;
	spriteResources_.reserve(maxSprites);
}

FORCEINLINE RenderManager::~RenderManager()
{
	renderObjectList_.Clear();
	for (auto it = spriteResources_.begin();it != spriteResources_.end(); ++it)
	{
		GLib::Sprites::Release(it->second);
	}
	spriteResources_.clear();
}


// for renderObject
FORCEINLINE void RenderObject::RemoveRenderObject()
{
	RenderManager::GetInstance()->RemoveFromList(*this);
}
