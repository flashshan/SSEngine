#pragma once

#include "SharedJobQueue.h"


struct JobRunnerInput
{
	SharedJobQueue *queue_;
#ifdef _DEBUG
	std::string	threadName_;
#endif
};

DWORD WINAPI JobRunner( void * i_jobRunnerInput );
