#pragma once
class HeapManager;

// custom alignment allocators
enum NewAlignment
{
	NEW_ALIGN_DEFAULT,
	NEW_ALIGN_16 = 16,
	NEW_ALIGN_32 = 32
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
void * operator new(size_t i_size, const char * i_pFile, unsigned int i_Line);
void operator delete(void * i_ptr, const char * i_pFile, unsigned int i_Line);

// heap allocators
// i_pHeap = nullptr means grab the default HeapManager
void * operator new(size_t i_size, HeapManager * i_pHeap);
void operator delete(void * i_ptr, HeapManager * i_pHeap);

void * operator new(size_t i_size, HeapManager * i_pHeap, unsigned int i_align);
void operator delete(void * i_ptr, HeapManager * i_pHeap, unsigned int i_align);
