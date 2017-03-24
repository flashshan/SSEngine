#pragma once

#include "Core\CoreMinimal.h"

class GameTimeManager
{
public:
	static FORCEINLINE GameTimeManager *GetInstance();

private:
	GameTimeManager() {}
};



// implement forceinline

FORCEINLINE GameTimeManager* GameTimeManager::GetInstance()
{
	static GameTimeManager *globalInstance;
	if (globalInstance == nullptr)
	{
		globalInstance = new GameTimeManager();
	}
	return globalInstance;
}