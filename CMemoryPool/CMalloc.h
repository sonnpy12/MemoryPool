#pragma once
#include "CMemoryPool.h"


class CMalloc
{
private:
	template<int SIZE>
	struct stMalloc
	{
		__int32		size=SIZE;
		char		data[SIZE];
	};
private:
	CMemoryPool<stMalloc<32>>		_pool32;
	CMemoryPool<stMalloc<64>>		_pool64;
	CMemoryPool<stMalloc<128>>		_pool128;
	CMemoryPool<stMalloc<256>>		_pool256;
	CMemoryPool<stMalloc<512>>		_pool512;
	CMemoryPool<stMalloc<1024>>		_pool1024;
	CMemoryPool<stMalloc<2048>>		_pool2048;
	CMemoryPool<stMalloc<4096>>		_pool4096;
public:
	CMalloc();
	void*	newMalloc(__int32 size);
	void	newFree(void* freeData);
private:
	int	findSize(__int32 size);
	
};
