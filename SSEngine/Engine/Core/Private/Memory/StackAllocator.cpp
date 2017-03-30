#include "Core\Memory\StackAllocator.h"

// TO DO

StackAllocator::~StackAllocator()
{
	destroy();
}


StackAllocator *StackAllocator::CreateStackAllocator(void *i_pMemory, const size_t i_sizeMemory)
{
	return nullptr;
}

StackAllocator::StackAllocator(void *i_pMemory, const size_t i_sizeMemory)
{
}

void StackAllocator::destroy()
{
}

void StackAllocator::freeToMarker()
{
}

void StackAllocator::alloc(const size_t i_size)
{
}

void StackAllocator::clear()
{
}



