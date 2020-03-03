#pragma once
//Andre Barajas


#ifndef __MEM_MENAGER_H__
#define __MEM_MENAGER_H__

//IMPORTANT DO NOT CHANGE THIS HEADER FILE

namespace MemoryManager
{
	void memView(int start, int finish);	//SHow the contents of meory from start to finish
	void initializeMemoryManager(void);	//Itiialize any data needed to manage the memory poool
	void *allocate(int);		//Return a pointer inside the moemory pool
	void deallocate(void *);	//Free up a chunck previously allocated
	int freeMemory(void);	//Scan memory pool, return the total free space remaining
	int inUseMemory(void); //Scann Memory pool and return the total memory being currently used.
	void onOutOfMemory(void);	//Call if no space is left for the allocation rquest
	int usedMemory(void);	//Scan memory pool, reutrn the total used memory that has been deallocated.

	/*
	int size(void*);	//Return the size (inbytes)of the variable that lives at this address.
	*/
};
#endif//__MEM_MENAGER_H__
