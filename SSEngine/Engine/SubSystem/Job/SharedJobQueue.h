#pragma once

#include <Windows.h>
#include "Core\Template\Queue.h"
#include "Jobs\IJob.h"


class SharedJobQueue
{
public:
	SharedJobQueue();
	~SharedJobQueue();

	FORCEINLINE void Shutdown();
	FORCEINLINE bool ShutdownRequested() const;

	bool Add( IJob & i_Job );
	IJob *GetWhenAvailable();


	FORCEINLINE const char* GetName() const { return name_; }
	FORCEINLINE void SetName(const char *i_name) { name_ = _strdup(i_name); }

private:
	Queue<IJob *> Jobs_;

	CONDITION_VARIABLE conditionVariable_;
	CRITICAL_SECTION criticalSection_;
	bool shutdownRequested_;

	const char *name_;
};







FORCEINLINE void SharedJobQueue::Shutdown()
{
	EnterCriticalSection(&criticalSection_);
	shutdownRequested_ = true;
	LeaveCriticalSection(&criticalSection_);

	WakeAllConditionVariable(&conditionVariable_);
}

FORCEINLINE bool SharedJobQueue::ShutdownRequested() const
{
	return shutdownRequested_;
}

