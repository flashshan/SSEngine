#include "Core\String\PooledString.h"


bool PooledString::Equal(const char *i_string) const
{
	if (strlen(i_string) != length_)
	{
		return false;
	}

	for (uint32 i = 0;i < length_;++i)
	{
		if (i_string[i] != string_[i])
		{
			return false;
		}
	}
	return true;
}

bool PooledString::Equal(const char *i_string, uint32 i_length) const
{
	if (i_length != length_)
	{
		return false;
	}

	for (uint32 i = 0;i < i_length; ++i)
	{
		if (i_string[i] != string_[i])
		{
			return false;
		}
	}
	return true;
}