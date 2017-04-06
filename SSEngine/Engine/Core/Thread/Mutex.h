#pragma once

#include <Windows.h>
#include "Core\CoreMinimal.h"

class Mutex
{
public:
	FORCEINLINE Mutex( bool i_bTakeOwnership = false, const char * i_name = nullptr );
	inline ~Mutex();

	FORCEINLINE bool TryLock();
	FORCEINLINE void Lock();
	FORCEINLINE bool Lock( DWORD i_WaitMilliseconds );
	FORCEINLINE void Unlock();

	FORCEINLINE HANDLE GetHandle() const { return handle_; }

private:
	FORCEINLINE Mutex(const Mutex &i_other) {}
	FORCEINLINE Mutex& operator=(const Mutex &i_other) {}

private:
	HANDLE handle_;
};






Mutex::Mutex(bool i_bTakeOwnership, const char * i_name)
{
	WCHAR wchar[MAX_NAME_LENGTH];
	size_t converted = 0;
	mbstowcs_s(&converted, wchar, i_name, strlen(i_name) + 1);
	handle_ = CreateMutex(NULL, (BOOL)i_bTakeOwnership, wchar);
	ASSERT(handle_ != NULL);
}

inline Mutex::~Mutex()
{
	BOOL result = CloseHandle(handle_);
	ASSERT(result == TRUE);
}

FORCEINLINE bool Mutex::TryLock()
{
	DWORD result = WaitForSingleObject(handle_, 0);
	return result == WAIT_OBJECT_0;
}

FORCEINLINE void Mutex::Lock()
{
	DWORD result = WaitForSingleObject(handle_, INFINITE);
	ASSERT(result == WAIT_OBJECT_0);
}

FORCEINLINE bool Mutex::Lock(DWORD i_WaitMilliseconds)
{
	DWORD result = WaitForSingleObject(handle_, i_WaitMilliseconds);
	ASSERT(((i_WaitMilliseconds == INFINITE) && (result == WAIT_OBJECT_0)) || (result == WAIT_TIMEOUT));
	return result == WAIT_OBJECT_0;
}

void Mutex::Unlock(void)
{
	BOOL result = ReleaseMutex(handle_);
	ASSERT(result == TRUE);
}