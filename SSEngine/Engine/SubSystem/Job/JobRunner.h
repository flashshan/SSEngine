#pragma once

#include "SharedJobQueue.h"
#include "Core\String\StringPool.h"

struct JobRunnerInput
{
	SharedJobQueue *queue_;
#ifdef _DEBUG
	const char *threadName_;
#endif
};

DWORD WINAPI JobRunner( void * i_jobRunnerInput );
