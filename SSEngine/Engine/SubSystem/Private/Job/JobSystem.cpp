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

	std::vector<HANDLE>	AllThreads;

	std::map<HashedString, JobQueueData *>::iterator iter = queues_.begin();
	while( iter != queues_.end() )
	{

		if( iter->second )
		{
			size_t count = iter->second->m_Runners.size();
			for( size_t i = 0; i < count; i++ )
			{
				JobRunnerData * pRunner = iter->second->m_Runners[i];
				if( pRunner  && pRunner->m_ThreadHandle != NULL )
						AllThreads.push_back( pRunner->m_ThreadHandle );
			}

			iter->second->m_SharedQueue.Shutdown();
		}
		++iter;
	}

	DEBUG_PRINT( "Job System: Waiting for Queue runner threads to shut down.\n" );

	DWORD result = WaitForMultipleObjects( static_cast<DWORD>( AllThreads.size() ), &AllThreads[0], TRUE, INFINITE );
	ASSERT( result == WAIT_OBJECT_0 );

	iter = queues_.begin();
	while( iter != queues_.end() )
	{
		if( iter->second )
		{
			size_t count = iter->second->m_Runners.size();
			for( size_t i = 0; i < count; i++ )
			{
				JobRunnerData * pRunner = iter->second->m_Runners[i];
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
	size_t runners = i_QueueData.m_Runners.size();

	JobRunnerData * pNewRunner = new JobRunnerData;
	
	pNewRunner->m_Input.i_pQueue = &i_QueueData.m_SharedQueue;
#ifdef _DEBUG
	char ThreadName[MAX_NAME_LENGTH];
	sprintf_s( ThreadName, MAX_NAME_LENGTH, "%s %d", i_QueueData.m_SharedQueue.GetName(), int( runners + 1 ) );
	pNewRunner->m_Input.m_ThreadName = ThreadName;
#endif

	pNewRunner->m_ThreadHandle = CreateThread( NULL, 0, JobRunner, &pNewRunner->m_Input, CREATE_SUSPENDED, &pNewRunner->m_ThreadID );
	ASSERT( pNewRunner->m_ThreadHandle != NULL );

	i_QueueData.m_Runners.push_back( pNewRunner );
	ResumeThread( pNewRunner->m_ThreadHandle );
}

void JobSystem::AddRunner( const HashedString & i_QueueName )
{
	std::map<HashedString, JobQueueData *>::iterator existing = queues_.find( i_QueueName );
	ASSERT( existing != queues_.end() );
	ASSERT( existing->second );

	AddRunner( *existing->second );
}

void JobSystem::CreateQueue( const char * i_pName, unsigned int i_numRunners )
{
	ASSERT( i_pName );
	
	HashedString	HashedName( i_pName );

	ASSERT( queues_.find( HashedName ) == queues_.end() );

	JobQueueData * pNewJobQueueData = new JobQueueData;
	pNewJobQueueData->m_SharedQueue.SetName( i_pName );

	queues_[HashedName] = pNewJobQueueData;

	for( unsigned int i = 0; i < i_numRunners; i++ )
		AddRunner( *pNewJobQueueData );
}

void JobSystem::RunJob( IJob & i_Job, const char * i_queueName )
{
	std::map<HashedString, JobQueueData *>::iterator existing = queues_.find(HashedString(i_queueName));
	ASSERT( existing != queues_.end() );
	ASSERT( existing->second );

	DEBUG_PRINT( "Job System: Adding Job to Queue \"%s\".\n", existing->second->m_SharedQueue.GetName() );
	existing->second->m_SharedQueue.Add( i_Job );
}
