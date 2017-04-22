#pragma once

#include "Core\CoreMinimal.h"
#include "ProfileManager.h"
#include "Core\String\StringPool.h"

class Accumulator
{
public:
	FORCEINLINE Accumulator();
	FORCEINLINE Accumulator(const char *i_name);

	FORCEINLINE void operator+=(float i_time);

	FORCEINLINE float average() { return totalTime_ / totalCount_; }

private:
	float totalTime_;
	uint32 totalCount_;
	float minTime_, maxTime_;
};




FORCEINLINE Accumulator::Accumulator()
	: totalTime_(0.f), totalCount_(0),
	maxTime_(0.f), minTime_(Constants::BIG_NUMBER)
{
}

FORCEINLINE Accumulator::Accumulator(const char *i_name)
	: totalTime_(0.f), totalCount_(0),
	maxTime_(0.f), minTime_(Constants::BIG_NUMBER)
{
	ProfilerManager::GetInstance()->AddAccumulator(i_name, this);
}

FORCEINLINE void Accumulator::operator+=(float i_time)
{
	totalTime_ += i_time;
	totalCount_++;

	if (i_time > maxTime_)
		maxTime_ = i_time;
	else if (i_time < minTime_)
		minTime_ = i_time;
}



// for ProfilerManager

FORCEINLINE void ProfilerManager::AddAccumulator(const char * i_name, Accumulator *i_accumulator)
{
	accumulators_.insert(std::pair<HashedString, Accumulator *>(HashedString(i_name), i_accumulator));
}

FORCEINLINE Accumulator* ProfilerManager::GetAccumulator(const char *i_name)
{
	auto it = ProfilerManager::globalInstance_->accumulators_.find(HashedString(i_name));
	ASSERT(it != ProfilerManager::globalInstance_->accumulators_.end());

	return it->second;
}


FORCEINLINE ProfilerManager::ProfilerManager()
{
	InitializeCriticalSection(&criticalSection_);

	accumulators_.insert(std::pair<HashedString, Accumulator *>(HashedString("Controller"), new TRACK_NEW Accumulator()));
	accumulators_.insert(std::pair<HashedString, Accumulator *>(HashedString("Render"), new TRACK_NEW Accumulator()));
	accumulators_.insert(std::pair<HashedString, Accumulator *>(HashedString("Physics"), new TRACK_NEW Accumulator()));
	accumulators_.insert(std::pair<HashedString, Accumulator *>(HashedString("Collision"), new TRACK_NEW Accumulator()));

	accumulators_.insert(std::pair<HashedString, Accumulator *>(HashedString("PreCalculation"), new TRACK_NEW Accumulator()));
	accumulators_.insert(std::pair<HashedString, Accumulator *>(HashedString("EarlyUpdate"), new TRACK_NEW Accumulator()));
	accumulators_.insert(std::pair<HashedString, Accumulator *>(HashedString("Update"), new TRACK_NEW Accumulator()));
	accumulators_.insert(std::pair<HashedString, Accumulator *>(HashedString("ActualUpdate"), new TRACK_NEW Accumulator()));
	accumulators_.insert(std::pair<HashedString, Accumulator *>(HashedString("LateUpdate"), new TRACK_NEW Accumulator()));
}