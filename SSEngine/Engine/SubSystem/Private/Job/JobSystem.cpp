#include "SubSystem\Job\JobSystem.h"

#include <Windows.h>
#include <map>

#include "Core\CoreMinimal.h"

#include "SubSystem\Job\JobRunner.h"
#include "SubSystem\Job\SharedJobQueue.h"

JobSystem* JobSystem::globalInstance_ = nullptr;

void JobSystem::Shutdown()
{
	DEBUG_PRINT( "Job System: Shutting down.\n" );

	IJob::Shutdown();

	Array<HANDLE>	AllThreads;

	std::map<HashedString, JobQueueData *>::iterator iter = queues_.begin();
	while( iter != queues_.end() )
	{

		if( iter->second )
		{
			size_t count = iter->second->runners_.Size();
			for( size_t i = 0; i < count; i++ )
			{
				JobRunnerData * pRunner = iter->second->runners_[i];
				if( pRunner  && pRunner->threadHandle_ != NULL )
						AllThreads.Add( pRunner->threadHandle_ );
			}

			iter->second->sharedQueue_.Shutdown();
		}
		++iter;
	}

	DEBUG_PRINT( "Job System: Waiting for Queue runner threads to shut down.\n" );

	DWORD result = WaitForMultipleObjects( static_cast<DWORD>( AllThreads.Size() ), &AllThreads[0], TRUE, INFINITE );
	ASSERT( result == WAIT_OBJECT_0 );

	iter = queues_.begin();
	while( iter != queues_.end() )
	{
		if( iter->second )
		{
			size_t count = iter->second->runners_.Size();
			for( size_t i = 0; i < count; i++ )
			{
				JobRunnerData * pRunner = iter->second->runners_[i];
				if( pRunner )
					delete pRunner;
			}

			delete iter->second;
		}

		++iter;
	}
	
	queues_.clear();

}

void JobSystem::AddRunner( JobQueueData & i_QueueData )
{
	size_t runners = i_QueueData.runners_.Size();

	JobRunnerData * pNewRunner = new JobRunnerData;
	
	pNewRunner->runnerInput_.queue_ = &i_QueueData.sharedQueue_;
#ifdef _DEBUG
	char ThreadName[MAX_NAME_LENGTH];
	sprintf_s( ThreadName, MAX_NAME_LENGTH, "%s %d", i_QueueData.sharedQueue_.GetName(), int( runners + 1 ) );
	pNewRunner->runnerInput_.threadName_ = ThreadName;
#endif

	pNewRunner->threadHandle_ = CreateThread( NULL, 0, JobRunner, &pNewRunner->runnerInput_, CREATE_SUSPENDED, &pNewRunner->threadID_ );
	ASSERT( pNewRunner->threadHandle_ != NULL );

	i_QueueData.runners_.Add( pNewRunner );
	ResumeThread( pNewRunner->threadHandle_ );
}

void JobSystem::AddRunner( const HashedString & i_QueueName )
{
	std::map<HashedString, JobQueueData *>::iterator existing = queues_.find( i_QueueName );
	ASSERT( existing != queues_.end() );
	ASSERT( existing->second );

	AddRunner( *existing->second );
}

void JobSystem::CreateQueue( const char * i_pName, uint32 i_numRunners )
{
	ASSERT( i_pName );
	
	HashedString	HashedName( i_pName );

	ASSERT( queues_.find( HashedName ) == queues_.end() );

	JobQueueData * pNewJobQueueData = new JobQueueData();
	pNewJobQueueData->sharedQueue_.SetName( i_pName );

	queues_[HashedName] = pNewJobQueueData;

	for( unsigned int i = 0; i < i_numRunners; i++ )
		AddRunner( *pNewJobQueueData );
}

void JobSystem::RunJob( IJob & i_Job, const char * i_queueName )
{
	std::map<HashedString, JobQueueData *>::iterator existing = queues_.find(HashedString(i_queueName));
	ASSERT( existing != queues_.end() );
	ASSERT( existing->second );

	DEBUG_PRINT( "Job System: Adding Job to Queue \"%s\".\n", existing->second->sharedQueue_.GetName() );
	existing->second->sharedQueue_.Add( i_Job );
}
