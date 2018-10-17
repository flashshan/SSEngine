#include "Core\Memory\New.h"

#include <inttypes.h>	// for print specifiers

#include "Core\Memory\HeapManager.h"
#include "Core\Memory\HeapAllocator.h"

// standard new & delete
void * operator new(size_t i_size)
{
	DEBUG_PRINT("Calling new( size_t ) with ( %Iu ).\n", i_size);
	return HeapManager::GetInstance()->Alloc(i_size, DEFAULT_ALIGNMENT);
}

void operator delete(void * i_ptr)
{
	DEBUG_PRINT("Calling delete( void * ) with ( %" PRIxPTR " ).\n", i_ptr);
	if (i_ptr)
	{
		HeapManager::GetInstance()->Free(i_ptr);
	}
}

// array new[] and delete[]
void * operator new[](size_t i_size)
{
	// TO DO.  should have size

	DEBUG_PRINT("Calling new[]( size_t ) with ( %Iu ).\n", i_size);
	void * ptr = HeapManager::GetInstance()->Alloc(i_size, DEFAULT_ALIGNMENT);

	DEBUG_PRINT("new[]( size_t ) returning %" PRIxPTR ".\n", ptr);
	return ptr;
}

void operator delete[](void * i_ptr)
{
	DEBUG_PRINT("Calling delete[]( void * ) on %" PRIxPTR ".\n", i_ptr);

	if (i_ptr)
	{
		HeapManager::GetInstance()->Free(i_ptr);
	}
}

void * operator new(size_t i_size, NewAlignment i_align)
{
	DEBUG_PRINT("Calling new( size_t, NewAlignment ) with ( %Iu, %d ).\n", i_size, i_align);

	switch (i_align)
	{
	case NewAlignment::EAlign16:
	case NewAlignment::EAlign32:
	case NewAlignment::EAlign64:
		return HeapManager::GetInstance()->Alloc(i_size, static_cast<uint32>(i_align));
	case NewAlignment::EAlignDefault:
	default:
		return HeapManager::GetInstance()->Alloc(i_size, DEFAULT_ALIGNMENT);
	}
}

// needed to keep compiler happy if above operator throws which is important if exception handling is enabled
void operator delete(void * i_ptr, NewAlignment i_align)
{
	DEBUG_PRINT("Calling delete( void *, NewAlignment ) with ( %" PRIxPTR ", %d ).\n", i_ptr, i_align);
	DEBUG_PRINT("By the way, this should never happen.\n");

	// don't attempt to delete nullptr pointers, can be changed to assert
	if (i_ptr)
	{
		HeapManager::GetInstance()->Free(i_ptr);
	}
}


// tracking allocators
void * operator new(size_t i_size, const char * i_file, uint32 i_line)
{
	DEBUG_PRINT("Calling new( size_t ) with ( %Iu ) from %s:%d.\n", i_size, i_file, i_line);

	return HeapManager::GetInstance()->Alloc(i_size, DEFAULT_ALIGNMENT);
}

void operator delete(void * i_ptr, const char * i_pFile, uint32 i_Line)
{
	DEBUG_PRINT("Calling delete( void *, unsigned int, const char * ) with ( %" PRIxPTR ", %s, %d ).\n", i_ptr, i_pFile, i_Line);
	DEBUG_PRINT("By the way, this should never happen.\n");

	if (i_ptr)
	{
		HeapManager::GetInstance()->Free(i_ptr);
	}
}

// alocate from heap
void * operator new(size_t i_size, HeapAllocator * i_heap)
{
	DEBUG_PRINT("Calling new( size_t, HeapManager * ) with ( %Iu, %" PRIxPTR ").\n", i_size, i_heap);

	if (i_heap == nullptr)
	{
		i_heap = HeapManager::GetInstance()->GetDefaultHeap();
	}
	ASSERT(i_heap);

	return i_heap->alloc(i_size, DEFAULT_ALIGNMENT);
}

void operator delete(void * i_ptr, HeapAllocator * i_heap)
{
	DEBUG_PRINT("Calling delete( void *, HeapManager * ) with ( %" PRIxPTR ", %" PRIxPTR " ).\n", i_ptr, i_heap);

	if (i_ptr)
	{
		if (i_heap == nullptr)
		{
			i_heap = HeapManager::GetInstance()->GetDefaultHeap();
		}

		ASSERT(i_heap);

		i_heap->free(i_ptr);
	}
}

// alocator from heap with alignment
void * operator new(size_t i_size, HeapAllocator * i_heap, NewAlignment i_align)
{
	DEBUG_PRINT("Calling new( size_t, HeapManager *, unsigned int ) with ( %Iu, %" PRIxPTR ", %d).\n", i_size, i_heap, i_align);

	if (i_heap == nullptr)
	{
		i_heap = HeapManager::GetInstance()->GetDefaultHeap();
	}

	ASSERT(i_heap);

	switch(i_align)
	{
	case NewAlignment::EAlign16:
	case NewAlignment::EAlign32:
	case NewAlignment::EAlign64:
		return i_heap->alloc(i_size, static_cast<uint32>(i_align));
	case NewAlignment::EAlignDefault:
	default:
		return i_heap->alloc(i_size, DEFAULT_ALIGNMENT);
	}
}


void operator delete(void * i_ptr, HeapAllocator * i_heap, NewAlignment i_align)
{
	DEBUG_PRINT("Calling delete( void *, HeapManager *, unsigned int  ) with ( %" PRIxPTR ", %" PRIxPTR ", %d ).\n", i_ptr, i_heap, i_align);
	DEBUG_PRINT("BTW... this should never happen.\n");

	if (i_ptr)
	{
		if (i_heap == nullptr)
		{
			i_heap = HeapManager::GetInstance()->GetDefaultHeap();
		}
		ASSERT(i_heap);

		i_heap->free(i_ptr);
	}
}





//void * operator new(size_t i_size, void * i_ptr)
//{
//	DEBUG_PRINT("Calling new( size_t, void * ) with ( %zx, %" PRIxPTR " ).\n", i_size, i_ptr);
//	return i_ptr;
//}
