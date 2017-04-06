#include "Jobs\IJob.h"

CRITICAL_SECTION IJob::globalShutdown_;
bool IJob::globalIsShutdown_ = false;
