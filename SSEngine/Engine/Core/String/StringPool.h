#pragma once

#include "Core\Basic\BasicTypes.h"
#include "Core\Basic\Assert.h"

class StringPool {
public:
	static inline StringPool * Create(void *i_memoryBase, size_t i_bytes);
	inline ~StringPool();

	const char *add(const char *i_string);
	const char *find(const char *i_string);

private:
	FORCEINLINE StringPool(void *i_poolBuffer, size_t i_poolSize);
	FORCEINLINE StringPool(StringPool &i_other) {}

private:
	uint8 *poolBuffer_;
	size_t poolSize_;
	uint8 *endPosition_;
};





inline StringPool *StringPool::Create(void *i_memoryBase, size_t i_bytes)
{
	ASSERT(i_memoryBase);
	ASSERT(i_bytes > 0);

	void * stringPoolMemoryBase = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(i_memoryBase) + sizeof(StringPool));
	size_t stringPoolSize = i_bytes - sizeof(StringPool);

	StringPool * stringPool = new (i_memoryBase) StringPool(stringPoolMemoryBase, stringPoolSize);

	return stringPool;
}

inline StringPool::~StringPool()
{
}

FORCEINLINE StringPool::StringPool(void *i_poolBuffer, size_t i_poolSize)
	: poolBuffer_(static_cast<uint8 *>(i_poolBuffer)), poolSize_(i_poolSize), endPosition_(static_cast<uint8 *>(i_poolBuffer))
{
}

