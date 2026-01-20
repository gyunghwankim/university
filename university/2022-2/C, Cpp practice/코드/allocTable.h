#ifndef ALLOC_TABLE_H
#define ALLOC_TABLE_H

#include <stdio.h>
#include <malloc.h>
#include <assert.h>


// index overflow를 해결하지 않는다.
static void* allocTable[5000] = { NULL, };
static int allocCount = 0;




// private
inline void addAlloc(void* _p)
{
	for (int i = 0; i < allocCount; ++i)
	{
		if (_p == allocTable[i])
		{
			return;
		}
	}

	allocTable[allocCount++] = _p;
}




// private
inline void subAlloc(void* _p)
{
	for (int i = 0; i < allocCount; ++i)
	{
		if (_p == allocTable[i])
		{
			free(_p);

			for (; i < allocCount - 1; ++i)
			{
				allocTable[i] = allocTable[i + 1];
			}

			allocTable[(allocCount--) - 1] = NULL;

			return;
		}
	}

	// not find _p in allocTable
	fprintf(stderr, "subAlloc(): pointer not found in allocTable, p = %p", _p);
}





void* myMalloc(size_t _bufferSize)
{
	void* buffer = malloc(_bufferSize);
	if (buffer == NULL)
	{
		fprintf(stderr, "myMalloc() failed of size %zu\n", _bufferSize);
		assert(0);
	}

	addAlloc(buffer);
	return buffer;
}





void* myCalloc(size_t _bufferSize)
{
	void* buffer = calloc(1, _bufferSize);
	if (buffer == NULL)
	{
		fprintf(stderr, "myCalloc() failed of size %zu\n", _bufferSize);
		assert(0);
	}

	addAlloc(buffer);
	return buffer;
}





void myFree(void* _p)
{
	subAlloc(_p);
}





void myFreeAll()
{
	while (allocCount)
	{
		free(allocTable[--allocCount]);
		allocTable[allocCount] = NULL;
	}
}












#endif // ALLOC_TABLE_H