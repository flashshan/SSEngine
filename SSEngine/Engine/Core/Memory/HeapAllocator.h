#pragma once

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

#define DEFAULT_ALIGNMENT 4
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
	static HeapAllocator *Create(void *i_pMemory, const size_t i_sizeMemory, const uint32 i_numDescriptors);
	
	inline ~HeapAllocator();

	FORCEINLINE void destroy();

	// allocate - with and without alignment requirement
	void *alloc(const size_t i_size);
	void *alloc(const size_t i_size, const uint32 i_alignment);

	// free
	bool free(const void *i_ptr);

	// run garbage collection
	void collect();

	// query whether a given pounsigned_inter is within this Heaps memory range
	FORCEINLINE bool Contains(const void * i_ptr) const;
	// query whether a given pounsigned_inter is an outstanding allocation
	FORCEINLINE bool IsAllocated(const void * i_ptr) const;

	size_t getLargestFreeBlock() const;
	size_t getTotalFreeMemory() const;

	size_t getLargestUsedBlock() const;
	size_t getTotalUsedMemory() const;

	uint32 getFreeCount() const;
	uint32 getUsedCount() const;

	FORCEINLINE void ShowFreeBlocks() const;
	FORCEINLINE void ShowUsedBlocks() const;

private:
	// create a new HeapManager
	HeapAllocator(void *i_pMemory, const size_t i_sizeMemory, const uint32 i_numDescriptors);
	
	// hide copy constructor
	FORCEINLINE HeapAllocator(const HeapAllocator &i_other) {}

	// return previous alignment position 
	inline uintPtr alignment(const uintPtr i_pos, const uint32 i_align) const
	{
		return i_pos - i_pos % i_align;
	}

	// double list quick sort
	BlockDescriptor *qsort(BlockDescriptor *i_head);

	// double list append
	BlockDescriptor *append(BlockDescriptor *i_h1, BlockDescriptor *i_h2);

	// free list combination
	BlockDescriptor *combineList(BlockDescriptor *i_list);

private:
	void *heapMemoryBase_;
	size_t memorySize_;
	unsigned int blockLength_;
	unsigned int blockMaxLength_;

	BlockDescriptor* freeList_;
	BlockDescriptor *usedList_;
};







// implement forceinline

inline HeapAllocator::~HeapAllocator()
{
	destroy();
}

FORCEINLINE void HeapAllocator::destroy()
{
	heapMemoryBase_ = nullptr;
	freeList_ = nullptr;
	usedList_ = nullptr;
}

FORCEINLINE bool HeapAllocator::Contains(const void * i_ptr) const
{
	if (heapMemoryBase_ <= i_ptr && reinterpret_cast<uintPtr>(heapMemoryBase_) + memorySize_ - blockMaxLength_ * BLOCK_SIZE > reinterpret_cast<uintPtr>(i_ptr))
		return true;
	else 
		return false;
}

FORCEINLINE bool HeapAllocator::IsAllocated(const void * i_ptr) const
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

FORCEINLINE void HeapAllocator::ShowFreeBlocks() const
{
	printf_s("Total Free block number is %d \n", getFreeCount());
	printf_s("Total Free block size is %zu \n", getTotalFreeMemory());
	printf_s("The largest block size is %zu \n", getLargestFreeBlock());
	printf_s("\n");
}

FORCEINLINE void HeapAllocator::ShowUsedBlocks() const
{
	printf_s("Total Used block number is %d \n", getUsedCount());
	printf_s("Total Used block size is %zu \n", getTotalUsedMemory());
	printf_s("The largest block size is %zu \n", getLargestUsedBlock());
	printf_s("\n");
}
