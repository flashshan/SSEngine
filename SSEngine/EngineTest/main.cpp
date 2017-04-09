#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#include <crtdbg.h>

#include <assert.h>
#include <algorithm>
#include <vector>
#include <list>

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "Manager\InputManager.h"
#include "Manager\RealTimeManager.h"
#include "Manager\WorldManager.h"
#include "Core\Basic\LuaData.h"
#include "Core\Basic\FileIO.h"
#include "Core\Template\CircleQueue.h"
#include "Core\Template\List.h"
#include "Core\Template\Array.h"
#include "Engine\Engine.h"


#include "GLib\GLib.h"
#include "Lua/lua.hpp"

// for Unit_test

//#define TestHeapManager
//#define TestVector
//#define TestFixedAllocator
//#define TestBitArray
#define TestContainer
//#define TestSmartPointer
//#define TestMatrix
//#define TestLua


#ifdef TestHeapManager
bool HeapManager_UnitTest()
{
	//using namespace HeapManager;
	const size_t 		sizeHeap = 1024 * 1024;
	const unsigned int 	numDescriptors = 2048;

	// Allocate memory for my test heap.
	void * pHeapMemory = _aligned_malloc(sizeHeap, 4);
	assert(pHeapMemory);

	// Create a heap manager for my test heap.
	HeapManager * pHeapManager = HeapManager::CreateHeapManager(pHeapMemory, sizeHeap, numDescriptors);
	assert(pHeapManager);

	if (pHeapManager == nullptr)
		return false;

#ifdef TEST_SINGLE_LARGE_ALLOCATION
	// This is a test I wrote to check to see if using the whole block if it was almost consumed by 
	// an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
	{
		ShowFreeBlocks(pHeapManager);

		size_t largestBeforeAlloc = GetLargestFreeBlock(pHeapManager);
		void * pPtr = alloc(pHeapManager, largestBeforeAlloc - HeapManager::s_MinumumToLeave);

		if (pPtr)
		{
			ShowFreeBlocks(pHeapManager);
			printf("\n");
#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
			printf("\n");

			size_t largestAfterAlloc = GgetLargestFreeBlock(pHeapManager);
			free(pHeapManager, pPtr);

			ShowFreeBlocks(pHeapManager);
#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
			printf("\n");

			Collect(pHeapManager);

			ShowFreeBlocks(pHeapManager);
#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
			printf("\n");

			size_t largestAfterCollect = GetLargestFreeBlock(pHeapManager);
		}
	}
#endif

	std::vector<void *> AllocatedAddresses;

	long	numAllocs = 0;
	long	numFrees = 0;
	long	numCollects = 0;

	bool	done = false;

	printf_s("Start test HeapManager.\n");
	pHeapManager->ShowFreeBlocks();
	pHeapManager->ShowUsedBlocks();

	// allocate memory of random sizes up to 1024 bytes from the heap manager
	// until it runs out of memory
	do
	{
		const size_t		maxTestAllocationSize = 1024;

		const unsigned int	alignments[] = { 4, 8, 16, 32, 64 };

		unsigned int	index = rand() % (sizeof(alignments) / sizeof(alignments[0]));

		unsigned int	alignment = alignments[index];

		size_t			sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

		void * pPtr = pHeapManager->alloc(sizeAlloc, alignment);

		assert((reinterpret_cast<uintptr_t>(pPtr) & (alignment - 1)) == 0);

		if (pPtr == nullptr)
		{
			pHeapManager->collect();

			pPtr = pHeapManager->alloc(sizeAlloc, alignment);

			if (pPtr == nullptr)
			{
				done = true;
				break;
			}
		}

		AllocatedAddresses.push_back(pPtr);
		numAllocs++;

		const unsigned int garbageCollectAboutEvery = 40;

		const unsigned int freeAboutEvery = 10;

		if (!AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0))
		{
			void * pPtr = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			bool success = pHeapManager->free(pPtr);
			assert(success);

			numFrees++;
		}

		if ((rand() % garbageCollectAboutEvery) == 0)
		{
			pHeapManager->collect();

			numCollects++;
		}

	} while (1);

	printf("After exhausting allocations:\n");

	pHeapManager->ShowFreeBlocks();
	pHeapManager->ShowUsedBlocks();

#ifdef __TRACK_ALLOCATIONS
	//pHeapManager->ShowOutstandingAllocations();
#endif // __TRACK_ALLOCATIONS
	printf("\n");

	// now free those blocks in a random order
	if (!AllocatedAddresses.empty())
	{
		// randomize the addresses
		std::random_shuffle(AllocatedAddresses.begin(), AllocatedAddresses.end());

		// return them back to the heap manager
		while (!AllocatedAddresses.empty())
		{
			void * pPtr = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			bool success = pHeapManager->Contains(pPtr);
			assert(success);

			success = pHeapManager->IsAllocated(pPtr);
			assert(success);

			success = pHeapManager->free(pPtr);
			assert(success);
		}

		printf("After freeing allocations:\n");

		pHeapManager->ShowFreeBlocks();
		pHeapManager->ShowUsedBlocks();
#ifdef __TRACK_ALLOCATIONS
		//ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS

		// do garbage collection
		pHeapManager->collect();
		// our heap should be one single block, all the memory it started with

		printf("After garbage collection:\n");

		pHeapManager->ShowFreeBlocks();
		pHeapManager->ShowUsedBlocks();
#ifdef __TRACK_ALLOCATIONS
		//ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS

		printf("\n");		// do a large test allocation to see if garbage collection worked
		void * pPtr = pHeapManager->alloc(sizeHeap / 2);
		assert(pPtr);

		if (pPtr)
		{
			bool success = pHeapManager->free(pPtr);
			assert(success);
		}
	}

	pHeapManager->destroy();
	pHeapManager = nullptr;

	_aligned_free(pHeapMemory);

	// we succeeded
	return true;
}

#endif // TestHeapManager

#ifdef TestVector
bool Vector2_UnitTest()
{
	Vector2 v1(1, 2);
	Vector2 v2 = v1 + Vector2::Unit();
	Vector2 v3(v2);
	v3 += v1;

	printf_s("Start test Vector2. \n");
	printf_s("v1 :");
	printf_s("(%f, %f)\n", v1[0], v1[1]);
	printf_s("v2 :");
	printf_s("(%f, %f)\n", v2[0], v2[1]);
	printf_s("v1 :");
	printf_s("(%f, %f)\n", v3[0], v3[1]);

	printf_s("Begin changes.\n");
	printf_s("v1 + v2: ");
	printf_s("(%f, %f)\n", (v1 + v2)[0], (v1 + v2)[1]);

	printf_s("v1 - v3: ");
	printf_s("(%f, %f)\n", (v1 - v3)[0], (v1 - v3)[1]);

	printf_s("v1 dot v2: %f \n", v1.Dot(v2));
	printf_s("v2 dot v3: %f \n", Vector2::Dot(v2, v3));


	printf_s("v1 * 3.5: ");
	printf_s("(%f, %f)\n", (v1 * 3.5f)[0], (v1 * 3.5f)[1]);

	printf_s("v3 / 2: ");
	printf_s("(%f, %f)\n", (v3 / 2.0f)[0], (v3 / 2.0f)[1]);

	printf_s("Distance betwwen v1 and v3: %f \n", v1.Distance(v3));

	printf_s("-v3: ");
	printf_s("(%f, %f)\n", (-v3)[0], (-v3)[1]);

	printf_s("Length of v3: %f, \n", v3.Length());

	printf_s("After v2 *= 5, v2: ");
	v2 *= 5;
	printf_s("(%f, %f)\n", v2[0], v2[1]);

	printf_s("After v3 normalize, v3: ");
	v3.Normalize();
	printf_s("(%f, %f)\n", v3[0], v3[1]);
	printf_s("\n");

	return true;
}

bool Vector3_UnitTest()
{
	Vector3 v1(1, 2, 3);
	Vector3 v2 = v1 + Vector3::Unit();
	Vector3 v3(v2);
	v3 += v1;

	printf_s("Start test Vector3. \n");
	printf_s("v1 :");
	printf_s("(%f, %f, %f)\n", v1[0], v1[1], v1[2]);

	printf_s("v2 :");
	printf_s("(%f, %f, %f)\n", v2[0], v2[1], v2[2]);

	printf_s("v3 :");
	printf_s("(%f, %f, %f)\n", v3[0], v3[1], v3[2]);

	printf_s("Begin changes.\n");
	printf_s("v1 + v2: ");
	printf_s("(%f, %f, %f)\n", (v1 + v2)[0], (v1 + v2)[1], (v1 + v2)[2]);

	printf_s("v1 - v3: ");
	printf_s("(%f, %f, %f)\n", (v1 - v3)[0], (v1 - v3)[1], (v1 - v3)[2]);

	printf_s("v1 dot v2: %f \n", v1.Dot(v2));
	printf_s("v2 dot v3: %f \n", Vector3::Dot(v2, v3));

	printf_s("v1 * 3.5: ");
	printf_s("(%f, %f, %f)\n", (v1 * 3.5f)[0], (v1 * 3.5f)[1], (v1 * 3.5f)[2]);

	printf_s("v3 / 2: ");
	printf_s("(%f, %f, %f)\n", (v3 / 2.0f)[0], (v3 / 2.0f)[1], (v3 / 2.0f)[2]);

	printf_s("Distance betwwen v1 and v3: %f \n", v1.Distance(v3));

	printf_s("-v3: ");
	printf_s("(%f, %f, %f)\n", (-v3)[0], (-v3)[1], (-v3)[2]);

	printf_s("Length of v3: %f, \n", v3.Length());

	printf_s("v1 cross product v2: ");
	printf_s("(%f, %f, %f)\n", (v1.Cross(v2))[0], (v1.Cross(v2))[1], (v1.Cross(v2))[2]);

	printf_s("v2 product v3: ");
	printf_s("(%f, %f, %f)\n", (Vector3::Cross(v2, v3))[0], (Vector3::Cross(v2, v3))[1], (Vector3::Cross(v2, v3))[2]);

	printf_s("After v2 *= 5, v2: ");
	v2 *= 5;
	printf_s("(%f, %f, %f)\n", v2[0], v2[1], v2[2]);

	printf_s("After v3 normalize, v3: ");
	v3.Normalize();
	printf_s("(%f, %f, %f)\n", v3[0], v3[1], v3[2]);

	printf_s("\n\n");

	return true;
}

bool Vector4_UnitTest()
{
	Vector4 v1(1, 2, 3);
	Vector4 v2 = v1 + Vector4::Unit();
	Vector4 v3(v2);
	v3 += v1;

	printf_s("Start test Vector3. \n");
	printf_s("v1 :");
	printf_s("(%f, %f, %f, %f)\n", v1[0], v1[1], v1[2], v1[3]);

	printf_s("v2 :");
	printf_s("(%f, %f, %f, %f)\n", v2[0], v2[1], v2[2], v2[3]);

	printf_s("v3 :");
	printf_s("(%f, %f, %f, %f)\n", v3[0], v3[1], v3[2], v3[3]);

	printf_s("Begin changes.\n");
	printf_s("v1 + v2: ");
	printf_s("(%f, %f, %f, %f)\n", (v1 + v2)[0], (v1 + v2)[1], (v1 + v2)[2], (v1 + v2)[3]);

	printf_s("v1 - v3: ");
	printf_s("(%f, %f, %f, %f)\n", (v1 - v3)[0], (v1 - v3)[1], (v1 - v3)[2], (v1 - v3)[3]);

	printf_s("v1 dot v2: %f \n", v1.Dot(v2));
	printf_s("v2 dot v3: %f \n", Vector4::Dot(v2, v3));

	printf_s("v1 * 3.5: ");
	printf_s("(%f, %f, %f, %f)\n", (v1 * 3.5f)[0], (v1 * 3.5f)[1], (v1 * 3.5f)[2], (v1 * 3.5f)[3]);

	printf_s("v3 / 2: ");
	printf_s("(%f, %f, %f, %f)\n", (v3 / 2.0f)[0], (v3 / 2.0f)[1], (v3 / 2.0f)[2], (v3 / 2.0f)[3]);

	printf_s("Distance betwwen v1 and v3: %f \n", v1.Distance(v3));

	printf_s("-v3: ");
	printf_s("(%f, %f, %f, %f)\n", (-v3)[0], (-v3)[1], (-v3)[2], (-v3)[3]);

	printf_s("Length of v3: %f, \n", v3.Length());

	printf_s("After v2 *= 5, v2: ");
	v2 *= 5;
	printf_s("(%f, %f, %f, %f)\n", v2[0], v2[1], v2[2], v2[3]);

	printf_s("After v3 normalize, v3: ");
	v3.Normalize();
	printf_s("(%f, %f, %f, %f)\n", v3[0], v3[1], v3[2], v3[3]);

	printf_s("\n\n");

	return true;
}
#endif // TestVector

#ifdef TestFixedAllocator
bool FixedAllocator_UnitTest()
{
	printf_s("Start test FixedAllocator.\n");

	//using namespace HeapManager;
	const size_t 		sizeHeap = 1024 * 1024;
	const unsigned int 	numDescriptors = 2048;

	const unsigned int defaultAlignment = 4;

	// Allocate memory for my test heap.
	void * pHeapMemory = _aligned_malloc(sizeHeap, defaultAlignment);
	SLOW_ASSERT(pHeapMemory, ErrorMessage::kNullPointer);

	// Create a heap manager for my test heap.
	HeapManager * pHeapManager = HeapManager::CreateHeapManager(pHeapMemory, sizeHeap, numDescriptors);
	SLOW_ASSERT(pHeapManager, ErrorMessage::kNullPointer);

	if (pHeapManager == nullptr)
		return false;

	printf_s("Alloc FixedAllocators.\n");

	size_t fixedBlockNumber4 = 40000;
	size_t fixedBlockNumber8 = 20000;
	size_t fixedBlockNumber16 = 10000;
	size_t fixedBlockNumber32 = 5000;
	size_t fixedBlockNumber64 = 2500;

	size_t additionalSize = 6000;

	size_t fixedMemorySize4 = fixedBlockNumber4 * 4 + additionalSize;
	size_t fixedMemorySize8 = fixedBlockNumber8 * 8 + additionalSize;
	size_t fixedMemorySize16 = fixedBlockNumber16 * 16 + additionalSize;
	size_t fixedMemorySize32 = fixedBlockNumber32 * 32 + additionalSize;
	size_t fixedMemorySize64 = fixedBlockNumber64 * 64 + additionalSize;

	void *fixedMemoryBase4 = pHeapManager->alloc(fixedMemorySize8, 4);
	void *fixedMemoryBase8 = pHeapManager->alloc(fixedMemorySize8, 8);
	void *fixedMemoryBase16 = pHeapManager->alloc(fixedMemorySize16, 16);
	void *fixedMemoryBase32 = pHeapManager->alloc(fixedMemorySize32, 32);
	void *fixedMemoryBase64 = pHeapManager->alloc(fixedMemorySize64, 64);

	FixedSizeAllocator *fixedAllocator4 = FixedSizeAllocator::Create(fixedMemoryBase4, fixedMemorySize4, fixedBlockNumber4, 4);
	FixedSizeAllocator *fixedAllocator8 = FixedSizeAllocator::Create(fixedMemoryBase8, fixedMemorySize8, fixedBlockNumber8, 8);
	FixedSizeAllocator *fixedAllocator16 = FixedSizeAllocator::Create(fixedMemoryBase16, fixedMemorySize16, fixedBlockNumber16, 16);
	FixedSizeAllocator *fixedAllocator32 = FixedSizeAllocator::Create(fixedMemoryBase32, fixedMemorySize32, fixedBlockNumber32, 32);
	FixedSizeAllocator *fixedAllocator64 = FixedSizeAllocator::Create(fixedMemoryBase64, fixedMemorySize64, fixedBlockNumber64, 64);

	std::vector<void *> AllocatedAddresses;

	long	numAllocs = 0;
	long	numFrees = 0;
	long	numCollects = 0;

	bool	done = false;

	pHeapManager->ShowFreeBlocks();
	pHeapManager->ShowUsedBlocks();

	// allocate memory of random sizes up to 1024 bytes from the heap manager
	// until it runs out of memory
	do
	{
		//const unsigned int	alignments[] = { 4, 8, 16, 32,  };

		//unsigned int	index = rand() % (sizeof(alignments) / sizeof(alignments[0]));

		//unsigned int	alignment = alignments[index];

		unsigned int ratio = (rand() % 32);
		size_t sizeAlloc;
		if (ratio < 16)
			sizeAlloc = 1 + rand() % 4;
		else if (ratio < 24)
			sizeAlloc = 5 + rand() % 4;
		else if (ratio < 28)
			sizeAlloc = 9 + rand() % 8;
		else if (ratio < 30)
			sizeAlloc = 17 + rand() % 16;
		else if (ratio < 31)
			sizeAlloc = 33 + rand() % 32;
		else
			sizeAlloc = 65 + rand() % 64;

		void * ptr = nullptr;
		if (sizeAlloc <= 4)
			ptr = fixedAllocator4->alloc();
		else if (sizeAlloc <= 8)
			ptr = fixedAllocator8->alloc();
		else if (sizeAlloc <= 16)
			ptr = fixedAllocator16->alloc();
		else if (sizeAlloc <= 32)
			ptr = fixedAllocator32->alloc();
		else if (sizeAlloc <= 64)
			ptr = fixedAllocator64->alloc();

		if (ptr == nullptr || sizeAlloc > 64)
			ptr = pHeapManager->alloc(sizeAlloc, defaultAlignment);

		if (ptr == nullptr)
		{

			pHeapManager->collect();

			ptr = pHeapManager->alloc(sizeAlloc, defaultAlignment);

			if (ptr == nullptr)
			{
				done = true;
				break;
			}
		}

		AllocatedAddresses.push_back(ptr);
		numAllocs++;

		const unsigned int garbageCollectAboutEvery = 40;

		const unsigned int freeAboutEvery = 10;

		if (!AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0))
		{
			void * pPtr = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			bool success;
			if (fixedAllocator4->Contains(pPtr))
				success = fixedAllocator4->free(pPtr);
			else if (fixedAllocator8->Contains(pPtr))
				success = fixedAllocator8->free(pPtr);
			else if (fixedAllocator16->Contains(pPtr))
				success = fixedAllocator16->free(pPtr);
			else if (fixedAllocator32->Contains(pPtr))
				success = fixedAllocator32->free(pPtr);
			else if (fixedAllocator64->Contains(pPtr))
				success = fixedAllocator64->free(pPtr);
			else
				success = pHeapManager->free(pPtr);
			assert(success);

			numFrees++;
		}

		if ((rand() % garbageCollectAboutEvery) == 0)
		{
			pHeapManager->collect();

			numCollects++;
		}

	} while (1);

	printf("After exhausting allocations:\n");
	printf("Total allocation times %d.\n\n", numAllocs);

	pHeapManager->ShowFreeBlocks();
	pHeapManager->ShowUsedBlocks();

	printf("\n");

	// now free those blocks in a random order
	if (!AllocatedAddresses.empty())
	{
		// randomize the addresses
		std::random_shuffle(AllocatedAddresses.begin(), AllocatedAddresses.end());

		// return them back to the heap manager
		while (!AllocatedAddresses.empty())
		{
			void * pPtr = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			bool success;

			if (!fixedAllocator64->Contains(pPtr))
			{
				int a = 1;
				a++;
			}

			if (fixedAllocator4->Contains(pPtr))
			{
				success = fixedAllocator4->IsAllocated(pPtr);
				assert(success);
				success = fixedAllocator4->free(pPtr);
				assert(success);
			}
			else if (fixedAllocator8->Contains(pPtr))
			{
				success = fixedAllocator8->IsAllocated(pPtr);
				assert(success);
				success = fixedAllocator8->free(pPtr);
				assert(success);
			}
			else if (fixedAllocator16->Contains(pPtr))
			{
				success = fixedAllocator16->IsAllocated(pPtr);
				assert(success);
				success = fixedAllocator16->free(pPtr);
				assert(success);
			}
			else if (fixedAllocator32->Contains(pPtr))
			{
				success = fixedAllocator32->IsAllocated(pPtr);
				assert(success);
				success = fixedAllocator32->free(pPtr);
				assert(success);
			}
			else if (fixedAllocator64->Contains(pPtr))
			{
				success = fixedAllocator64->IsAllocated(pPtr);
				assert(success);
				success = fixedAllocator64->free(pPtr);
				assert(success);
			}
			else
			{
				success = pHeapManager->IsAllocated(pPtr);
				assert(success);
				success = pHeapManager->free(pPtr);
				assert(success);
			}
		}

		printf("After freeing allocations:\n");

		// do garbage collection
		pHeapManager->collect();

		pHeapManager->ShowFreeBlocks();
		pHeapManager->ShowUsedBlocks();

		printf("Freeing fixed size allocators\n");

		pHeapManager->free(fixedAllocator4);
		pHeapManager->free(fixedAllocator8);
		pHeapManager->free(fixedAllocator16);
		pHeapManager->free(fixedAllocator32);
		pHeapManager->free(fixedAllocator64);

		// do garbage collection
		pHeapManager->collect();

		pHeapManager->ShowFreeBlocks();
		pHeapManager->ShowUsedBlocks();

		printf("\n");		// do a large test allocation to see if garbage collection worked
		void * pPtr = pHeapManager->alloc(sizeHeap / 2);
		assert(pPtr);

		if (pPtr)
		{
			bool success = pHeapManager->free(pPtr);
			assert(success);
		}
	}

	pHeapManager->destroy();
	_aligned_free(pHeapMemory);

	pHeapManager = nullptr;

	// we succeeded
	return true;
}
#endif // TestFixedAllocator

#ifdef TestBitArray
bool BitArray_UnitTest(void)
{
	const size_t bitCount = 1000;

	BitArray* pMyArray = BitArray::Create(bitCount);

	pMyArray->SetBit(20);

	size_t firstSetBit = 0;
	size_t firstClearBit = 0;

	bool foundSetBit = pMyArray->GetFirstSetBit(firstSetBit);
	assert(foundSetBit && (firstSetBit == 20));

	pMyArray->ClearBit(20);
	foundSetBit = pMyArray->GetFirstSetBit(firstSetBit);
	assert(foundSetBit == false);

	for (unsigned int i = 0; i < bitCount; i++)
	{
		assert(pMyArray->IsBitClear(i) == true);
		assert(pMyArray->IsBitSet(i) == false);

		size_t bit = 0;

		pMyArray->GetFirstClearBit(bit);
		assert(bit == i);

		pMyArray->SetBit(i);

		assert(pMyArray->IsBitClear(i) == false);
		assert(pMyArray->IsBitSet(i) == true);

		bool success = pMyArray->GetFirstClearBit(bit);
		assert(((i < (bitCount - 1)) && success && (bit == (i + 1))) || ((i == (bitCount - 1)) && !success));
	}

	pMyArray->SetAll();
	assert(pMyArray->GetFirstClearBit(firstClearBit) == false);

	pMyArray->ClearAll();
	assert(pMyArray->GetFirstSetBit(firstSetBit) == false);

	delete pMyArray;

	return true;
}
#endif // TestBitArray

#ifdef TestContainer
bool Container_UnitTest()
{
	LinkedList<Vector3> testLinkedList;
	testLinkedList.PushHead(Vector3(0, 0, 0));
	testLinkedList.InsertNext(testLinkedList.Head(), Vector3(1, 1, 1));
	testLinkedList.DeleteNext(testLinkedList.Head());
	testLinkedList.PopHead();
	testLinkedList.Clear();

	List<Vector3> testList;
	testList.PushHead(Vector3(0, 0, 0));
	testList.PushTail(Vector3(1, 1, 1));
	testList.InsertNext(testList.Tail(), Vector3(2, 2, 2));
	testList.Delete(testList.Head());
	testList.PopTail();
	testList.PopHead();
	testList.Clear();

	CircleQueue<Vector3> circleQueue(3);
	bool empty = circleQueue.Empty();
	circleQueue.Push(Vector3(1, 1, 1));
	circleQueue.Push(Vector3(2, 2, 2));
	circleQueue.Push(Vector3(3, 3, 3));
	Vector3 test = circleQueue.Head();
	test = circleQueue.Tail();
	empty = circleQueue.Empty();
	circleQueue.Push(Vector3(4, 4, 4));
	test = circleQueue.Head();
	test = circleQueue.Tail();
	circleQueue.Push(Vector3(5, 5, 5));
	circleQueue.Pop();
	test = circleQueue.Head();
	test = circleQueue.Tail();
	circleQueue.Clear();

	Array<Vector3> testArray;
	empty = testArray.Empty();
	size_t size = testArray.Size();
	testArray.Add(Vector3(1, 1, 1));
	testArray.Add(Vector3(2, 2, 2), 0);
	size = testArray.Size();
	testArray.Add(Vector3(3, 3, 3));
	testArray.Add(Vector3(4, 4, 4), 0);

	testArray.NoOrderRemove(0);
	testArray.Remove(1);
	testArray.Remove();
	Array<Vector3> testArray2 = testArray;
	testArray2.Clear();

	return true;
}
#endif // TestContainer

#ifdef TestSmartPointer
bool SmartPointer_UnitTest()
{
	GameObject* testObject1 = new GameObject(Vector3(0, 0, 0));
	GameObject* testObject2 = new GameObject(Vector3(1, 1, 1));
	{
		StrongPtr<GameObject> sp1(testObject1);
		StrongPtr<GameObject> sp2(sp1);
		StrongPtr<GameObject> sp3(testObject2);
		StrongPtr<GameObject> sp4 = sp2;
		sp4 = sp3;

		printf_s("Sp1 has object? %c\n", (sp1) ? 'Y' : 'N');
		printf_s("Sp1 == Sp2? %c\n", (sp1 == sp2) ? 'Y' : 'N');
		printf_s("Sp1 object location: %f, %f, %f\n", (*sp1).GetPosition().X, (*sp1).GetPosition().Y, (*sp1).GetPosition().Z);

		WeakPtr<GameObject> wp1(sp1);
		WeakPtr<GameObject> wp2(sp2);
		WeakPtr<GameObject> wp3(sp3);

		wp1.Acquire();
		printf_s("Wp1 has object? %c\n", (wp1) ? 'Y' : 'N');
		printf_s("Wp1 == Wp2? %c\n", (wp1 == wp2) ? 'Y' : 'N');
		printf_s("Wp3 object location: %f, %f, %f\n", (*wp3.Acquire()).GetPosition().X, (*wp3.Acquire()).GetPosition().Y, (*wp3.Acquire()).GetPosition().Z);

		sp1.ClearPointer();
		sp2.ClearPointer();
		sp3.ClearPointer();
		sp4.ClearPointer();
		printf_s("Sp1 has object? %c\n", (sp1) ? 'Y' : 'N');
		printf_s("Sp3 has object? %c\n", (sp3) ? 'Y' : 'N');

		printf_s("Wp1 has object? %c\n", (wp1) ? 'Y' : 'N');
		printf_s("Wp3 has object? %c\n", (wp3) ? 'Y' : 'N');

	}
	testObject1 = nullptr;
	testObject2 = nullptr;

	return true;
}
#endif // TestSmartPointer

#ifdef TestMatrix
bool Matrix_UnitTest()
{
	Vector3 position(0, 0, 0);
	Matrix m1 = Matrix::CreateScale(1, 2, 0.5f);
	Matrix m2 = Matrix::CreateXRotation(Float::DegreesToRadians(30));
	Matrix m3 = Matrix::CreateTranslate(Vector3(1, 2, 3));

	printf_s("position : (%f, %f, %f)\n", position[0], position[1], position[2], position[3]);

	printf_s("Identity Matrix m1 :\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n\n",
		m1[0], m1[1], m1[2], m1[3], m1[4], m1[5], m1[6], m1[7], m1[8], m1[9], m1[10], m1[11], m1[12], m1[13], m1[14], m1[15]);

	printf_s("m2 :\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n\n",
		m2[0], m2[1], m2[2], m2[3], m2[4], m2[5], m2[6], m2[7], m2[8], m2[9], m2[10], m2[11], m2[12], m2[13], m2[14], m2[15]);

	printf_s("m3 :\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n\n",
		m3[0], m3[1], m3[2], m3[3], m3[4], m3[5], m3[6], m3[7], m3[8], m3[9], m3[10], m3[11], m3[12], m3[13], m3[14], m3[15]);

	position = Vector3(Vector4(position, 1.0f).Transform(m1));
	printf_s("After scaled by m1, position : (%f, %f, %f)\n", position[0], position[1], position[2], position[3]);

	position = Vector3(Vector4(position, 1.0f).Transform(m2));
	printf_s("After rotationed by m2, position : (%f, %f, %f)\n", position[0], position[1], position[2], position[3]);

	position = Vector3(Vector4(position, 1.0f).Transform(m3));
	printf_s("After translated by m3, position : (%f, %f, %f)\n", position[0], position[1], position[2], position[3]);

	m1 = m1 * m2;
	m1 *= m3;
	printf_s("After (m1 * m2) *= m3 m2:\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n\n",
		m1[0], m1[1], m1[2], m1[3], m1[4], m1[5], m1[6], m1[7], m1[8], m1[9], m1[10], m1[11], m1[12], m1[13], m1[14], m1[15]);

	position = Vector3(Vector4(0, 0, 0, 1).Transform(m1));
	printf_s("After transformed by new m1, (0, 0, 0) bacome to: (%f, %f, %f)\n", position[0], position[1], position[2], position[3]);
}
#endif // TestMatrix

#ifdef TestLua
bool Lua_UnitTest()
{
	// Initialize us a lua_State
	lua_State * pLuaState = luaL_newstate();
	ASSERT(pLuaState);

	luaL_openlibs(pLuaState);

	// Read in a file
	size_t sizeFile = 0;
	uint8_t *pFileContents = LoadFile("TestData\\Data\\Player.lua", sizeFile);

	if (pFileContents && sizeFile)
	{
		int	result = 0;

		// Do the initial buffer parsing
		result = luaL_loadbuffer(pLuaState, reinterpret_cast<char *>(pFileContents), sizeFile, nullptr);
		ASSERT(result == 0);
		lua_pcall(pLuaState, 0, 0, 0);
		ASSERT(result == 0);

		int	type = LUA_TNIL;

		// Find the global variable named "Player" and push it onto stack.
		// If it doesn't find it it pushes a nil value instead
		result = lua_getglobal(pLuaState, "Player");
		ASSERT(result == LUA_TTABLE);

		// Get the Actor name from the Player table
		// 1. Push the key string
		lua_pushstring(pLuaState, "name");
		// 2. Get the value associated with the key
		type = lua_gettable(pLuaState, -2);
		ASSERT(type == LUA_TSTRING);
		const char * pName = lua_tostring(pLuaState, -1);
		ASSERT(pName != NULL);
		if (pName)
			pName = _strdup(pName);

		free(const_cast<char *>(pName));

		// 3. Remove the value from the stack now that we're done with it
		lua_pop(pLuaState, 1);

		// Get the Actors name from the Player table
		lua_pushstring(pLuaState, "initial_position");
		type = lua_gettable(pLuaState, -2);
		ASSERT(type == LUA_TTABLE);

		float Position[3] = { 0.0f };
		ReadFloatArray(pLuaState, -1, Position, 3);

		// we're done with the "initial_position table - pop it
		lua_pop(pLuaState, 1);

		lua_pushstring(pLuaState, "bounding_box");
		type = lua_gettable(pLuaState, -2);
		ASSERT(type == LUA_TTABLE);

		// get bounding box offset
		lua_pushstring(pLuaState, "offset");
		type = lua_gettable(pLuaState, -2);
		ASSERT(type == LUA_TTABLE);

		float BoundingBox_Offset[3] = { 0.0f };
		ReadFloatArray(pLuaState, -1, BoundingBox_Offset, 3);

		// we're done with the "offset" table - pop it
		lua_pop(pLuaState, 1);

		lua_pushstring(pLuaState, "size");
		type = lua_gettable(pLuaState, -2);
		ASSERT(type == LUA_TTABLE);

		float BoundingBox_Size[3] = { 0.0f };
		ReadFloatArray(pLuaState, -1, BoundingBox_Size, 3);

		// we're done with the "size" table - pop it
		lua_pop(pLuaState, 1);
		// we're done with the "bounding_box" table - pop it
		lua_pop(pLuaState, 1);

		// let's process the "physics_settings" sub table
		lua_pushstring(pLuaState, "physics_settings");
		type = lua_gettable(pLuaState, -2);
		// It either needs to be a table or not there (no physics applied)
		ASSERT((type == LUA_TNIL) || (type == LUA_TTABLE));

		if (type == LUA_TTABLE)
		{
			lua_pushstring(pLuaState, "mass");
			type = lua_gettable(pLuaState, -2);
			assert(type == LUA_TNUMBER);

			lua_pushstring(pLuaState, "drag");
			type = lua_gettable(pLuaState, -3);
			assert(type == LUA_TNUMBER);

			float mass = float(lua_tonumber(pLuaState, -2));
			float drag = float(lua_tonumber(pLuaState, -1));

			// pop the "mass" and "drag" values together
			lua_pop(pLuaState, 2);
		}

		// we're done with the "physics_settings" table - pop it
		// NOTE: If it didn't exist Lua would have pushed a nil value so still need the pop
		lua_pop(pLuaState, 1);

		// let's process the "render_settings" sub table
		lua_pushstring(pLuaState, "render_settings");
		type = lua_gettable(pLuaState, -2);
		assert((type == LUA_TNIL) || (type == LUA_TTABLE));

		if (type == LUA_TTABLE)
		{
			lua_pushstring(pLuaState, "sprite");
			type = lua_gettable(pLuaState, -2);
			assert(type == LUA_TTABLE);
			{
				lua_pushstring(pLuaState, "offset");
				type = lua_gettable(pLuaState, -2);
				assert(type == LUA_TTABLE);

				float Sprite_Offset[2] = { 0.0f };
				ReadFloatArray(pLuaState, -1, Sprite_Offset, 2);

				// pop "offset" table
				lua_pop(pLuaState, 1);

				lua_pushstring(pLuaState, "extents");
				type = lua_gettable(pLuaState, -2);
				assert(type == LUA_TTABLE);

				float Sprite_Extents[4] = { 0.0f };
				ReadFloatArray(pLuaState, -1, Sprite_Extents, 4);

				// pop "extents" table
				lua_pop(pLuaState, 1);


			}
			// pop "sprite" table
			lua_pop(pLuaState, 1);

			lua_pushstring(pLuaState, "sprite_color");
			type = lua_gettable(pLuaState, -2);
			if (type == LUA_TTABLE)
			{
				int			SpriteRGBA[4];
				ReadIntArray(pLuaState, -1, SpriteRGBA, 4);
			}

			// pop "sprite_color" table
			lua_pop(pLuaState, 1);
		}

		// pop "render_settings" table
		lua_pop(pLuaState, 1);

		// pop "Player" table
		lua_pop(pLuaState, 1);

		int stack_items = lua_gettop(pLuaState);
		assert(stack_items == 0);

	}

	delete[] pFileContents;

	//lua_gc(pLuaState, LUA_GCCOLLECT, 0);

	lua_close(pLuaState);

	return true;
}
#endif // TestLua

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	bool res = true;

	Engine engine;
	engine.EngineStartup();

	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "UnitTest", -1, 800, 600);


#ifdef TestHeapManager
	res = res && HeapManager_UnitTest();
#endif // TestHeapManager
	
#ifdef TestVector
	res = res && Vector2_UnitTest() && Vector3_UnitTest() && Vector4_UnitTest();
#endif // TestVector

#ifdef TestFixedAllocator
	res = res && FixedAllocator_UnitTest();
#endif // TestFixedAllocator

#ifdef TestBitArray
	res = res && BitArray_UnitTest();
#endif // TestBitArray

#ifdef TestContainer
	res = res && Container_UnitTest();
#endif // TestContainer

#ifdef TestSmartPointer
	res = res && SmartPointer_UnitTest();
#endif // TestSmartPointer

#ifdef TestMatrix
	res = res && Matrix_UnitTest();
#endif // TestMatrix

#ifdef TestLua
	res = res && Lua_UnitTest();
#endif // TestLua

	bool quit = true;
	GLib::Service(quit);

	engine.EngineShutdown();


#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
	return res;
}