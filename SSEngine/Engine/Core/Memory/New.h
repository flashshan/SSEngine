#pragma once

#include "Core\CoreMinimal.h"
class HeapAllocator;

#ifdef _DEBUG
#define TRACK_NEW (__FILE__,__LINE__)
#else
#define TRACK_NEW
#endif


enum class NewAlignment : uint32
{
	EAlignDefault,
	EAlign16 = 16,
	EAlign32 = 32,
	EAlign64 = 64
};

// standard new / delete
void * operator new(size_t i_size);
void operator delete(void * i_ptr);

// standard array new / delete   
// for test
void * operator new[](size_t i_size);
void operator delete[](void * i_ptr);

// alignment new / delete
void * operator new(size_t i_size, NewAlignment i_align);
void operator delete(void * i_ptr, NewAlignment i_align);

// tracking allocators
// used by TRACK_NEW
void * operator new(size_t i_size, const char * i_file, uint32 i_line);
void operator delete(void * i_ptr, const char * i_file, uint32 i_line);

// heap allocators
// i_pHeap = nullptr means grab the default HeapManager
void * operator new(size_t i_size, HeapAllocator * i_heap);
void operator delete(void * i_ptr, HeapAllocator * i_heap);

// heap alignment new / delete
void * operator new(size_t i_size, HeapAllocator * i_heap, NewAlignment i_align);
void operator delete(void * i_ptr, HeapAllocator * i_heap, NewAlignment i_align);

// placement new
void * operator new(size_t i_size, void * i_ptr);