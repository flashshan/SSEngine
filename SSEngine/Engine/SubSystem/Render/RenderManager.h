#pragma once

#include <set>
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

	void RenderUpdate();
	WeakPtr<RenderObject> AddRenderObject(const WeakPtr<GameObject> &i_gameObject, const char *i_filePath, const uint32 i_priority);
	void Remove(WeakPtr<RenderObject> i_renderObject);

private:
	FORCEINLINE RenderManager();
	FORCEINLINE RenderManager(RenderManager &i_other) {}
	FORCEINLINE RenderManager& operator=(RenderManager &i_other) {}

	static RenderManager *globalInstance_;

private:
	// for test, change to vector in future
	std::multiset<StrongPtr<RenderObject>> renderObjects_;
	//LinkedList<StrongPtr<RenderObject>> renderObjectList_;
	std::map<HashedString, GLib::Sprites::Sprite*> spriteResources_;

	CRITICAL_SECTION criticalSection;
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
	InitializeCriticalSection(&criticalSection);
}
