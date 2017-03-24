#include "Core\String\HashedString.h"


uint32 HashedString::Hash(const void * i_bytes, uint32 i_bytecount)
{
	// FNV hash, http://isthe.com/chongo/tech/comp/fnv/

	const uint8 *p = static_cast<const uint8 *>(i_bytes);
	uint32 hash = 2166136261;

	for (uint32 i = 0; i < i_bytecount; ++i)
		hash = 16777619 * (hash ^ p[i]);

	return hash ^ (hash >> 16);
}
