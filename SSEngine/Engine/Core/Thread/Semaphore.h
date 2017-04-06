#pragma once

#include <Windows.h>
#include "Core\CoreMinimal.h"

class Semaphore
{
public:
	FORCEINLINE Semaphore( uint32 i_initCount, uint32 i_maxCount, const char * i_name = nullptr );
	inline ~Semaphore();

	FORCEINLINE void Increment( uint32 i_Count = 1 );
	FORCEINLINE void Decrement();
	FORCEINLINE bool Decrement( DWORD i_WaitMilliseconds );

	FORCEINLINE HANDLE GetHandle() const { return handle_; }

private:
	FORCEINLINE Semaphore(const Semaphore &i_other) {}
	FORCEINLINE Semaphore& operator=(const Semaphore &i_other) {}

private:
	HANDLE handle_;
};








// implement forceline

inline Semaphore::Semaphore(uint32 i_initCount, uint32 i_maxCount, const char * i_name)
{
	ASSERT(i_initCount <= i_maxCount);

	WCHAR wchar[MAX_NAME_LENGTH];
	size_t converted = 0;
	mbstowcs_s(&converted, wchar, i_name, strlen(i_name) + 1);
	handle_ = CreateSemaphore(NULL, i_initCount, i_maxCount, wchar);
	ASSERT(handle_ != NULL);
}

FORCEINLINE Semaphore::~Semaphore()
{
	BOOL result = CloseHandle(handle_);
	ASSERT(result == TRUE);
}

// should 
FORCEINLINE void Semaphore::Increment(uint32 i_count)
{
	BOOL result = ReleaseSemaphore(handle_, i_count, NULL);
	ASSERT(result == TRUE);
}

FORCEINLINE void Semaphore::Decrement(void)
{
	DWORD result = WaitForSingleObject(handle_, INFINITE);
	ASSERT(result == WAIT_OBJECT_0);
}

FORCEINLINE bool Semaphore::Decrement(DWORD i_WaitMilliseconds)
{
	DWORD result = WaitForSingleObject(handle_, i_WaitMilliseconds);
	ASSERT(((i_WaitMilliseconds == INFINITE) && (result == WAIT_OBJECT_0)) || (result == WAIT_TIMEOUT));

	return result == WAIT_OBJECT_0;
}
