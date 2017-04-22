#pragma once

#include "Core\CoreMinimal.h"
#include "Accumulator.h"
#include "Manager\RealTimeManager.h"

class ScopedTimer
{
public:
	FORCEINLINE ScopedTimer(Accumulator *i_accumulator);
	FORCEINLINE ~ScopedTimer();

private:
	float startTimeMS_;
	Accumulator *accumulator_;
};



FORCEINLINE ScopedTimer::ScopedTimer(Accumulator *i_accumulator) 
	: startTimeMS_(RealTimeManager::GetInstance()->GetCurrentTimeMS()), accumulator_(i_accumulator)
{

}

FORCEINLINE ScopedTimer::~ScopedTimer()
{
	*accumulator_ += RealTimeManager::GetInstance()->GetCurrentTimeMS() - startTimeMS_;
}
