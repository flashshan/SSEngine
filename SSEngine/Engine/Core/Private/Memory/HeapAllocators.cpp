#include "Core\Memory\HeapAllocators.h"
#include "Core\Memory\HeapManager.h"

#include <assert.h>
#include <malloc.h>		// for _aligned_malloc
#include <inttypes.h>	// for print specifiers
#include <stdlib.h>		// for nullptr
#include <stdint.h>

#define DEFAULT_HEAP_ALIGNMENT 4

HeapManager * getDefaultHeap()
{
	static HeapManager * defaultHeap;

	if (defaultHeap == nullptr)
	{
		static const size_t defaultHeapSize = 1024 * 1024;
		
		void * defaultHeapMemory = _aligned_malloc(defaultHeapSize, DEFAULT_HEAP_ALIGNMENT);
		assert(defaultHeapMemory);

		static const unsigned int numDescriptors = 2048;

		defaultHeap = HeapManager::CreateHeapManager(defaultHeapMemory, defaultHeapSize, numDescriptors);
		if (defaultHeap == nullptr)
			_aligned_free(defaultHeapMemory);
	}

	assert(defaultHeap);
	return defaultHeap;
}

// standard new & delete
void * operator new(size_t i_size)
{
	DEBUG_PRINT("Calling new( size_t ) with ( %Iu ).\n", i_size);

	return _aligned_malloc(i_size, DEFAULT_HEAP_ALIGNMENT);
}

void operator delete(void * i_ptr)
{
	DEBUG_PRINT("Calling delete( void * ) with ( %" PRIxPTR " ).\n", i_ptr);

	if (i_ptr)
		_aligned_free(i_ptr);
}

// array new[] and delete[]
void * operator new[](size_t i_size)
{
	DEBUG_PRINT("Calling new[]( size_t ) with ( %Iu ).\n", i_size);
	void * ptr = _aligned_malloc(i_size, DEFAULT_HEAP_ALIGNMENT);

	DEBUG_PRINT("new[]( size_t ) returning %" PRIxPTR ".\n", ptr);
	return ptr;
}

void operator delete[](void * i_ptr)
{
	DEBUG_PRINT("Calling delete[]( void * ) on %" PRIxPTR ".\n", i_ptr);

	if (i_ptr)
		_aligned_free(i_ptr);
}

void * operator new(size_t i_size, NewAlignment i_align)
{
	DEBUG_PRINT("Calling new( size_t, NewAlignment ) with ( %Iu, %d ).\n", i_size, i_align);

	switch (i_align)
	{
	case NewAlignment::EAlign16:
	case NewAlignment::EAlign32:
		return _aligned_malloc(i_size, static_cast<uint32>(i_align));
	case NewAlignment::EAlignDefault:
	default:
		return _aligned_malloc(i_size, DEFAULT_HEAP_ALIGNMENT);
	}
}

// needed to keep compiler happy if above operator throws which is important if exception handling is enabled
void operator delete(void * i_ptr, NewAlignment i_align)
{
	DEBUG_PRINT("Calling delete( void *, NewAlignment ) with ( %" PRIxPTR ", %d ).\n", i_ptr, i_align);
	DEBUG_PRINT("By the way, this should never happen.\n");

	// don't attempt to delete nullptr pointers, can be changed to assert
	if (i_ptr)
		_aligned_free(i_ptr);
}


// tracking allocators
void * operator new(size_t i_size, const char * i_file, unsigned int i_line)
{
	DEBUG_PRINT("Calling new( size_t ) with ( %Iu ) from %s:%d.\n", i_size, i_file, i_line);

	void *pointer = _aligned_malloc(i_size, DEFAULT_HEAP_ALIGNMENT);
	// StoreTrackingInfo( pMem, i_pFile, i_Line);
	return pointer;
}

void operator delete(void * i_ptr, const char * i_pFile, unsigned int i_Line)
{
	DEBUG_PRINT("Calling delete( void *, unsigned int, const char * ) with ( %" PRIxPTR ", %s, %d ).\n", i_ptr, i_pFile, i_Line);
	DEBUG_PRINT("By the way, this should never happen.\n");

	if (i_ptr)
	{
		// ReleaseTrackingInfo( i_ptr );
		_aligned_free(i_ptr);
	}
}

void * operator new(size_t i_size, HeapManager * i_heap)
{
	DEBUG_PRINT("Calling new( size_t, HeapManager * ) with ( %Iu, %" PRIxPTR ").\n", i_size, i_heap);

	if (i_heap == nullptr)
		i_heap = getDefaultHeap();

	assert(i_heap);

	return i_heap->alloc(i_size, DEFAULT_HEAP_ALIGNMENT);
}

void operator delete(void * i_ptr, HeapManager * i_heap, unsigned int i_align)
{
	DEBUG_PRINT("Calling delete( void *, HeapManager *, unsigned int  ) with ( %" PRIxPTR ", %" PRIxPTR ", %d ).\n", i_ptr, i_heap, i_align);
	DEBUG_PRINT("BTW... this should never happen.\n");

	if (i_ptr)
	{
		if (i_heap == nullptr)
			i_heap = getDefaultHeap();

		assert(i_heap);

		i_heap->free(i_ptr);
	}
}

void * operator new(size_t i_size, HeapManager * i_heap, unsigned int i_align)
{
	DEBUG_PRINT("Calling new( size_t, HeapManager *, unsigned int ) with ( %Iu, %" PRIxPTR ", %d).\n", i_size, i_heap, i_align);

	if (i_heap == nullptr)
		i_heap = getDefaultHeap();

	assert(i_heap);

	return i_heap->alloc(i_size, i_align);

}

void operator delete(void * i_ptr, HeapManager * i_heap)
{
	DEBUG_PRINT("Calling delete( void *, HeapManager * ) with ( %" PRIxPTR ", %" PRIxPTR " ).\n", i_ptr, i_heap);

	if (i_ptr)
	{
		if (i_heap == nullptr)
			i_heap = getDefaultHeap();

		assert(i_heap);

		i_heap->free(i_ptr);
	}
}
