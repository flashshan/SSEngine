#include "Core\Memory\HeapManager.h"

#include "Core\Memory\HeapAllocator.h"
#include "Core\Memory\FixedSizeAllocator.h"

HeapManager* HeapManager::globalInstance_ = nullptr;

HeapManager::~HeapManager()
{
	fixedSizeAllocator8_->~FixedSizeAllocator();
	fixedSizeAllocator16_->~FixedSizeAllocator();
	fixedSizeAllocator32_->~FixedSizeAllocator();
	fixedSizeAllocator64_->~FixedSizeAllocator();
	fixedSizeAllocator128_->~FixedSizeAllocator();

	heapAllocator_->~HeapAllocator();
}


HeapManager::HeapManager(void *i_heapBase, size_t i_heapSize)
{
	const uint32 numDescriptors = 4096;

	const size_t defaultAlignment = 4;

	// Create a heap manager for my test heap.
	heapAllocator_ = HeapAllocator::Create(i_heapBase, i_heapSize, numDescriptors);
	ASSERT(heapAllocator_ != nullptr);

	// create fixedSizeAllocator
	const size_t fixedBlockNumber8 = 80000;
	const size_t fixedBlockNumber16 = 40000;
	const size_t fixedBlockNumber32 = 20000;
	const size_t fixedBlockNumber64 = 10000;
	const size_t fixedBlockNumber128 = 5000;

	const size_t additionalSize = 11000;

	const size_t fixedMemorySize8 = fixedBlockNumber8 * 8 + additionalSize;
	size_t fixedMemorySize16 = fixedBlockNumber16 * 16 + additionalSize;
	size_t fixedMemorySize32 = fixedBlockNumber32 * 32 + additionalSize;
	size_t fixedMemorySize64 = fixedBlockNumber64 * 64 + additionalSize;
	size_t fixedMemorySize128 = fixedBlockNumber128 * 128 + additionalSize;

	void *fixedMemoryBase8 = heapAllocator_->alloc(fixedMemorySize8, 8);
	void *fixedMemoryBase16 = heapAllocator_->alloc(fixedMemorySize16, 16);
	void *fixedMemoryBase32 = heapAllocator_->alloc(fixedMemorySize32, 32);
	void *fixedMemoryBase64 = heapAllocator_->alloc(fixedMemorySize64, 64);
	void *fixedMemoryBase128 = heapAllocator_->alloc(fixedMemorySize128, 128);

	fixedSizeAllocator8_ = FixedSizeAllocator::Create(fixedMemoryBase8, fixedMemorySize8, fixedBlockNumber8, 8);
	fixedSizeAllocator16_ = FixedSizeAllocator::Create(fixedMemoryBase16, fixedMemorySize16, fixedBlockNumber16, 16);
	fixedSizeAllocator32_ = FixedSizeAllocator::Create(fixedMemoryBase32, fixedMemorySize32, fixedBlockNumber32, 32);
	fixedSizeAllocator64_ = FixedSizeAllocator::Create(fixedMemoryBase64, fixedMemorySize64, fixedBlockNumber64, 64);
	fixedSizeAllocator128_ = FixedSizeAllocator::Create(fixedMemoryBase128, fixedMemorySize128, fixedBlockNumber128, 128);
}


void *HeapManager::Alloc(size_t i_size, uint32 i_alignment)
{
	void *ptr = nullptr;
	if (i_size <= 8)
	{
		ptr = fixedSizeAllocator8_->alloc();
	}
	else if (i_size <= 16)
	{
		ptr = fixedSizeAllocator16_->alloc();
	}
	else if (i_size <= 32)
	{
		ptr = fixedSizeAllocator32_->alloc();
	}
	else if (i_size <= 64)
	{
		ptr = fixedSizeAllocator64_->alloc();
	}
	else if (i_size <= 128)
	{
		ptr = fixedSizeAllocator128_->alloc();
	}

	if (ptr == nullptr || i_size > 128)
	{
		ptr = heapAllocator_->alloc(i_size, i_alignment);
	}
	if (ptr == nullptr)
	{
		heapAllocator_->collect();

		ptr = heapAllocator_->alloc(i_size, i_alignment);

		SLOW_ASSERT(ptr, ErrorType::EHeapOverflow);
	}
	return ptr;
}


bool HeapManager::Free(void *i_ptr)
{
	if (fixedSizeAllocator8_->Contains(i_ptr))
	{
		ASSERT(fixedSizeAllocator8_->IsAllocated(i_ptr));
		return fixedSizeAllocator8_->free(i_ptr);
	}
	else if (fixedSizeAllocator16_->Contains(i_ptr))
	{
		ASSERT(fixedSizeAllocator16_->IsAllocated(i_ptr));
		return fixedSizeAllocator16_->free(i_ptr);
	}
	else if (fixedSizeAllocator32_->Contains(i_ptr))
	{
		ASSERT(fixedSizeAllocator32_->IsAllocated(i_ptr));
		return fixedSizeAllocator32_->free(i_ptr);
	}
	else if (fixedSizeAllocator64_->Contains(i_ptr))
	{
		ASSERT(fixedSizeAllocator64_->IsAllocated(i_ptr));
		return fixedSizeAllocator64_->free(i_ptr);
	}
	else if (fixedSizeAllocator128_->Contains(i_ptr))
	{
		ASSERT(fixedSizeAllocator128_->IsAllocated(i_ptr));
		return fixedSizeAllocator128_->free(i_ptr);
	}
	else
	{
		ASSERT(heapAllocator_->IsAllocated(i_ptr));
		return heapAllocator_->free(i_ptr);
	}
}