#include "Core\Memory\BitArray.h"

#include "Core\Basic\ErrorMessage.h"

// Branch Free Scan 0 from forward
#ifdef _WIN64
bool BitScanForwardZero64(uint32 *o_index, uintPtr i_value)
{
	uint32 i32 = ((i_value & 0xffffffff) == 0xffffffff) << 5;
	i_value >>= i32;

	uint32 i16 = ((i_value & 0xffff) == 0xffff) << 4;
	i_value >>= i16;

	uint32 i8 = ((i_value & 0xff) == 0xff) << 3;
	i_value >>= i8;

	uint32 i4 = ((i_value & 0xf) == 0xf) << 2;
	i_value >>= i4;

	uint32 i2 = ((i_value & 0x3) == 0x3) << 1;
	i_value >>= i2;

	uint32 i1 = ((i_value & 0x1) == 0x1);

	if (((i_value >> i1) & 1) == 1) return false;
	
	*o_index = i32 + i16 + i8 + i4 + i2 + i1;
	return true;
}

bool BitScanForwardOne64(uint32 *o_index, uintPtr i_value)
{
	uint32 i32 = ((i_value | 0xffffffff00000000) == 0xffffffff00000000) << 5;
	i_value >>= i32;

	uint32 i16 = ((i_value | 0xffff0000) == 0xffff0000) << 4;
	i_value >>= i16;

	uint32 i8 = ((i_value | 0xff00) == 0xff00) << 3;
	i_value >>= i8;

	uint32 i4 = ((i_value | 0xf0) == 0xf0) << 2;
	i_value >>= i4;

	uint32 i2 = ((i_value | 0xc) == 0xc) << 1;
	i_value >>= i2;

	uint32 i1 = ((i_value | 0x2) == 0x2);

	if (((i_value >> i1) | 0) == 0) return false;
	*o_index = i32 + i16 + i8 + i4 + i2 + i1;
	return true;
}
#else
bool BitScanForwardZero32(uint32 *o_index, uintPtr i_value)
{
	uint32 i16 = ((i_value & 0xffff) == 0xffff) << 4;
	i_value >>= i16;

	uint32 i8 = ((i_value & 0xff) == 0xff) << 3;
	i_value >>= i8;

	uint32 i4 = ((i_value & 0xf) == 0xf) << 2;
	i_value >>= i4;

	uint32 i2 = ((i_value & 0x3) == 0x3) << 1;
	i_value >>= i2;

	uint32 i1 = ((i_value & 0x1) == 0x1);

	if(((i_value >> i1) & 1) == 1) return false;
	*o_index = i16 + i8 + i4 + i2 + i1;
	return true;
}

bool BitScanForwardOne32(uint32 *o_index, uintPtr i_value)
{
	uint32 i16 = ((i_value | 0xffff0000) == 0xffff0000) << 4;
	i_value >>= i16;

	uint32 i8 = ((i_value | 0xff00) == 0xff00) << 3;
	i_value >>= i8;

	uint32 i4 = ((i_value | 0xf0) == 0xf0) << 2;
	i_value >>= i4;

	uint32 i2 = ((i_value | 0xc) == 0xc) << 1;
	i_value >>= i2;

	uint32 i1 = ((i_value | 0x2) == 0x2);

	if (((i_value >> i1) | 0) == 0) return false;
	*o_index = i16 + i8 + i4 + i2 + i1;
	return true;
}

#endif // _WIN64




BitArray * BitArray::Create(size_t i_numBits, void * i_pAllocator, size_t i_memorySize, bool i_startClear)
{
	SLOW_ASSERT(i_pAllocator, ErrorMessage::kNullPointer);
	SLOW_ASSERT(i_numBits > 0, ErrorMessage::kNonPostiveValue);

	void * pBitArrayMemoryBase = reinterpret_cast<void *>(reinterpret_cast<uintPtr>(i_pAllocator) + sizeof(BitArray));
	size_t bitArraySize = i_memorySize - sizeof(BitArray);

	BitArray * pBitArray = new (i_pAllocator) BitArray(i_numBits, pBitArrayMemoryBase, i_startClear);

	return pBitArray;
}

BitArray::BitArray(size_t i_numBits, void * i_memoryBase, bool i_startClear) : numBits_(i_numBits)
{
	arrayBits_ = sizeof(uintPtr) * 8;
	roundUpBits_ = (i_numBits / arrayBits_ + 1) * arrayBits_;

	arrayBase_ = new (i_memoryBase) uintPtr[roundUpBits_ / arrayBits_];
	ASSERT(arrayBase_);

	if(i_startClear)
		memset(arrayBase_, 0xffffffff, roundUpBits_);
	else
		memset(arrayBase_, 0x00000000, roundUpBits_);
}



bool BitArray::GetFirstClearBit(size_t & o_bitNumber) const
{
	size_t arrayIndex = 0;
	const size_t arrayLength = roundUpBits_ / arrayBits_;
	while ((arrayBase_[arrayIndex] == 0))
	{
		arrayIndex++;
		if (arrayIndex == arrayLength)
			return false;
	}
	uint32 index;
#ifdef _WIN64
	BitScanForwardZero64(&index, m_arrayBase[arrayIndex]);
#else
	BitScanForwardZero32(&index, arrayBase_[arrayIndex]);
#endif // _WIN64
	if (arrayIndex * arrayBits_ + index > numBits_)
		return false;

	o_bitNumber = arrayIndex * arrayBits_ + index;
	return true;
}

bool BitArray::GetFirstSetBit(size_t & o_bitNumber) const
{
	size_t arrayIndex = 0;
	const size_t arrayLength = roundUpBits_ / arrayBits_;
	while ((arrayBase_[arrayIndex] == UINTPTR_MAX))
	{
		arrayIndex++;
		if (arrayIndex == arrayLength)
			return false;
	}
	uint32 index;
#ifdef _WIN64
	BitScanForwardZero64(&index, m_arrayBase[arrayIndex]);
#else
	BitScanForwardZero32(&index, arrayBase_[arrayIndex]);
#endif // _WIN64
	if (arrayIndex * arrayBits_ + index > numBits_)
		return false;
	o_bitNumber = arrayIndex * arrayBits_ + index;
	return true;
}

