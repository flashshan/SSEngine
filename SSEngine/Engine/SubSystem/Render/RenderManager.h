#pragma once

#include "Core\Template\List.h"
#include "Core\Memory\New.h"

#include "Core\String\HashedString.h"
#include "RenderObject.h"

GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename);

//singleton class
class RenderManager
{
public:
	static FORCEINLINE RenderManager *CreateInstance();
	static FORCEINLINE RenderManager *GetInstance();
	static FORCEINLINE void DestroyInstance();
	~RenderManager();

	void RenderUpdate() const;
	StrongPtr<RenderObject>& AddRenderObject(const StrongPtr<GameObject> &i_gameObject, const char *i_filePath);
	void RemoveFromList(RenderObject &i_renderObject);

private:
	FORCEINLINE RenderManager();
	FORCEINLINE RenderManager(RenderManager &i_other) {}
	FORCEINLINE RenderManager& operator=(RenderManager &i_other) {}

	static RenderManager *globalInstance_;

private:
	// for test, change to vector in future
	LinkedList<StrongPtr<RenderObject>> renderObjectList_;
	std::map<HashedString, GLib::Sprites::Sprite*> spriteResources_;
};






// implement

FORCEINLINE RenderManager *RenderManager::CreateInstance()
{
	ASSERT(RenderManager::globalInstance_ == nullptr);
	RenderManager::globalInstance_ = new TRACK_NEW RenderManager();
	return RenderManager::globalInstance_;
}

FORCEINLINE RenderManager *RenderManager::GetInstance()
{
	ASSERT(RenderManager::globalInstance_ != nullptr);
	return RenderManager::globalInstance_;
}

FORCEINLINE void RenderManager::DestroyInstance()
{
	ASSERT(RenderManager::globalInstance_ != nullptr);
	delete RenderManager::globalInstance_;
	RenderManager::globalInstance_ = nullptr;
}


FORCEINLINE RenderManager::RenderManager()
{
}



// for renderObject
FORCEINLINE void RenderObject::RemoveRenderObject()
{
	RenderManager::GetInstance()->RemoveFromList(*this);
}
