#pragma once

#include "Core\CoreMinimal.h"
class HeapManager;

// custom alignment allocators
enum class NewAlignment : uint32
{
	EAlignDefault,
	EAlign16 = 16,
	EAlign32 = 32
};

HeapManager * getDefaultHeap();

// standard new / delete
void * operator new(size_t i_size);
void operator delete(void * i_ptr);

// standard array new / delete
void * operator new[](size_t i_size);
void operator delete[](void * i_ptr);


#ifdef _DEBUG
#define TRACK_NEW (__FILE__,__LINE__)
#else
#define TRACK_NEW
#endif

void * operator new(size_t i_size, NewAlignment i_align);
void operator delete(void * i_ptr, NewAlignment i_align);

// tracking allocators
// used by TRACK_NEW
void * operator new(size_t i_size, const char * i_file, unsigned int i_line);
void operator delete(void * i_ptr, const char * i_file, unsigned int i_line);

// heap allocators
// i_pHeap = nullptr means grab the default HeapManager
void * operator new(size_t i_size, HeapManager * i_heap);
void operator delete(void * i_ptr, HeapManager * i_heap);

void * operator new(size_t i_size, HeapManager * i_heap, unsigned int i_align);
void operator delete(void * i_ptr, HeapManager * i_heap, unsigned int i_align);
