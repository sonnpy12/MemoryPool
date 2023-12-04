#include "stdafx.h"
#include "CMemoryPool.h"
#include"CTest.h"
#include"CMalloc.h"


int main() {

	
	/*unsigned long type = (127 - 1)>>4;
	unsigned long index;
	unsigned long result;
	result = 31-__lzcnt(type);*/

	//CMemoryPool<CTest> MemPool(10,false); 
	//CMemoryPool<CTest> MemPool2(1, false); 
	/*CTest* cTest = MemPool.Alloc();
	char* p = (char*)cTest;
	p -= 8;
	*p = 10;
	MemPool.Free(cTest);*/
	
	//for (int i = 0; i < 10000; i++) {

	//	CTest* pData = MemPool.Alloc();
	//	MemPool.Free(pData);
	//}
	//for (int i = 0; i < 10000; i++) {
	//	CTest* cTest = new CTest;
	//	delete	cTest;
	//}
	/*CMalloc test;
	CTest* test2= (CTest*)test.newMalloc(5);
	CTest* test3 = (CTest*)test.newMalloc(31);
	CTest* test4 = (CTest*)test.newMalloc(55);
	CTest* test5 = (CTest*)test.newMalloc(155);
	CTest* test6 = (CTest*)test.newMalloc(300);
	CTest* test7 = (CTest*)test.newMalloc(990);
	CTest* test8 = (CTest*)test.newMalloc(2000);
	CTest* test9 = (CTest*)test.newMalloc(3050);
	CTest* test10 = (CTest*)test.newMalloc(5000);
	test.newFree(test10);*/
	
	CTest::GetInstance();

}