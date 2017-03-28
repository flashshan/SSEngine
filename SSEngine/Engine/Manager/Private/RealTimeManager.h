#pragma once

#include <Windows.h>
#include <memory>

#include "Core\CoreMinimal.h"

#define DESIRED_FPS		60.0f
#define DESIRED_FRAMETIME_MS	(1000.0f / DESIRED_FPS)
#define MAX_FRAMETIME_MS	(2 * DESIRED_FRAMETIME_MS)

class RealTimeManager
{
public:
	static FORCEINLINE RealTimeManager *GetInstance();

	void CalcLastFrameTime_ms();

	FORCEINLINE float GetLastFrameTimeMS() const;
	FORCEINLINE float GetLastFrameTimeS() const;

private:
	FORCEINLINE RealTimeManager();

private:
	LARGE_INTEGER lastFrameStartTick_;
	float lastFrameTime_;
};







// implement forceinline

FORCEINLINE RealTimeManager *RealTimeManager::GetInstance()
{
	static RealTimeManager *globalInstance;
	if (globalInstance == nullptr)
	{
		globalInstance = new RealTimeManager();
	}

	return globalInstance;
}

FORCEINLINE float RealTimeManager::GetLastFrameTimeMS() const
{
	return lastFrameTime_;
}

FORCEINLINE float RealTimeManager::GetLastFrameTimeS() const
{
	return lastFrameTime_ / 1000;
}


FORCEINLINE RealTimeManager::RealTimeManager()
{
	lastFrameStartTick_.QuadPart = 0;
}