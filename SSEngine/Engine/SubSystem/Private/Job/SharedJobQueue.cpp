#include "SubSystem\Job\SharedJobQueue.h"

#include "Core\CoreMinimal.h"


SharedJobQueue::SharedJobQueue() :
	shutdownRequested_( false )
{
	InitializeConditionVariable( &conditionVariable_ );
	InitializeCriticalSection( &criticalSection_ );
}

SharedJobQueue::~SharedJobQueue()
{
#ifdef _DEBUG
	if (name_ != nullptr)
		free(const_cast<char *>(name_));
#endif // _DEBUG
}

bool SharedJobQueue::Add( IJob & i_Job )
{
	bool bAdded = false;

	EnterCriticalSection( &criticalSection_ );
	if( shutdownRequested_ == false )
	{
		Jobs_.Push( &i_Job );
		bAdded = true;
	}
	LeaveCriticalSection( &criticalSection_ );

	if( bAdded )
		WakeConditionVariable( &conditionVariable_ );

	return bAdded;
}

IJob * SharedJobQueue::GetWhenAvailable()
{
	EnterCriticalSection( &criticalSection_ );

	if( Jobs_.Empty() && ( shutdownRequested_ == false ) )
	{
		BOOL result = SleepConditionVariableCS( &conditionVariable_, &criticalSection_, INFINITE );
		ASSERT( result != 0 );

		if( shutdownRequested_ == true )
		{
			LeaveCriticalSection( &criticalSection_ );
			return nullptr;
		}
	}

	IJob * pJob = nullptr;

	if( !Jobs_.Empty() )
	{
		pJob = Jobs_.Front();
		Jobs_.Pop();
	}

	LeaveCriticalSection( &criticalSection_ );
	return pJob;
}
