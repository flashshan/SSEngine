#include "SubSystem\Job\JobRunner.h"

#include "Core\CoreMinimal.h"

DWORD WINAPI JobRunner( void * i_pThreadInput )
{
	ASSERT( i_pThreadInput );
	ASSERT( ( (JobRunnerInput *) i_pThreadInput )->i_pQueue );

	JobRunnerInput * pInput = reinterpret_cast<JobRunnerInput *>( i_pThreadInput );
 	SharedJobQueue * pQueue = reinterpret_cast<SharedJobQueue *>( pInput->i_pQueue );

	//DEBUG_PRINT( "JobRunner \"%s\": Starting Queue \"%s\".\n", pInput->m_ThreadName.c_str(), pQueue->GetName() ? pQueue->GetName() : "Unknown" );

	bool bDone = false;

	do
	{
		DEBUG_PRINT( "JobRunner \"%s\": Acquiring Job.\n", pInput->m_ThreadName.c_str() );
		IJob * pJob = pQueue->GetWhenAvailable();
		if( pJob )
		{
			DEBUG_PRINT( "JobRunner \"%s\": Starting Job \"%s\" on Processor %d.\n", pInput->m_ThreadName.c_str(), pJob->GetName() ? pJob->GetName() : "Unknown", GetCurrentProcessorNumber() );

			pJob->Run();

			DEBUG_PRINT( "JobRunner \"%s\": Finished Job \"%s\".\n", pInput->m_ThreadName.c_str(), pJob->GetName() ? pJob->GetName() : "Unknown" );
			delete pJob;

			bDone = pQueue->ShutdownRequested();
		}
		else
			bDone = pQueue->ShutdownRequested();

	} while( bDone == false );

#ifdef _DEBUG
	DEBUG_PRINT( "JobRunner \"%s\": Shutting down.\n", pInput->m_ThreadName.c_str() );
#endif
	return 0;
}