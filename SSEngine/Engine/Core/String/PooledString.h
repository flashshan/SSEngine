#pragma once

#include "Core\Basic\BasicTypes.h"
#include "Core\Basic\Assert.h"

class PooledString {
public:
	inline PooledString(const char *i_string);
	inline PooledString(const uint32 i_length, const char *i_string);

	FORCEINLINE uint32 GetLength() const { return length_; }
	FORCEINLINE const char* GetString() const { return string_; }

	bool Equal(const char *i_string) const;
	bool Equal(const char *i_string, uint32 i_length) const;

private:
	PooledString(PooledString &i_other) {}

private:
	uint32 length_;
	char *string_;
};





inline PooledString::PooledString(const char *i_string)
	: length_(strlen(i_string)), string_(reinterpret_cast<char *>(this) + sizeof(PooledString))
{
	memcpy(string_, i_string, length_ + 1);
}

inline PooledString::PooledString(const uint32 i_length, const char *i_string)
	: length_(i_length), string_(reinterpret_cast<char *>(this) + sizeof(PooledString))
{
	memcpy(string_, i_string, i_length + 1);
}

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