#pragma once 

#include <Windows.h>

#include "BitArray.h"

#if defined(_WIN64)
#define BLOCK_POINTER 8					  // this is the length of BlockDescriptor's pointer
#else
#define BLOCK_POINTER 4					  // this is the length of BlockDescriptor's pointer
#endif

#define DEFAULT_ALIGNMENT 4
#define GUARD_BANDING_Length 4			  // default Guard banding's length
#define GUARD_BANDING_VALUE 0xdead	  // default Guard banding's value

class BitArray;
class FixedSizeAllocator
{
public:
	static FixedSizeAllocator *Create(void *i_pMemory, size_t i_sizeMemory, size_t i_numBlocks, size_t i_blockSize);
	
	~FixedSizeAllocator();

	// allocate - with and without alignment requirement
	inline void *alloc();

	// free
	FORCEINLINE bool free(const void *i_ptr);

	// query whether a given pounsigned_inter is within this Heaps memory range
	FORCEINLINE bool Contains(const void * i_ptr) const;

	// query whether a given pounsigned_inter is an outstanding allocation 
	FORCEINLINE bool IsAllocated(const void * i_ptr) const;

	FORCEINLINE size_t getFreeCount() const;
	FORCEINLINE size_t getUsedCount() const;

private:
	// create a new HeapManager
	FixedSizeAllocator(void *i_pTotalMemory, size_t i_sizeMemory, size_t i_numBlocks, size_t i_blockSize);
	
	// hide copy constructor
	FORCEINLINE FixedSizeAllocator(const FixedSizeAllocator &i_other) {}
	FORCEINLINE FixedSizeAllocator& operator=(const FixedSizeAllocator &i_other) {}


private:
	void *blocksMemoryBase_;
	size_t memorySize_;
	size_t numBlocks_;
	size_t blockSize_;
	BitArray *bitArray_;

	CRITICAL_SECTION criticalSection_;
};









// implement inline

// allocate - with and without alignment requirement
inline void *FixedSizeAllocator::alloc()
{
	size_t i_firstAvailable;

	EnterCriticalSection(&criticalSection_);
	if (bitArray_->GetFirstClearBit(i_firstAvailable))
	{
		// mark it in use because we're going to allocate it to user
		bitArray_->SetBit(i_firstAvailable);

		LeaveCriticalSection(&criticalSection_);
		// calculate it’s address and return it to user
		return reinterpret_cast<void *>(reinterpret_cast<uintPtr>(blocksMemoryBase_) + ((i_firstAvailable - 1) * blockSize_));
	}
	else
	{
		LeaveCriticalSection(&criticalSection_);
		return nullptr;
	}
}

// free
FORCEINLINE bool FixedSizeAllocator::free(const void *i_ptr)
{
	ASSERT(i_ptr);
	EnterCriticalSection(&criticalSection_);
	bitArray_->ClearBit((reinterpret_cast<uintPtr>(i_ptr) - reinterpret_cast<uintPtr>(blocksMemoryBase_)) / blockSize_ + 1);
	LeaveCriticalSection(&criticalSection_);
	return true;
}

// query whether a given pounsigned_inter is within this Heaps memory range
FORCEINLINE bool FixedSizeAllocator::Contains(const void * i_ptr) const
{
	ASSERT(i_ptr);
	return i_ptr >= blocksMemoryBase_ && i_ptr < reinterpret_cast<void *>(reinterpret_cast<uintPtr>(blocksMemoryBase_) + memorySize_);
}

// query whether a given pounsigned_inter is an outstanding allocation
FORCEINLINE bool FixedSizeAllocator::IsAllocated(const void * i_ptr) const
{
	ASSERT(i_ptr);
	return bitArray_->IsBitSet((reinterpret_cast<uintPtr>(i_ptr) - reinterpret_cast<uintPtr>(blocksMemoryBase_)) / blockSize_ + 1);
}

FORCEINLINE size_t FixedSizeAllocator::getFreeCount() const
{
	return bitArray_->GetClearCount();
}


FORCEINLINE size_t FixedSizeAllocator::getUsedCount() const
{
	return bitArray_->GetSetCount();
}