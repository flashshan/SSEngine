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
	// Hide copy constructor
	FORCEINLINE PooledString(const PooledString &i_other) {}

private:
	uint32 length_;
	char *string_;
};





inline PooledString::PooledString(const char *i_string)
	: length_(static_cast<uint32>(strlen(i_string))), string_(reinterpret_cast<char *>(this) + sizeof(PooledString))
{
	memcpy(string_, i_string, length_ + 1);
}

inline PooledString::PooledString(const uint32 i_length, const char *i_string)
	: length_(i_length), string_(reinterpret_cast<char *>(this) + sizeof(PooledString))
{
	memcpy(string_, i_string, i_length + 1);
}