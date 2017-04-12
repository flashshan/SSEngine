#include "Core\Memory\FixedSizeAllocator.h"

FixedSizeAllocator *FixedSizeAllocator::Create(void *i_pMemory, size_t i_sizeMemory, size_t i_numBlocks, size_t i_blockSize)
{
	ASSERT(i_pMemory);
	void *pBitArrayBase = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(i_pMemory) + sizeof(FixedSizeAllocator));
	size_t fixedAllocatorSize = i_sizeMemory - sizeof(FixedSizeAllocator);
	FixedSizeAllocator *pFixedSizeAllocator = new (i_pMemory) FixedSizeAllocator(pBitArrayBase, fixedAllocatorSize, i_numBlocks, i_blockSize);
	return pFixedSizeAllocator;
}

FixedSizeAllocator::FixedSizeAllocator(void *i_pTotalMemory, size_t i_sizeMemory, size_t i_numBlocks, size_t i_blockSize)
	: numBlocks_(i_numBlocks), blockSize_(i_blockSize)
{
	size_t bitArrayTotalSize = sizeof(BitArray) + ((i_numBlocks - 1) / (sizeof(uintPtr) * 8) + 1) * sizeof(uintPtr);

	bitArray_ = BitArray::Create(i_numBlocks, i_pTotalMemory, bitArrayTotalSize, true);

	blocksMemoryBase_ = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(i_pTotalMemory) + bitArrayTotalSize);
	
	// keep alignment
	blocksMemoryBase_ = reinterpret_cast<void *>(((reinterpret_cast<uintPtr>(i_pTotalMemory) + bitArrayTotalSize - 1) / i_blockSize + 1) * i_blockSize);
	
	memorySize_ = i_numBlocks * i_blockSize;

	InitializeCriticalSection(&criticalSection_);
}


FixedSizeAllocator::~FixedSizeAllocator()
{
	bitArray_ = nullptr;
}