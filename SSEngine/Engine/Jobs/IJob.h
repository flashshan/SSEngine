#pragma once

#include <Windows.h>

class IJob
{
public:
	FORCEINLINE IJob(const char * i_name = nullptr);

	virtual	~IJob() {}
	virtual void Run() = 0;

	static FORCEINLINE void	Shutdown();
	static FORCEINLINE bool	ShutdownRequested();

#ifdef _DEBUG
	FORCEINLINE const char* GetName() const { return name_; }
	FORCEINLINE void SetName(const char *i_name) { name_ = i_name; }
#endif // _DEBUG

private:
	static CRITICAL_SECTION	criticalSection_;
	static bool	globalIsShutdown_;

#ifdef _DEBUG
	const char *name_;
#endif
};




// implement forceinline
FORCEINLINE IJob::IJob(const char *i_name)
#ifdef _DEBUG
	: name_(i_name ? i_name : "")
#endif // _DEBUG
{
}

FORCEINLINE bool IJob::ShutdownRequested()
{
	return globalIsShutdown_;
}

FORCEINLINE void IJob::Shutdown()
{
	static bool bInited = false;
	if (bInited == false)
	{
		InitializeCriticalSection(&criticalSection_);
		bInited = true;
	}

	EnterCriticalSection(&criticalSection_);
	globalIsShutdown_ = true;
	LeaveCriticalSection(&criticalSection_);
}
