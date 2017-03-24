#pragma once

#include "Core\CoreMinimal.h"

class BitArray {
public:
	static BitArray * Create(size_t i_numBits, void * i_pAllocator, size_t i_memorySize, bool i_startClear = true);
	FORCEINLINE void destroy();

	FORCEINLINE void ClearAll();
	FORCEINLINE void SetAll();

	FORCEINLINE bool AreAllClear() const;
	FORCEINLINE bool AreAllSet() const;

	FORCEINLINE bool IsBitSet(const size_t i_bitNumber) const { return !(*this)[i_bitNumber]; }
	FORCEINLINE bool IsBitClear(const size_t i_bitNumber) const { return (*this)[i_bitNumber]; }

	FORCEINLINE void SetBit(const size_t i_bitNumber);
	FORCEINLINE void ClearBit(const size_t i_bitNumber);

	bool GetFirstClearBit(size_t & o_bitNumber) const;
	bool GetFirstSetBit(size_t & o_bitNumber) const;

	FORCEINLINE bool operator[](const size_t i_index) const;

private:
	BitArray(size_t i_numBits, void * i_memoryBase = nullptr, bool i_startClear = true);
	
	//hide Copy constructor
	BitArray(const BitArray &i_other) {};

private:
	size_t numBits_;
	size_t roundUpBits_;
	uintPtr *arrayBase_;
	size_t arrayBits_;
};






// implement forceinline

FORCEINLINE void BitArray::destroy()
{
	if (arrayBase_ != nullptr)
		delete arrayBase_;
	arrayBase_ = nullptr;
	delete this;
}

FORCEINLINE void BitArray::ClearAll()
{
	memset(arrayBase_, 0xffffffff, roundUpBits_);
}

FORCEINLINE void BitArray::SetAll()
{
	memset(arrayBase_, 0x00000000, roundUpBits_);
}

FORCEINLINE bool BitArray::AreAllClear() const
{
	size_t noUseTemp;
	return GetFirstSetBit(noUseTemp);
}
FORCEINLINE bool BitArray::AreAllSet() const
{
	size_t noUseTemp;
	return !GetFirstClearBit(noUseTemp);
}

FORCEINLINE void BitArray::SetBit(const size_t i_bitNumber)
{
	ASSERT(i_bitNumber <= numBits_);
	arrayBase_[i_bitNumber / arrayBits_] &= (UINTPTR_MAX - (static_cast<uintPtr>(1) << i_bitNumber % arrayBits_));
}
FORCEINLINE void BitArray::ClearBit(const size_t i_bitNumber)
{

	ASSERT(i_bitNumber <= numBits_);
	arrayBase_[i_bitNumber / arrayBits_] |= (static_cast<uintPtr>(1) << i_bitNumber % arrayBits_);
}

FORCEINLINE bool BitArray::operator[](const size_t i_index) const
{
	ASSERT(i_index <= numBits_);
	return (arrayBase_[i_index / arrayBits_] >> (i_index % arrayBits_)) & 1;
}