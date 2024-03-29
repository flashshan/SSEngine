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
	LARGE_INTEGER currentTick, elapsedMS;
	QueryPerformanceCounter(&currentTick);

	if (lastFrameStartTick_.QuadPart) {
		elapsedMS.QuadPart = (currentTick.QuadPart - lastMarkStartTick_.QuadPart);
		lastMarkTime_ = static_cast<float>(elapsedMS.QuadPart) * 1000000 / static_cast<float>(frequency_.QuadPart);

		elapsedMS.QuadPart = (currentTick.QuadPart - lastFrameStartTick_.QuadPart);
		lastFrameTime_ = static_cast<float>(elapsedMS.QuadPart) * 1000000 / static_cast<float>(frequency_.QuadPart);
	}
	else {
		lastFrameTime_ = 13.3f; // Assume a 60Hz frame for first call.
	}
	lastFrameStartTick_ = currentTick;

#endif
}

