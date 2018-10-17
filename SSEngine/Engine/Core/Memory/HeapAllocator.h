#pragma once

#include <windows.h>

#include "Core\CoreMinimal.h"

#if defined(_WIN64)
#define BLOCK_SIZE 40
#define BLOCK_POINTER 8					  // this is the length of BlockDescriptor's pointer
#define FIT_LENGTH 48   
#else
#define BLOCK_SIZE 20
#define BLOCK_POINTER 4					  // this is the length of BlockDescriptor's pointer
#define FIT_LENGTH 24   
#endif

#define GUARD_BANDING_LENGTH 4				      // default Guard banding's length
#define GUARD_BANDING_VALUE 0xdead	  // default Guard banding's value

// double list
struct BlockDescriptor
{
	BlockDescriptor *Previous;
	BlockDescriptor *Next;
	void *MemoryBase;   // pointer to actual memory's start position (useful in _alloc_align)
	void *BlockBase;	// pointer to useful memory's start position
	size_t SizeBlock;	// size of that block of memory
};

class HeapAllocator
{
public:
	static HeapAllocator *Create(void *i_pMemory, size_t i_sizeMemory, uint32 i_numDescriptors);
	
	~HeapAllocator();

	// allocate - with and without alignment requirement
	void *alloc(size_t i_size);
	void *alloc(size_t i_size, uint32 i_alignment);

	// free
	bool free(const void *i_ptr);

	// run garbage collection
	void collect();

	// query whether a given pounsigned_inter is within this Heaps memory range
	FORCEINLINE bool Contains(const void * i_ptr) const;
	// query whether a given pounsigned_inter is an outstanding allocation
	inline bool IsAllocated(const void * i_ptr) const;

	size_t GetLargestFreeBlock() const;
	size_t GetTotalFreeMemory() const;

	size_t GetLargestUsedBlock() const;
	size_t GetTotalUsedMemory() const;

	uint32 GetFreeCount() const;
	uint32 GetUsedCount() const;

	inline void ShowFreeBlocks() const;
	inline void ShowUsedBlocks() const;

private:
	// create a new HeapManager
	HeapAllocator(void *i_pMemory, size_t i_sizeMemory, uint32 i_numDescriptors);
	
	// hide copy constructor
	FORCEINLINE HeapAllocator(const HeapAllocator &i_other) {}
	FORCEINLINE HeapAllocator& operator=(const HeapAllocator &i_other) {}

	// return previous alignment position 
	FORCEINLINE uintPtr alignment(uintPtr i_pos, uint32 i_align) const;

	// double list quick sort
	BlockDescriptor *qsort(BlockDescriptor *i_head);

	// double list append
	BlockDescriptor *append(BlockDescriptor *i_h1, BlockDescriptor *i_h2);

	// free list combination
	BlockDescriptor *combineList(BlockDescriptor *i_list);

private:
	void *heapMemoryBase_;
	size_t memorySize_;
	uint32 blockLength_;
	uint32 blockMaxLength_;

	BlockDescriptor* freeList_;
	BlockDescriptor *usedList_;

	CRITICAL_SECTION criticalSection_;
};







// implement forceinline

FORCEINLINE bool HeapAllocator::Contains(const void * i_ptr) const
{
	if (heapMemoryBase_ <= i_ptr && reinterpret_cast<uintPtr>(heapMemoryBase_) + memorySize_ - blockMaxLength_ * BLOCK_SIZE > reinterpret_cast<uintPtr>(i_ptr))
		return true;
	else 
		return false;
}

inline bool HeapAllocator::IsAllocated(const void * i_ptr) const
{
#ifdef _DEBUG
	unsigned int forward = BLOCK_POINTER + GUARD_BANDING_LENGTH;
#else
	unsigned int forward = BLOCK_POINTER;
#endif // _DEBUG

	BlockDescriptor * temp = reinterpret_cast<BlockDescriptor *>(*reinterpret_cast<uintPtr *>(reinterpret_cast<uintPtr>(i_ptr) - forward));
	if (temp == nullptr) return false;

	if (reinterpret_cast<uintPtr>(temp->BlockBase) != reinterpret_cast<uintPtr>(i_ptr) - forward) return false;

	return true;
}

inline void HeapAllocator::ShowFreeBlocks() const
{
	printf_s("Total Free block number is %d \n", GetFreeCount());
	printf_s("Total Free block size is %zu \n", GetTotalFreeMemory());
	printf_s("The largest block size is %zu \n", GetLargestFreeBlock());
	printf_s("\n");
}

inline void HeapAllocator::ShowUsedBlocks() const
{
	printf_s("Total Used block number is %d \n", GetUsedCount());
	printf_s("Total Used block size is %zu \n", GetTotalUsedMemory());
	printf_s("The largest block size is %zu \n", GetLargestUsedBlock());
	printf_s("\n");
}


FORCEINLINE uintPtr HeapAllocator::alignment(uintPtr i_pos, uint32 i_align) const
{
	return i_pos - i_pos % i_align;
}
