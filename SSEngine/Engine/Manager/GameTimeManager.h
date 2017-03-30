#pragma once

#include "Core\CoreMinimal.h"
#include "Core\Memory\New.h"

// TO DO Functions

class GameTimeManager
{
public:
	static FORCEINLINE GameTimeManager *CreateInstance();
	static FORCEINLINE GameTimeManager *GetInstance();
	static FORCEINLINE void DestroyInstance();

private:
	FORCEINLINE GameTimeManager();
	FORCEINLINE GameTimeManager(const GameTimeManager &i_other) {}

	static GameTimeManager* globalInstance_;
};




// implement forceinline
FORCEINLINE GameTimeManager *GameTimeManager::CreateInstance()
{
	ASSERT(GameTimeManager::globalInstance_ == nullptr);
	GameTimeManager::globalInstance_ = new GameTimeManager();
	return GameTimeManager::globalInstance_;
}

FORCEINLINE GameTimeManager* GameTimeManager::GetInstance()
{
	ASSERT(GameTimeManager::globalInstance_ != nullptr);
	return GameTimeManager::globalInstance_;
}

FORCEINLINE void GameTimeManager::DestroyInstance()
{
	ASSERT(GameTimeManager::globalInstance_ != nullptr);
	delete GameTimeManager::globalInstance_;
	GameTimeManager::globalInstance_ = nullptr;
}


FORCEINLINE GameTimeManager::GameTimeManager()
{
}
