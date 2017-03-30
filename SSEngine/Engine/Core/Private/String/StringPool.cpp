#include "Core\String\StringPool.h"

#include "Core\String\PooledString.h"


StringPool *StringPool::globalInstance_ = nullptr;

char *StringPool::add(const char *i_string)
{
	ASSERT(i_string != nullptr);

	char *res = const_cast<char *>(find(i_string));
	if (res != nullptr)
	{
		return res;
	}

	uint32 length = strlen(i_string);
	uint8 * newPosition = reinterpret_cast<uint8 *>((reinterpret_cast<uintPtr>(endPosition_ + sizeof(PooledString) + length) / sizeof(uint32) + 1) * sizeof(uint32));
	ASSERT(newPosition < poolBuffer_ + poolSize_);

	PooledString *pooledString = new (endPosition_) PooledString(length, i_string);
	
	res = const_cast<char *>(pooledString->GetString());
	endPosition_ = newPosition;
	
	return res;
}

char *StringPool::find(const char *i_string)
{
	ASSERT(i_string != nullptr);

	uint32 length = strlen(i_string);

	char *resPosition = nullptr;

	uint8 *tempPosition = poolBuffer_;
	while (tempPosition < endPosition_)
	{
		PooledString *pooledString = reinterpret_cast<PooledString *>(tempPosition);
		if (pooledString->Equal(i_string, length))
		{
			resPosition = const_cast<char *>(pooledString->GetString());
			break;
		}
		tempPosition = reinterpret_cast<uint8 *>((reinterpret_cast<uintPtr>(tempPosition + sizeof(PooledString) + length) / sizeof(uint32) + 1) * sizeof(uint32));
	}
	return resPosition;
}


	

