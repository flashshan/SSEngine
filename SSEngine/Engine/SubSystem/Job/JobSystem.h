#pragma once

#include "Core\String\HashedString.h"
#include "Core\Memory\New.h"
#include "JobRunner.h"
#include "Core\Template\Array.h"

class IJob;

struct JobRunnerData
{
	HANDLE threadHandle_;
	DWORD threadID_;
	JobRunnerInput runnerInput_;
};

struct JobQueueData
{
	SharedJobQueue sharedQueue_;
	Array<JobRunnerData *> runners_;
};


class JobSystem {
public:
	static FORCEINLINE JobSystem* CreateInstance();
	static FORCEINLINE JobSystem* GetInstance();
	static FORCEINLINE void DestroyInstance();

	void CreateQueue(const char * i_pName, unsigned int i_numRunners);
	void AddRunner(const HashedString & i_QueueName);
	void AddRunner(JobQueueData & i_QueueData);
	void RunJob(IJob & i_Job, const char *i_queueName);
	void Shutdown();

private:
	FORCEINLINE JobSystem();
	FORCEINLINE JobSystem(JobSystem &i_other) {}
	FORCEINLINE JobSystem& operator=(JobSystem &i_other) {}

	static JobSystem *globalInstance_;

	std::map<HashedString, JobQueueData *> queues_;
};





// implement forceinline

FORCEINLINE JobSystem* JobSystem::CreateInstance()
{
	ASSERT(globalInstance_ == nullptr);
	globalInstance_ = new TRACK_NEW JobSystem();
	return globalInstance_;
}

FORCEINLINE JobSystem* JobSystem::GetInstance()
{
	ASSERT(globalInstance_ != nullptr);
	return globalInstance_;
}

FORCEINLINE void JobSystem::DestroyInstance()
{
	ASSERT(globalInstance_ != nullptr);
	delete globalInstance_;
}

FORCEINLINE JobSystem::JobSystem()
{
}
