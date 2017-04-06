#pragma once

#include <Windows.h>

class SingleWriterMutipleReader
{
public:
	FORCEINLINE SingleWriterMutipleReader();

	FORCEINLINE void ReadLock();
	FORCEINLINE bool TryReadLock();

	FORCEINLINE void ReleaseReadLock();
	FORCEINLINE void WriteLock();

	FORCEINLINE bool TryWriteLock();
	FORCEINLINE void ReleaseWriteLock();

private:
	SRWLOCK rwLock_;
};





// implement forceinline

FORCEINLINE SingleWriterMutipleReader::SingleWriterMutipleReader()
{
	InitializeSRWLock(&rwLock_);
}

FORCEINLINE void SingleWriterMutipleReader::ReadLock()
{
	AcquireSRWLockShared(&rwLock_);
}

FORCEINLINE bool SingleWriterMutipleReader::TryReadLock()
{
	return TryAcquireSRWLockShared(&rwLock_) == TRUE;
}

FORCEINLINE void SingleWriterMutipleReader::ReleaseReadLock()
{
	ReleaseSRWLockShared(&rwLock_);
}

FORCEINLINE void SingleWriterMutipleReader::WriteLock()
{
	AcquireSRWLockExclusive(&rwLock_);
}
FORCEINLINE bool SingleWriterMutipleReader::TryWriteLock()
{
	return TryAcquireSRWLockExclusive(&rwLock_) == TRUE;
}

FORCEINLINE void SingleWriterMutipleReader::ReleaseWriteLock()
{
	ReleaseSRWLockShared(&rwLock_);
}