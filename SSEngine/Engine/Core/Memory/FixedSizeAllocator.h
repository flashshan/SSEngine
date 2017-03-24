#pragma once 

#include "Core\CoreMinimal.h"
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
	static FixedSizeAllocator *Create(void *i_pMemory, const size_t i_sizeMemory, const size_t i_numBlocks, const size_t i_blockSize);
	// destroy the HeapManager

	FORCEINLINE void destroy();

	// allocate - with and without alignment requirement
	FORCEINLINE void *alloc();

	// free
	FORCEINLINE bool free(const void *i_ptr);

	// query whether a given pounsigned_inter is within this Heaps memory range
	FORCEINLINE bool Contains(const void * i_ptr) const;

	// query whether a given pounsigned_inter is an outstanding allocation
	FORCEINLINE bool IsAllocated(const void * i_ptr) const;

	FORCEINLINE uint32 getFreeCount() const;
	FORCEINLINE uint32 getUsedCount() const;

private:
	// create a new HeapManager
	FixedSizeAllocator(void *i_pTotalMemory, const size_t i_sizeMemory, const size_t i_numBlocks, const size_t i_blockSize);
	
	// hide copy constructor
	FixedSizeAllocator(FixedSizeAllocator &i_other) {}

private:
	void *blocksMemoryBase_;
	size_t memorySize_;
	size_t numBlocks_;
	size_t blockSize_;
	BitArray *bitArray_;
};









// implement inline

// allocate - with and without alignment requirement
FORCEINLINE void *FixedSizeAllocator::alloc()
{
	size_t i_firstAvailable;

	if (bitArray_->GetFirstClearBit(i_firstAvailable))
	{
		// mark it in use because we're going to allocate it to user
		bitArray_->SetBit(i_firstAvailable);

		// calculate it’s address and return it to user
		return reinterpret_cast<void *>(reinterpret_cast<uintPtr>(blocksMemoryBase_) + (i_firstAvailable * blockSize_));
	}
	else
		return nullptr;
}

FORCEINLINE void FixedSizeAllocator::destroy()
{
	if (bitArray_ != nullptr)
		delete bitArray_;
}

// free
FORCEINLINE bool FixedSizeAllocator::free(const void *i_ptr)
{
	ASSERT(i_ptr);
	bitArray_->ClearBit((reinterpret_cast<uintPtr>(i_ptr) - reinterpret_cast<uintPtr>(blocksMemoryBase_)) / blockSize_);
	return true;
}

// query whether a given pounsigned_inter is within this Heaps memory range
FORCEINLINE bool FixedSizeAllocator::Contains(const void * i_ptr) const
{
	ASSERT(i_ptr);
	return i_ptr >= blocksMemoryBase_ && i_ptr < reinterpret_cast<void *>(reinterpret_cast<uintPtr>(blocksMemoryBase_) + blockSize_ * numBlocks_);
	//return i_ptr >= m_blocksMemoryBase && i_ptr <= reinterpret_cast<void *>(reinterpret_cast<uintPtr>(m_blocksMemoryBase) + m_blockSize * m_numBlocks);
}

// query whether a given pounsigned_inter is an outstanding allocation
FORCEINLINE bool FixedSizeAllocator::IsAllocated(const void * i_ptr) const
{
	ASSERT(i_ptr);
	return bitArray_->IsBitSet((reinterpret_cast<uintPtr>(i_ptr) - reinterpret_cast<uintPtr>(blocksMemoryBase_)) / blockSize_);
}

FORCEINLINE uint32 FixedSizeAllocator::getFreeCount() const
{
	// TO DO

	return 0;
}


FORCEINLINE uint32 FixedSizeAllocator::getUsedCount() const
{
	// TO DO

	return 0;
}





