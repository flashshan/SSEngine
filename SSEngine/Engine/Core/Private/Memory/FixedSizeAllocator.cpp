#include "Core\Memory\FixedSizeAllocator.h"

FixedSizeAllocator *FixedSizeAllocator::Create(void *i_pMemory, const size_t i_sizeMemory, const size_t i_numBlocks, const size_t i_blockSize)
{
	ASSERT(i_pMemory);
	void *pBitArrayBase = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(i_pMemory) + sizeof(FixedSizeAllocator));
	size_t fixedAllocatorSize = i_sizeMemory - sizeof(FixedSizeAllocator);

	FixedSizeAllocator *pFixedSizeAllocator = new (i_pMemory) FixedSizeAllocator(pBitArrayBase, fixedAllocatorSize, i_numBlocks, i_blockSize);
	return pFixedSizeAllocator;
}

FixedSizeAllocator::FixedSizeAllocator(void *i_pTotalMemory, const size_t i_sizeMemory, const size_t i_numBlocks, const size_t i_blockSize)
	: numBlocks_(i_numBlocks), blockSize_(i_blockSize)
{
	size_t bitArrayTotalSize = sizeof(BitArray) + ((i_numBlocks - 1) / (sizeof(uintPtr) * 8) + 1) * sizeof(uintPtr);

	bitArray_ = BitArray::Create(i_numBlocks - 1, i_pTotalMemory, bitArrayTotalSize, true);

	blocksMemoryBase_ = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(i_pTotalMemory) + bitArrayTotalSize);
	memorySize_ = i_sizeMemory - bitArrayTotalSize;
}