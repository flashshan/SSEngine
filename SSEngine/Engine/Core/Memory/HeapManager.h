#pragma once

#include "Core\CoreMinimal.h"

class HeapAllocator;
class FixedSizeAllocator;

// singleton class
class HeapManager {
public:
	static inline HeapManager *CreateInstance(void *i_heapManagerBase, size_t i_heapSize);
	static FORCEINLINE HeapManager *GetInstance();
	static FORCEINLINE void DestroyInstance();

	~HeapManager();

	void *Alloc(size_t i_size, uint32 i_alignment = DEFAULT_ALIGNMENT);
	bool Free(void *i_ptr);

	HeapAllocator *GetDefaultHeap() const { return heapAllocator_; }

private:
	HeapManager(void *i_heapBase, size_t i_heapSize);
	FORCEINLINE HeapManager(const HeapManager &i_other) {}
	FORCEINLINE HeapManager& operator=(const HeapManager &i_other) {}


private:
	static HeapManager *globalInstance_;

	HeapAllocator *heapAllocator_;
	FixedSizeAllocator *fixedSizeAllocator8_;
	FixedSizeAllocator *fixedSizeAllocator16_;
	FixedSizeAllocator *fixedSizeAllocator32_;
	FixedSizeAllocator *fixedSizeAllocator64_;
	FixedSizeAllocator *fixedSizeAllocator128_;
};


inline HeapManager *HeapManager::CreateInstance(void *i_heapManagerBase, size_t i_heapSize)
{
	ASSERT(HeapManager::globalInstance_ == nullptr);
	void *heapBase = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(i_heapManagerBase) + sizeof(HeapManager));
	HeapManager::globalInstance_ = new (i_heapManagerBase) HeapManager(heapBase, i_heapSize - sizeof(HeapManager));
	return HeapManager::globalInstance_;
}

FORCEINLINE HeapManager *HeapManager::GetInstance()
{
	ASSERT(HeapManager::globalInstance_ != nullptr);
	return HeapManager::globalInstance_;
}

FORCEINLINE void HeapManager::DestroyInstance()
{
	ASSERT(HeapManager::globalInstance_ != nullptr);
	// delete was overload and can not be used here
	HeapManager::globalInstance_->~HeapManager();
	HeapManager::globalInstance_ = nullptr;
}



