#pragma once

#include <Windows.h>
#include "Core\Memory\New.h"

#define DESIRED_FPS		60.0f
#define DESIRED_FRAMETIME_MS	(1000.0f / DESIRED_FPS)
#define MAX_FRAMETIME_MS	(2 * DESIRED_FRAMETIME_MS)

// singleton class
class RealTimeManager
{
public:
	static FORCEINLINE RealTimeManager *CreateInstance();
	static FORCEINLINE RealTimeManager *GetInstance();
	static FORCEINLINE void DestroyInstance();

	void CalcLastFrameTime_ms();
	FORCEINLINE void SetLastTimeMark();

	FORCEINLINE float GetLastFrameTimeMS() const;
	FORCEINLINE float GetLastFrameTimeS() const;
	FORCEINLINE float GetLastMarkTimeMS() const;

	FORCEINLINE float GetCurrentTimeMS();
private:
	FORCEINLINE RealTimeManager();
	FORCEINLINE RealTimeManager(const RealTimeManager &i_other) {}
	FORCEINLINE RealTimeManager& operator=(const RealTimeManager &i_other) {}

	static RealTimeManager *globalInstance_;

private:
	LARGE_INTEGER startTick_;
	LARGE_INTEGER lastMarkStartTick_;
	LARGE_INTEGER lastFrameStartTick_;
	LARGE_INTEGER frequency_;
	float lastMarkTime_;
	float lastFrameTime_;
};







// implement forceinline
FORCEINLINE RealTimeManager *RealTimeManager::CreateInstance()
{
	ASSERT(RealTimeManager::globalInstance_ == nullptr);
	RealTimeManager::globalInstance_ = new TRACK_NEW RealTimeManager();
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
	RealTimeManager::globalInstance_ = nullptr;
}

FORCEINLINE float RealTimeManager::GetCurrentTimeMS()
{
	LARGE_INTEGER currentTick, elapsedMS;
	QueryPerformanceCounter(&currentTick);

	elapsedMS.QuadPart = (currentTick.QuadPart - startTick_.QuadPart);
	return static_cast<float>(elapsedMS.QuadPart) * 1000000 / static_cast<float>(frequency_.QuadPart);
}



FORCEINLINE float RealTimeManager::GetLastFrameTimeMS() const
{
	return lastFrameTime_ / 1000;
}

FORCEINLINE float RealTimeManager::GetLastFrameTimeS() const
{
	return lastFrameTime_ / 1000000;
}

FORCEINLINE float RealTimeManager::GetLastMarkTimeMS() const
{
	return lastMarkTime_ / 1000;
}

FORCEINLINE RealTimeManager::RealTimeManager()
{
	lastFrameStartTick_.QuadPart = 0;
	lastMarkStartTick_.QuadPart = 0;

	QueryPerformanceCounter(&startTick_);
	QueryPerformanceFrequency(&frequency_);

	//calculate once to get initial values
	CalcLastFrameTime_ms();
}

FORCEINLINE void RealTimeManager::SetLastTimeMark()
{
	LARGE_INTEGER currentTick;
	QueryPerformanceCounter(&currentTick);

	lastMarkStartTick_ = currentTick;
}


