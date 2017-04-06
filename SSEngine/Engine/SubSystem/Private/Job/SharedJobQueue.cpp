#include "SubSystem\Job\SharedJobQueue.h"

#include "Core\CoreMinimal.h"


SharedJobQueue::SharedJobQueue() :
	m_ShutdownRequested( false )
{
	InitializeConditionVariable( &m_WakeAndCheck );
	InitializeCriticalSection( &m_QueueAccess );
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

	EnterCriticalSection( &m_QueueAccess );
	if( m_ShutdownRequested == false )
	{
		m_Jobs.push( &i_Job );
		bAdded = true;
	}
	LeaveCriticalSection( &m_QueueAccess );

	if( bAdded )
		WakeConditionVariable( &m_WakeAndCheck );

	return bAdded;
}

IJob * SharedJobQueue::GetWhenAvailable()
{
	EnterCriticalSection( &m_QueueAccess );

	if( m_Jobs.empty() && ( m_ShutdownRequested == false ) )
	{
		BOOL result = SleepConditionVariableCS( &m_WakeAndCheck, &m_QueueAccess, INFINITE );
		ASSERT( result != 0 );

		if( m_ShutdownRequested == true )
		{
			LeaveCriticalSection( &m_QueueAccess );
			return nullptr;
		}
	}

	IJob * pJob = nullptr;

	if( !m_Jobs.empty() )
	{
		pJob = m_Jobs.front();
		m_Jobs.pop();
	}

	LeaveCriticalSection( &m_QueueAccess );
	return pJob;
}
