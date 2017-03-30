#pragma once

#include <Windows.h>
#include "Core\CoreMinimal.h"

#define DESIRED_FPS		60.0f
#define DESIRED_FRAMETIME_MS	(1000.0f / DESIRED_FPS)
#define MAX_FRAMETIME_MS	(2 * DESIRED_FRAMETIME_MS)

class RealTimeManager
{
public:
	static FORCEINLINE RealTimeManager *CreateInstance();
	static FORCEINLINE RealTimeManager *GetInstance();
	static FORCEINLINE void DestroyInstance();

	void CalcLastFrameTime_ms();

	FORCEINLINE float GetLastFrameTimeMS() const;
	FORCEINLINE float GetLastFrameTimeS() const;

private:
	FORCEINLINE RealTimeManager();
	FORCEINLINE RealTimeManager(const RealTimeManager &i_other) {}

	static RealTimeManager *globalInstance_;

private:
	LARGE_INTEGER lastFrameStartTick_;
	float lastFrameTime_;
};







// implement forceinline
FORCEINLINE RealTimeManager *RealTimeManager::CreateInstance()
{
	ASSERT(RealTimeManager::globalInstance_ == nullptr);
	RealTimeManager::globalInstance_ = new RealTimeManager();
	return RealTimeManager::globalInstance_;
}

FORCEINLINE RealTimeManager *RealTimeManager::GetInstance()
{
	ASSERT(RealTimeManager::globalInstance_ != nullptr);
	return RealTimeManager::globalInstance_;
}

FORCEINLINE void RealTimeManager::DestroyInstance()
{
	ASSERT(RealTimeManager::globalInstance_ != nullptr);
	delete RealTimeManager::globalInstance_;
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