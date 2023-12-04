#include "stdafx.h"
#include "CMalloc.h"
inline CMalloc::CMalloc():_pool32(10,false),_pool64(10,false),_pool128(10,false),_pool256(10,false),_pool512(10,false),_pool1024(10,false),
_pool2048(10,false),_pool4096(10,false)
{

}

inline int CMalloc::findSize(__int32 size)
{	
	//초과사이즈 , 0<값 
	if (size <= 0) {
		__debugbreak();
	}
	if (size > 4096) {
		return 9;
	}
	//비트 연산
		char type = 32 - __lzcnt((size - 1) >> 4); // 비트 함수 채우고 
		return type;
}
	

inline void* CMalloc::newMalloc(__int32 size)
{
	char type=CMalloc::findSize(size);

	switch (type)
	{
	case 0:
	{
		void* allocData = reinterpret_cast<char*>(_pool32.Alloc()) + sizeof(__int32);
		return allocData;
	}
		break;
	case 1:
		
	{//리스트 생성하는 함수 만들기 33byte
		void* allocData = reinterpret_cast<char*>(_pool64.Alloc()) + sizeof(__int32);
		return allocData;
	}
		break;
	case 2:
	{
		void* allocData = reinterpret_cast<char*>(_pool64.Alloc()) + sizeof(__int32);
		return allocData;
	}
	case 3: {

		void* allocData = reinterpret_cast<char*>(_pool128.Alloc()) + sizeof(__int32);
		return allocData;
	}
		break;
	case 4: {

		void* allocData = reinterpret_cast<char*>(_pool256.Alloc()) + sizeof(__int32);
		return allocData;
	}
		break;
	case 5:
	{

		void* allocData = reinterpret_cast<char*>(_pool512.Alloc()) + sizeof(__int32);
		return allocData;		
	}
		break;
	case 6: {

		void* allocData = reinterpret_cast<char*>(_pool1024.Alloc()) + sizeof(__int32);
		return allocData;		
	}
		break;
	case 7: {

		void* allocData = reinterpret_cast<char*>(_pool2048.Alloc()) + sizeof(__int32);
		return allocData;		
	}
		  break;
	case 8:
	{

		void* allocData = reinterpret_cast<char*>(_pool4096.Alloc()) + sizeof(__int32);
		return allocData;		
	}
		break;
	case 9:{

			char* overSize = (char*)malloc(size); 
			return overSize;
		}
		break;
	default:
		break;
	}
}

inline void CMalloc::newFree(void* freeData)
{

	char* type =(reinterpret_cast<char*>(freeData)) - sizeof(__int32);
	switch (*type)
	{
	case 1:
	{
		//리스트 생성하는 함수 만들기 33byte
		_pool32.Free(reinterpret_cast<CMalloc::stMalloc<32>*>(type));
	}
		break;
	case 2: {
		_pool64.Free(reinterpret_cast<CMalloc::stMalloc <64>* > (type));
	}
		  break;
	case 3: {
		_pool128.Free(reinterpret_cast<CMalloc::stMalloc<128>*>(type));
	}
		break;
	case 4:
	{		
		_pool256.Free(reinterpret_cast<CMalloc::stMalloc<256>*>(type));
	}
		break;
	case 5:
	{
		_pool512.Free(reinterpret_cast<CMalloc::stMalloc<512>*>(type));
	}
		break;
	case 6:
	{
		_pool1024.Free(reinterpret_cast<CMalloc::stMalloc<1024>*>(type));
	}
		break;
	case 7:
	{
		_pool2048.Free(reinterpret_cast<CMalloc::stMalloc<2048>*>(type));
	}
		break;
	case 8:
	{
		_pool4096.Free(reinterpret_cast<CMalloc::stMalloc<4096>*>(type));
	}
		break;
	default:
		free(freeData); //malloc사이즈를 받지 않은 경우 
		break;
	}

}
