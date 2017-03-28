#pragma once

#include "Core\Basic\BasicTypes.h"
#include "Core\Basic\Assert.h"
#include "PooledString.h"

#ifdef _DEBUG
#define KEEP_STRING
#endif // _DEBUG


class HashedString {
public:
	FORCEINLINE HashedString();
	explicit FORCEINLINE HashedString(const char *i_string);
	FORCEINLINE HashedString(const PooledString &i_pooledString);
	FORCEINLINE HashedString(const HashedString &i_other);
	inline ~HashedString();

	FORCEINLINE HashedString& operator =(const HashedString &i_other);

	FORCEINLINE uint32 GetHash() const { return hash_; }

	FORCEINLINE bool operator ==(const HashedString &i_other) const;

private:
	static FORCEINLINE uint32 Hash(const char * i_string);
	static uint32 Hash(const void * i_bytes, uint32 i_bytecount);

private:
	uint32 hash_;

#ifdef KEEP_STRING
	const char *string_;
#endif // KEEP_STRING
};




FORCEINLINE HashedString::HashedString()
	: hash_(Hash(""))
{
}

FORCEINLINE HashedString::HashedString(const char *i_string)
	: hash_(Hash(i_string))
{
#ifdef KEEP_STRING
	string_ = i_string;
#endif // KEEP_STRING
}

FORCEINLINE HashedString::HashedString(const PooledString &i_pooledString)
	: hash_( Hash(i_pooledString.GetString(), i_pooledString.GetLength()) )
{
#ifdef KEEP_STRING
	string_ = i_pooledString.GetString();
#endif // KEEP_STRING
}


FORCEINLINE HashedString::HashedString(const HashedString &i_other)
	: hash_(i_other.hash_)
{
#ifdef KEEP_STRING
	string_ = i_other.string_;
#endif // KEEP_STRING
}

inline HashedString::~HashedString()
{
}

FORCEINLINE HashedString & HashedString::operator =(const HashedString &i_other)
{
	hash_ = i_other.hash_;
#ifdef KEEP_STRING
	string_ = i_other.string_;
#endif // KEEP_STRING
	return *this;
}

bool HashedString::operator ==(const HashedString &i_other) const
{
	return hash_ == i_other.hash_;
}

uint32 HashedString::Hash(const char * i_string)
{
	ASSERT(i_string);

	return Hash(reinterpret_cast<void *>(const_cast<char *>(i_string)), static_cast<uint32>(strlen(i_string)));
}

