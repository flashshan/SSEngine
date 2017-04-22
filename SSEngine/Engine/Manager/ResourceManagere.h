#pragma once

#include <Windows.h>
#include <map>

#include "Glib\Glib.h"

#include "Core\Template\Pointers.h"
#include "Core\String\HashedString.h"

GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename);

// singleton class
class ResourceManager
{
public:
	static FORCEINLINE ResourceManager *CreateInstance();
	static FORCEINLINE ResourceManager *GetInstance();
	static FORCEINLINE void DestroyInstance();

	inline ~ResourceManager();

	FORCEINLINE GLib::Sprites::Sprite* AddSprite(const char *i_filePath);
	FORCEINLINE GLib::Sprites::Sprite* FindSprite(const char *i_filePath);

private:
	FORCEINLINE ResourceManager();
	FORCEINLINE ResourceManager(const ResourceManager &i_other) {}
	FORCEINLINE ResourceManager& operator=(const ResourceManager &i_other) {}

	static ResourceManager *globalInstance_;

private:
	std::map<HashedString, GLib::Sprites::Sprite*> spriteResources_;

	CRITICAL_SECTION criticalSection_;
};





// implement forceinline

FORCEINLINE ResourceManager *ResourceManager::CreateInstance()
{
	ASSERT(ResourceManager::globalInstance_ == nullptr);
	ResourceManager::globalInstance_ = new TRACK_NEW ResourceManager();
	return ResourceManager::globalInstance_;
}

FORCEINLINE ResourceManager *ResourceManager::GetInstance()
{
	ASSERT(ResourceManager::globalInstance_ != nullptr);
	return ResourceManager::globalInstance_;
}

FORCEINLINE void ResourceManager::DestroyInstance()
{
	ASSERT(ResourceManager::globalInstance_ != nullptr);
	delete ResourceManager::globalInstance_;
	ResourceManager::globalInstance_ = nullptr;
}


FORCEINLINE ResourceManager::ResourceManager()
{
	InitializeCriticalSection(&criticalSection_);
}


inline ResourceManager::~ResourceManager()
{
	for (auto it = spriteResources_.begin();it != spriteResources_.end(); ++it)
	{
		GLib::Sprites::Release(it->second);
	}
	spriteResources_.clear();
}

FORCEINLINE GLib::Sprites::Sprite* ResourceManager::AddSprite(const char *i_filePath)
{
	EnterCriticalSection(&criticalSection_);
	GLib::Sprites::Sprite *tempSprite = FindSprite(i_filePath);
	if (tempSprite != nullptr)
	{
		LeaveCriticalSection(&criticalSection_);
		return tempSprite;
	}

	// check if sprite exist, if not create one and store it in map
	tempSprite = CreateSprite(i_filePath);
	ASSERT(tempSprite != nullptr);

	spriteResources_.insert(std::pair<HashedString, GLib::Sprites::Sprite*>(HashedString(i_filePath), tempSprite));
	LeaveCriticalSection(&criticalSection_);
	return tempSprite;
}

FORCEINLINE GLib::Sprites::Sprite* ResourceManager::FindSprite(const char *i_filePath)
{
	HashedString filePathHash(i_filePath);

	EnterCriticalSection(&criticalSection_);
	auto it = spriteResources_.find(filePathHash);
	if (it == spriteResources_.end())
	{
		LeaveCriticalSection(&criticalSection_);
		return nullptr;
	}
	else
	{
		LeaveCriticalSection(&criticalSection_);
		return it->second;
	}
}