#pragma once

#include <Windows.h>
#include <queue>

#include "Jobs\IJob.h"


class SharedJobQueue
{
public:
	SharedJobQueue();
	~SharedJobQueue();

	inline void				Shutdown();
	inline bool				ShutdownRequested() const;

	bool					Add( IJob & i_Job );
	IJob *					GetWhenAvailable();

#ifdef _DEBUG
	FORCEINLINE const char* GetName() const { return name_; }
	FORCEINLINE void SetName(const char *i_name) { name_ = _strdup(i_name); }
#endif

private:
	std::queue<IJob *>		m_Jobs;

	CONDITION_VARIABLE		m_WakeAndCheck;
	CRITICAL_SECTION		m_QueueAccess;

	bool					m_ShutdownRequested;
#ifdef _DEBUG
	const char *name_;
#endif
};






inline void SharedJobQueue::Shutdown()
{
	EnterCriticalSection(&m_QueueAccess);
	m_ShutdownRequested = true;
	LeaveCriticalSection(&m_QueueAccess);

	WakeAllConditionVariable(&m_WakeAndCheck);
}

inline bool SharedJobQueue::ShutdownRequested() const
{
	return m_ShutdownRequested;
}

