//Andre Barajas
#include "MemoryManager.h"
#include <iomanip>
#include <iostream>
using namespace std;

namespace MemoryManager
{
	// IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
	//
	// This is the only static memory that you may use, no other global variables
	// may be created, if you need to save data make it fit in MM_pool
	//
	// IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT

	const int MM_POOL_SIZE = 65536;
	char MM_pool[MM_POOL_SIZE];

	// I have provided this tool for your use
	void memView(int start, int end)
	{
		const unsigned int SHIFT = 8;
		const unsigned int MASK = 1 << SHIFT - 1;

		unsigned int value;	// used to facilitate bit shifting and masking

		cout << "         Pool                     Unsignd  Unsigned " << endl;
		cout << "Mem Add  indx   bits   chr ASCII#  short      int    " << endl;
		cout << "-------- ---- -------- --- ------ ------- ------------" << endl;

		for (int i = start; i <= end; i++)
		{
			cout << (long*)(MM_pool + i) << ':';	// the actual address in hexadecimal
			cout << '[' << setw(2) << i << ']';		// the index into MM_pool

			value = MM_pool[i];
			cout << " ";
			for (int j = 1; j <= SHIFT; j++)	// the bit sequence for this byte (8 bits)
			{
				cout << ((value & MASK) ? '1' : '0');
				value <<= 1;
			}
			cout << " ";

			cout << '|' << *(char*)(MM_pool + i) << "| (";	// the ASCII character of the 8 bits (1 byte)
			cout << setw(4) << ((int)(*((unsigned char*)(MM_pool + i)))) << ")";// the ASCII number of the character

			cout << " (" << setw(5) << (*(unsigned short*)(MM_pool + i)) << ")";// the unsigned short value of 16 bits (2 bytes)
			cout << " (" << setw(10) << (*(unsigned int*)(MM_pool + i)) << ")";	// the unsigned int value of 32 bits (4 bytes)

			cout << endl;
		}
	}

	// Initialize set up any data needed to manage the memory pool
	void initializeMemoryManager(void)
	{
		int  y= 0;
		int x = 2;
		int z = 4;
		*(unsigned short*)(MM_pool + y) = 6;
		*(unsigned short*)(MM_pool + 6) = MM_POOL_SIZE - 6;
		*(unsigned short*)(MM_pool + x) = 0;
		*(unsigned short*)(MM_pool + z) = 0;
	}

	// Return a pointer inside the memory pool
	// If no chunk can accommodate aSize call onOutOfMemory()
	void* allocate(int aSize)
	{
		int y = 0;
		int x = 2;
		int U = 4;
		if (freeMemory() > aSize)
		{
			int size = aSize + 6;
			int oldy = *(unsigned short*)(MM_pool + y);
			int oldx = *(unsigned short*)(MM_pool + x);
			int nexty = *(unsigned short*)(MM_pool + y) + size;
			int oldPrvLnk = oldx + 4;
			int currNxtLnk = oldy + 2;
			int currPrvLnk = oldy + 4;
			int currx = *(unsigned short*)(MM_pool + x);
			*(unsigned short*)(MM_pool + x) = oldy;
			*(unsigned short*)(MM_pool + y) = nexty;
			if (oldy != 6)
			{
				*(unsigned short*)(MM_pool + oldPrvLnk) = oldy;
			}
			*(unsigned short*)(MM_pool + currNxtLnk) = oldx;
			*(unsigned short*)(MM_pool + currPrvLnk) = 0;
			*(unsigned short*)(MM_pool + oldy) = aSize;
			*(unsigned short*)(MM_pool + nexty) = MM_POOL_SIZE - nexty;
			return (void*)(MM_pool + *(unsigned short*)MM_pool - aSize);
		}
		else
		{
			onOutOfMemory();
			return 0;
		}
	}

	// Free up a chunk previously allocated memory
	void deallocate(void* aPointer)
	{
		int size = MM_pool[((char*)aPointer - MM_pool) - 6] + 6;
		int index = (char*)aPointer - MM_pool - 6;
		int currDelNode = index;
		int y = 0;
		int x = 2;
		int U = 4;
		int nextLink = 2;
		int prevLink = 4;
		int prevNode = *(unsigned short*)(MM_pool + currDelNode + nextLink);
		int nextNode = *(unsigned short*)(MM_pool + currDelNode + prevLink);
		*(unsigned short*)(MM_pool + prevNode + prevLink) = nextNode;
		*(unsigned short*)(MM_pool + nextNode + nextLink) = prevNode;
		int oldU = *(unsigned short*)(MM_pool + U);
		*(unsigned short*)(MM_pool + currDelNode + prevLink) = 0;
		*(unsigned short*)(MM_pool + currDelNode + nextLink) = oldU;
		*(unsigned short*)(MM_pool + oldU + prevLink) = currDelNode;
		*(unsigned short*)(MM_pool + U) = currDelNode;
	}

	// Scan the memory pool and return the total free space remaining in bytes
	int freeMemory(void)
	{
		int y = 0;
		return MM_POOL_SIZE - *(unsigned short*)(MM_pool + y);
	}

	// Scan the memory pool and return the total deallocated memory in bytes
	int usedMemory(void)
	{
		int usedMem = 0;
		int nextUsedMem = *(unsigned short*)(MM_pool + 4);
		while (nextUsedMem > 0)
		{
			usedMem = usedMem + *(unsigned short*)(MM_pool + nextUsedMem) + 6;
			nextUsedMem = *(unsigned short*)(MM_pool + nextUsedMem + 2);
		}
		return usedMem;
	}

	// Scan the memory pool and return the total in use memory
	int inUseMemory(void)
	{
		int totalUsedMem = MM_POOL_SIZE - freeMemory() - 6;
		return totalUsedMem - usedMemory();
	}

	// Return the size (in bytes) associated with this memory address
	int size(void * aPointer)
	{
		unsigned short size = (MM_pool[(char*)aPointer - (char*)MM_pool] + 2);
		return size;
	}

	// This is called when no more memory is available to allocate
	void onOutOfMemory(void)
	{
		std::cerr << "/nMemory pool out of memory" << std::endl;
		cin.get();
		exit(1);
	}
}
