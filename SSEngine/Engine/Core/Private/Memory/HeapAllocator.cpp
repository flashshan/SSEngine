#include "Core\Memory\HeapAllocator.h"

	
HeapAllocator* HeapAllocator::Create(void *i_pMemory, const size_t i_sizeMemory, const uint32 i_numDescriptors)
{
	ASSERT(i_sizeMemory > sizeof(HeapAllocator) + (sizeof(BlockDescriptor) * i_numDescriptors));

	void * pManagerMemory = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(i_pMemory) + sizeof(HeapAllocator));
	size_t pManagerSize = i_sizeMemory - sizeof(HeapAllocator);

	HeapAllocator * pHeapManager = new (i_pMemory) HeapAllocator(pManagerMemory, pManagerSize, i_numDescriptors);

	return pHeapManager;
}

// create a new HeapManager
HeapAllocator::HeapAllocator(void *i_pMemory, const size_t i_sizeMemory, const uint32 i_numDescriptors)
	:heapMemoryBase_(i_pMemory), memorySize_(i_sizeMemory), blockMaxLength_(i_numDescriptors)
{
	ASSERT(i_pMemory);
	ASSERT(i_sizeMemory);
	ASSERT(i_numDescriptors * sizeof(BlockDescriptor) < i_sizeMemory);

	// initialize free list and used list
	freeList_ = reinterpret_cast<BlockDescriptor *>(reinterpret_cast<uintPtr>(heapMemoryBase_) + memorySize_ - blockMaxLength_ * BLOCK_SIZE);
	freeList_->BlockBase = heapMemoryBase_;
	freeList_->SizeBlock = memorySize_ - blockMaxLength_ * BLOCK_SIZE;
	freeList_->MemoryBase = heapMemoryBase_;
	freeList_->Previous = nullptr;
	freeList_->Next = nullptr;

	usedList_ = nullptr;

	blockLength_ = 1;
}

HeapAllocator::~HeapAllocator()
{
	heapMemoryBase_ = nullptr;
	freeList_ = nullptr;
	usedList_ = nullptr;
}


void* HeapAllocator::alloc(size_t i_size)
{
	BlockDescriptor * temp = freeList_;

	if (temp == nullptr) return nullptr;

	// store addtional length in head and tail according to _DEBUG
#if defined(_DEBUG)
	uint32 headLength = BLOCK_POINTER + GUARD_BANDING_LENGTH;
	uint32 tailLength = GUARD_BANDING_LENGTH;
#else
	uint32 headLength = BLOCK_POINTER;
	uint32 tailLength = 0;
#endif

	void *res = nullptr;

	//search the free list to find useable memory
	while (temp != nullptr)
	{
		if (temp->SizeBlock - i_size >= headLength + tailLength)
		{
			BlockDescriptor *innerTemp;
			if (temp->SizeBlock - i_size <= headLength + tailLength + FIT_LENGTH)
			{

				// set BlockDescriptor's position before GUARD BANDING
				*static_cast<uintPtr *>(temp->BlockBase) = reinterpret_cast<uintPtr>(temp);
				
				// set GUARD BANDING and return position according to _DEBUG
#if defined(_DEBUG)
				*reinterpret_cast<uint32 *>(reinterpret_cast<uintPtr>(temp->BlockBase) + BLOCK_POINTER) = GUARD_BANDING_VALUE;
				res = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(temp->BlockBase) + BLOCK_POINTER + GUARD_BANDING_LENGTH);
				*reinterpret_cast<uint32 *>(reinterpret_cast<uintPtr>(temp->BlockBase) + BLOCK_POINTER + GUARD_BANDING_LENGTH + i_size) = GUARD_BANDING_VALUE;
#else
				res = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(temp->m_pBlockBase) + BLOCK_POINTER);
#endif
				// operate free list node and used list
				if (temp->Next != nullptr)
					temp->Next->Previous = temp->Previous;
				if (temp->Previous != nullptr)
					temp->Previous->Next = temp->Next;
				if (temp == freeList_)
					freeList_ = temp->Next;

				if (usedList_ != nullptr)
					usedList_->Previous = temp;
				temp->Next = usedList_;
				temp->Previous = nullptr;
				usedList_ = temp;
			}
			else
			{
				innerTemp = usedList_;

				// split block in free list and assign new used block 
				ASSERT(blockLength_ < blockMaxLength_);

				usedList_ = reinterpret_cast<BlockDescriptor *>(reinterpret_cast<uintPtr>(heapMemoryBase_) + memorySize_ - (blockMaxLength_ - blockLength_) * BLOCK_SIZE);
				usedList_->MemoryBase = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(temp->MemoryBase) + temp->SizeBlock - tailLength - i_size - headLength);
				usedList_->BlockBase = usedList_->MemoryBase;
				usedList_->SizeBlock = i_size + headLength + tailLength;

				// set BlockDescriptor's position before GUARD BANDING
				*static_cast<uintPtr *>(usedList_->BlockBase) = (uintPtr)(usedList_);
				
				// set GUARD BANDING and return position according to _DEBUG
#if defined(_DEBUG)
				*reinterpret_cast<uint32 *>(reinterpret_cast<uintPtr>(usedList_->BlockBase) + BLOCK_POINTER) = GUARD_BANDING_VALUE;
				res = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(usedList_->BlockBase) + BLOCK_POINTER + GUARD_BANDING_LENGTH);
				*reinterpret_cast<uint32 *>(reinterpret_cast<uintPtr>(usedList_->BlockBase) + BLOCK_POINTER + GUARD_BANDING_LENGTH + i_size) = GUARD_BANDING_VALUE;
#else
				res = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(usedList_->m_pBlockBase) + BLOCK_POINTER);
#endif
				// operate free list node and used list
				usedList_->Previous = nullptr;
				usedList_->Next = innerTemp;
				if (innerTemp != nullptr)
					innerTemp->Previous = usedList_;

				temp->SizeBlock -= i_size + headLength + tailLength;
				blockLength_++;
			}
			return res;
		}
		else
			temp = temp->Next;
	}
	return nullptr;
}


void *	HeapAllocator::alloc(size_t i_size, uint32 i_alignment)
{
	BlockDescriptor * temp = freeList_;

	if (temp == nullptr) return nullptr;

	// store addtional length in head and tail according to _DEBUG
#if defined(_DEBUG)
	uint32 headLength = BLOCK_POINTER + GUARD_BANDING_LENGTH;
	uint32 tailLength = GUARD_BANDING_LENGTH;
#else
	uint32 headLength = BLOCK_POINTER;
	uint32 tailLength = 0;
#endif

	void *res = nullptr;
	while (temp != nullptr)
	{
		// find alignment position
		uintPtr tempAlignment = alignment(reinterpret_cast<uintPtr>(temp->MemoryBase) + temp->SizeBlock - tailLength - i_size, i_alignment);
		if (tempAlignment >= reinterpret_cast<uintPtr>(temp->MemoryBase) + headLength)
		{
			BlockDescriptor *innerTemp;

			// use alignment position to judge whether blocks' size is enough
			if (tempAlignment <= reinterpret_cast<uintPtr>(temp->MemoryBase) + FIT_LENGTH + headLength)
			{
				// change m_pBlockBase into the start of useful memory
				temp->BlockBase = reinterpret_cast<void*>(tempAlignment - headLength);

				// set BlockDescriptor's position before GUARD BANDING
				*static_cast<uintPtr *>(temp->BlockBase) = reinterpret_cast<uintPtr>(temp);

				// set GUARD BANDING and return position according to _DEBUG
#if defined(_DEBUG)
				*reinterpret_cast<uint32 *>(reinterpret_cast<uintPtr>(temp->BlockBase) + BLOCK_POINTER) = GUARD_BANDING_VALUE;
				res = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(temp->BlockBase) + BLOCK_POINTER + GUARD_BANDING_LENGTH);
				*reinterpret_cast<uint32 *>(reinterpret_cast<uintPtr>(temp->BlockBase) + BLOCK_POINTER + GUARD_BANDING_LENGTH + i_size) = GUARD_BANDING_VALUE;
#else
				res = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(temp->m_pBlockBase) + BLOCK_POINTER);
#endif

				// operate free list node and used list
				if (temp->Next != nullptr)
					temp->Next->Previous = temp->Previous;
				if (temp->Previous != nullptr)
					temp->Previous->Next = temp->Next;
				if(temp == freeList_)
					freeList_ = temp->Next;

				if (usedList_ != nullptr)
					usedList_->Previous = temp;
				temp->Next = usedList_;
				temp->Previous = nullptr;
				usedList_ = temp;
			}
			else
			{
				innerTemp = usedList_;

				// split block in free list and assign new used block 
				ASSERT(blockLength_ < blockMaxLength_);

				usedList_ = reinterpret_cast<BlockDescriptor *>(reinterpret_cast<uintPtr>(heapMemoryBase_) + memorySize_ - (blockMaxLength_ - blockLength_) * BLOCK_SIZE);
				usedList_->MemoryBase = reinterpret_cast<void *>(tempAlignment - headLength);
				usedList_->BlockBase = usedList_->MemoryBase;
				usedList_->SizeBlock = reinterpret_cast<uintPtr>(temp->MemoryBase) + temp->SizeBlock - tempAlignment + headLength;

				// set BlockDescriptor's position before GUARD BANDING
				*static_cast<uintPtr *>(usedList_->BlockBase) = (uintPtr)usedList_;

				// set GUARD BANDING and return position according to _DEBUG
#if defined(_DEBUG)
				*reinterpret_cast<uint32 *>(reinterpret_cast<uintPtr>(usedList_->BlockBase) + BLOCK_POINTER) = GUARD_BANDING_VALUE;
				res = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(usedList_->BlockBase) + BLOCK_POINTER + GUARD_BANDING_LENGTH);
				*reinterpret_cast<uint32 *>(reinterpret_cast<uintPtr>(usedList_->BlockBase) + BLOCK_POINTER + GUARD_BANDING_LENGTH + i_size) = GUARD_BANDING_VALUE;
#else
				res = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(usedList_->m_pBlockBase) + BLOCK_POINTER);
#endif

				// operate free list node and used list
				usedList_->Previous = nullptr;
				usedList_->Next = innerTemp;
				if (innerTemp != nullptr)
					innerTemp->Previous = usedList_;

				temp->SizeBlock = reinterpret_cast<uintPtr>(usedList_->MemoryBase) - reinterpret_cast<uintPtr>(temp->MemoryBase);
				blockLength_++;
			}
			return res;
		}
		else
			temp = temp->Next;
	}
	return nullptr;
}

bool	HeapAllocator::free(const void * i_ptr)
{
#ifdef _DEBUG
	uint32 forward = BLOCK_POINTER + GUARD_BANDING_LENGTH;
#else
	uint32 forward = BLOCK_POINTER;
#endif // _DEBUG

	BlockDescriptor * temp = reinterpret_cast<BlockDescriptor *>(*reinterpret_cast<uintPtr *>(reinterpret_cast<uintPtr>(i_ptr) - forward));
	ASSERT(temp != nullptr);

	//check GUARD BANDING (now can only support the previous check)
	uint32 firstGuardBand = *reinterpret_cast<uint32 *>(reinterpret_cast<uintPtr>(i_ptr) - GUARD_BANDING_LENGTH);
	//uint32 secondGUARDBAND = *(uint32 *)(uint32)
	ASSERT(firstGuardBand == GUARD_BANDING_VALUE);

	// modify free list and used list
	if (temp->Previous == nullptr && temp->Next == nullptr)
		usedList_ = nullptr;
	if(temp->Previous != nullptr)
		temp->Previous->Next = temp->Next;
	if(temp->Next != nullptr)
		temp->Next->Previous = temp->Previous;
	usedList_ = temp->Next;

	if (freeList_ != nullptr)
		freeList_->Previous = temp;
	temp->Next = freeList_;

	// reset m_pBlockBase to m_pMemoryBase
	temp->BlockBase = temp->MemoryBase;
	temp->Next = freeList_;
	temp->Previous = nullptr;
	freeList_ = temp;
	return true;
}

void	HeapAllocator::collect()
{
	// quick sort free list
	freeList_ = qsort(freeList_);

	// combine fragment in free list
	freeList_ = combineList(freeList_);

	/*
	Magic code below, hard to understand but it works to reorder all used list and free list
	*/
	// reset free list and used list's BlockDescriptor's memory position
	// handle the situation that freelist or usedlist only has one node
	bool singleFree = false, singleUsed = false;
	void *freeBase = nullptr, *usedBase = nullptr;
	if (freeList_ != nullptr &&freeList_->Next == nullptr)
	{
		singleFree = true;
		freeBase = freeList_;
	}
	if (usedList_ != nullptr && usedList_->Next == nullptr)
	{
		singleUsed = true;
		usedBase = usedList_;
	}
	
	// search reasonable position of BlockDescriptor and reorgnize their position
	BlockDescriptor * temp1 = freeList_;
	BlockDescriptor * temp2 = usedList_;
	BlockDescriptor * temp;

	uintPtr maxAddress = reinterpret_cast<uintPtr>(heapMemoryBase_) + memorySize_ - (blockMaxLength_ - blockLength_) * BLOCK_SIZE;
	bool mark = true;

	for (uint32 i = 0; i < blockLength_;i++)
	{
		uintPtr address = reinterpret_cast<uintPtr>(heapMemoryBase_) + memorySize_ - (blockMaxLength_ - i) * BLOCK_SIZE;
		temp = reinterpret_cast<BlockDescriptor *>(address);
		if (temp->Previous == nullptr && temp->Next == nullptr)
		{
			while (temp1 != nullptr || temp2 != nullptr)
			{
				if (mark)
				{
					while (reinterpret_cast<uintPtr>(temp1) < maxAddress)
					{
						if (temp1 == nullptr)
						{
							mark = false;
							break;
						}
						temp1 = temp1->Next;	
					}
				}
				if (!mark)
				{
					while (temp2 != nullptr && reinterpret_cast<uintPtr>(temp2) < maxAddress)
					{
						temp2 = temp2->Next;
					}
				}

				if (mark)
				{
					*temp = *temp1;
					if (!singleFree || (singleFree && freeBase != temp1))
					{
						if (temp->Previous != nullptr)
							temp->Previous->Next = temp;
						if (temp->Next != nullptr)
							temp->Next->Previous = temp;
						if (temp1 == freeList_) 
							freeList_ = temp;
#ifdef _DEBUG
						uint32 forward = BLOCK_POINTER + GUARD_BANDING_LENGTH;
#else
						uint32 forward = BLOCK_POINTER;
#endif // _DEBUG
						*static_cast<uintPtr *>(temp->BlockBase) = reinterpret_cast<uintPtr>(temp);

						temp1->Previous = nullptr;
						temp1->Next = nullptr;
					}
					temp1 = temp->Next;
					break;
				}
				else if(temp2 != nullptr)
				{
					*temp = *temp2;
					if (!singleUsed || (singleUsed && usedBase != temp2))
					{
						if(temp->Previous != nullptr)
							temp->Previous->Next = temp;
						if(temp->Next != nullptr)
							temp->Next->Previous = temp;
						if (temp2 == usedList_) 
							usedList_ = temp;
#ifdef _DEBUG
						uint32 forward = BLOCK_POINTER + GUARD_BANDING_LENGTH;
#else
						uint32 forward = BLOCK_POINTER;
#endif // _DEBUG
						*static_cast<uintPtr *>(temp->BlockBase) = reinterpret_cast<uintPtr>(temp);

						temp2->Previous = nullptr;
						temp2->Next = nullptr;
					}
					temp2 = temp->Next;
					break;
				}
			}
		}
	}
}

size_t	HeapAllocator::getLargestFreeBlock() const
{
	size_t res = 0;
	BlockDescriptor *temp = freeList_;
	while (temp != nullptr)
	{
		if (temp->SizeBlock > res)
			res = temp->SizeBlock;
		temp = temp->Next;
	}
	return res;
}

size_t  HeapAllocator::getLargestUsedBlock() const
{
	size_t res = 0;
	BlockDescriptor *temp = usedList_;
	while (temp != nullptr)
	{
		if (temp->SizeBlock > res)
			res = temp->SizeBlock;
		temp = temp->Next;
	}
	return res;
}

size_t	HeapAllocator::getTotalFreeMemory() const
{
	size_t res = 0;
	BlockDescriptor *temp = freeList_;
	while (temp != nullptr)
	{
		res += temp->SizeBlock;
		temp = temp->Next;
	}
	return res;
}

size_t	HeapAllocator::getTotalUsedMemory() const
{
	size_t res = 0;
	BlockDescriptor *temp = usedList_;
	while (temp != nullptr)
	{
		res += temp->SizeBlock;
		temp = temp->Next;
	}
	return res;
}

uint32 HeapAllocator::getFreeCount() const
{
	uint32 res = 0;
	BlockDescriptor *temp = freeList_;
	while (temp != nullptr)
	{
		res++;
		temp = temp->Next;
	}
	return res;
}

uint32 HeapAllocator::getUsedCount() const
{
	uint32 res = 0;
	BlockDescriptor *temp = usedList_;
	while (temp != nullptr)
	{
		res++;
		temp = temp->Next;
	}
	return res;
}


BlockDescriptor* HeapAllocator::qsort(BlockDescriptor *i_head)
{
	if (i_head == nullptr || i_head->Next == nullptr) return i_head;
	BlockDescriptor *h1 = nullptr, *h2 = nullptr;
	BlockDescriptor *temp1 = h1, *temp2 = h2, *temp = i_head->Next;
	i_head->Previous = nullptr;
	i_head->Next = nullptr;
	
	while (temp != nullptr)
	{
		if (temp->MemoryBase < i_head->MemoryBase)
		{
			if (temp1 == nullptr)
			{
				h1 = temp;
				temp1 = temp;
				h1->Previous = nullptr;
			}
			else
			{
				temp->Previous = temp1;
				temp1->Next = temp;
				temp1 = temp;
			}
		}
		else
		{
			if (temp2 == nullptr)
			{
				h2 = temp;
				temp2 = temp;
				h2->Previous = nullptr;
			}
			else
			{
				temp->Previous = temp2;
				temp2->Next = temp;
				temp2 = temp;
			}
		}
		temp = temp->Next;
	}
	if(temp1 != nullptr)
		temp1->Next = nullptr;
	if(temp2 != nullptr)
		temp2->Next = nullptr;
	return append(append(qsort(h1), i_head), qsort(h2));
}

BlockDescriptor * HeapAllocator::append(BlockDescriptor *i_h1, BlockDescriptor *i_h2)
{
	if (i_h1 == nullptr) return i_h2;
	if (i_h2 == nullptr) return i_h1;
	BlockDescriptor * temp = i_h1;
	while (temp->Next != nullptr)
	{
		temp = temp->Next;
	}
	temp->Next = i_h2;
	i_h2->Previous = temp;
	return i_h1;
}

BlockDescriptor* HeapAllocator::combineList(BlockDescriptor *i_list)
{
	if (i_list == nullptr) return nullptr;
	BlockDescriptor *temp = i_list;
	while (temp->Next != nullptr)
	{
		if (reinterpret_cast<uintPtr>(temp->MemoryBase) + temp->SizeBlock == reinterpret_cast<uintPtr>(temp->Next->MemoryBase))
		{
			temp->SizeBlock += temp->Next->SizeBlock;
			if (temp->Next->Next != nullptr)
			{
				temp->Next = temp->Next->Next;
				temp->Next->Previous->Previous = nullptr;
				temp->Next->Previous->Next = nullptr;
				temp->Next->Previous = temp;
			}
			else
			{
				temp->Next->Previous = nullptr;
				temp->Next->Next = nullptr;
				temp->Next = nullptr;
			}
			blockLength_--;
		}
		else
		{
			temp = temp->Next;
		}
	}
	return i_list;
}
