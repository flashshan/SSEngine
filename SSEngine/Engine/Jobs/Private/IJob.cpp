#include "Jobs\IJob.h"

CRITICAL_SECTION IJob::criticalSection_;
bool IJob::globalIsShutdown_ = false;
