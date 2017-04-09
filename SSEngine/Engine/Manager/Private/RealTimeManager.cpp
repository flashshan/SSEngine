#include "Manager\RealTimeManager.h"

RealTimeManager *RealTimeManager::globalInstance_ = nullptr;

void RealTimeManager::CalcLastFrameTime_ms()
{
#if defined( CONSTANT_FRAMETIME )
	m_LastFrameTime = DESIRED_FRAMETIME_MS;
#elif defined( CLAMP_FRAMETIME )
	if (LastFrameTime_ms > MAX_FRAMETIME_MS)
		m_LastFrameTime MAX_FRAMETIME_MS;
	else
		m_LastFrameTime lastFrameTime_ms;
#else
	LARGE_INTEGER currentTick, frequency, elapsedMS;
	QueryPerformanceCounter(&currentTick);

	if (lastFrameStartTick_.QuadPart) {
		QueryPerformanceFrequency(&frequency);
		elapsedMS.QuadPart = (currentTick.QuadPart - lastMarkStartTick_.QuadPart);
		lastMarkTime_ = (float)(elapsedMS.QuadPart) * 1000000 / float(frequency.QuadPart);

		elapsedMS.QuadPart = (currentTick.QuadPart - lastFrameStartTick_.QuadPart);
		lastFrameTime_ = (float)(elapsedMS.QuadPart) * 1000000 / float(frequency.QuadPart);
	}
	else {
//		elapsedMS.QuadPart = currentTick.QuadPart;

	//	QueryPerformanceFrequency(&frequency);
	//	lastFrameTime_ = (float)(elapsedMS.QuadPart) * 1000000 / float(frequency.QuadPart);
		lastFrameTime_ = 13.3f; // Assume a 60Hz frame for first call.
	}
	// Note start of this frame
	lastFrameStartTick_ = currentTick;

#endif
}

