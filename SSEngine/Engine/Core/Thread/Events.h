#pragma once

#include <Windows.h>
#include "Core\CoreMinimal.h"


class ManualResetEvent
{
public:
	FORCEINLINE ManualResetEvent(bool i_bInitiallySignaled, const char * i_name = nullptr);
	inline ~ManualResetEvent();

	FORCEINLINE void Reset();

	FORCEINLINE bool Wait(DWORD i_WaitMilliseconds );
	FORCEINLINE void Signal();

	FORCEINLINE HANDLE GetHandle() const { return handle_; }

private:
	FORCEINLINE ManualResetEvent(const ManualResetEvent &i_other) {}
	FORCEINLINE ManualResetEvent& operator=(const ManualResetEvent &i_other) {}

private:
	HANDLE handle_;
};

class AutoResetEvent
{
public:
	FORCEINLINE AutoResetEvent(bool i_bInitiallySignaled, const char * i_name = nullptr);
	inline ~AutoResetEvent();

	FORCEINLINE bool Wait(DWORD i_WaitMilliseconds );
	FORCEINLINE void Signal();

	FORCEINLINE HANDLE GetHandle() const { return handle_; }

private:
	FORCEINLINE AutoResetEvent(const AutoResetEvent &i_other) {}
	FORCEINLINE AutoResetEvent operator=(const AutoResetEvent &i_other) {}

private:
	HANDLE handle_;

};








// implement forceinline

FORCEINLINE ManualResetEvent::ManualResetEvent(bool i_bInitiallySignaled, const char * i_name)
{
	WCHAR wchar[MAX_NAME_LENGTH];
	size_t converted = 0;
	mbstowcs_s(&converted, wchar, i_name, strlen(i_name) + 1);
	handle_ = CreateEvent(NULL, TRUE, i_bInitiallySignaled, wchar);
	ASSERT(handle_ != INVALID_HANDLE_VALUE);
}

inline ManualResetEvent::~ManualResetEvent()
{
	CloseHandle(handle_);
}

FORCEINLINE void ManualResetEvent::Reset()
{
	ResetEvent(handle_);
}

FORCEINLINE bool ManualResetEvent::Wait(DWORD i_WaitMilliseconds)
{
	DWORD result = WaitForSingleObject(handle_, i_WaitMilliseconds);
	ASSERT(((i_WaitMilliseconds == INFINITE) && (result == WAIT_OBJECT_0)) || (result == WAIT_TIMEOUT));
	return result == WAIT_OBJECT_0;
}

FORCEINLINE void ManualResetEvent::Signal()
{
	BOOL result = SetEvent(handle_);
	ASSERT(result == TRUE);
}





FORCEINLINE AutoResetEvent::AutoResetEvent(bool i_bInitiallySignaled, const char * i_name)
{
	WCHAR wchar[MAX_NAME_LENGTH];
	size_t converted = 0;
	mbstowcs_s(&converted, wchar, i_name, strlen(i_name) + 1);
	handle_ = CreateEvent(NULL, FALSE, i_bInitiallySignaled, wchar);
	ASSERT(handle_ != INVALID_HANDLE_VALUE);
}

inline AutoResetEvent::~AutoResetEvent()
{
	CloseHandle(handle_);
}

bool AutoResetEvent::Wait(DWORD i_WaitMilliseconds)
{
	DWORD result = WaitForSingleObject(handle_, i_WaitMilliseconds);
	ASSERT(((i_WaitMilliseconds == INFINITE) && (result == WAIT_OBJECT_0)) || (result == WAIT_TIMEOUT));
	return result == WAIT_OBJECT_0;
}

void AutoResetEvent::Signal(void)
{
	BOOL result = SetEvent(handle_);
	ASSERT(result == TRUE);
}

