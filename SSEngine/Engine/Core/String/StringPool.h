#pragma once

#include "Core\CoreMinimal.h"


class StringPool {
public:
	static inline StringPool * CreateInstance(void *i_memoryBase, size_t i_bytes);
	static FORCEINLINE StringPool * GetInstance();
	static FORCEINLINE void DestroyInstance();

	inline ~StringPool();

	char *add(const char *i_string);
	char *find(const char *i_string);

private:
	FORCEINLINE StringPool(void *i_poolBuffer, size_t i_poolSize);
	FORCEINLINE StringPool(const StringPool &i_other) {}
	FORCEINLINE StringPool& operator=(const StringPool &i_other) {}

	static StringPool *globalInstance_;
private:
	uint8 *poolBuffer_;
	size_t poolSize_;
	uint8 *endPosition_;
};





inline StringPool *StringPool::CreateInstance(void *i_memoryBase, size_t i_bytes)
{
	ASSERT(StringPool::globalInstance_ == nullptr);
	ASSERT(i_memoryBase);
	ASSERT(i_bytes > 0);

	void * stringPoolMemoryBase = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(i_memoryBase) + sizeof(StringPool));
	size_t stringPoolSize = i_bytes - sizeof(StringPool);

	StringPool::globalInstance_ = new (i_memoryBase) StringPool(stringPoolMemoryBase, stringPoolSize);

	return StringPool::globalInstance_;
}

FORCEINLINE StringPool *StringPool::GetInstance()
{
	ASSERT(StringPool::globalInstance_ != nullptr);
	return StringPool::globalInstance_;
}

FORCEINLINE void StringPool::DestroyInstance()
{
	ASSERT(StringPool::globalInstance_ != nullptr);
	delete StringPool::globalInstance_;
	StringPool::globalInstance_ = nullptr;
}

inline StringPool::~StringPool()
{
}

FORCEINLINE StringPool::StringPool(void *i_poolBuffer, size_t i_poolSize)
	: poolBuffer_(static_cast<uint8 *>(i_poolBuffer)), poolSize_(i_poolSize), endPosition_(static_cast<uint8 *>(i_poolBuffer))
{
}

