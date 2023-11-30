#pragma once
#include"stdafx.h"
//메모리 풀 클래스(오브젝트 풀 / 프리리스트)
//특정 데이타(구조체, 클래스, 변수)를 일정량 할당 후 나눠쓴다.
//
//- 사용법.
//
//procademy::CMemoryPool<DATA> MemPool(300, FALSE);
//DATA* pData = MemPool.Alloc();
//
//pData 사용
//
//MemPool.Free(pData);
//
//
//----------------------------------------------------------------


template <class DATA>
class CMemoryPool
{

private:
	struct stBlock {
		stBlock* prev;
		stBlock* next;
		DATA data;
	};
	stBlock*	_head;
	stBlock*	_tail;
	int			_capapcity;
	int			_useCount;
	bool		_placementNew;
public:

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Parameters:	(int) 초기 블럭 개수.
	//				(bool) Alloc 시 생성자 / Free 시 파괴자 호출 여부
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CMemoryPool(int blockNum, bool placementNew = false);
	virtual	~CMemoryPool();


	//////////////////////////////////////////////////////////////////////////
	// 블럭 하나를 할당받는다.  
	//
	// Parameters: 없음.
	// Return: (DATA *) 데이타 블럭 포인터.
	//////////////////////////////////////////////////////////////////////////
	DATA* Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// 사용중이던 블럭을 해제한다.
	//
	// Parameters: (DATA *) 블럭 포인터.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	void	Free(DATA* pData);


	//////////////////////////////////////////////////////////////////////////
	// 현재 확보 된 블럭 개수를 얻는다. (메모리풀 내부의 전체 개수)
	//
	// Parameters: 없음.
	// Return: (int) 메모리 풀 내부 전체 개수
	//////////////////////////////////////////////////////////////////////////
	int		GetCapacityCount(void) { return _capapcity; }

	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//
	// Parameters: 없음.
	// Return: (int) 사용중인 블럭 개수.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return _useCount; }

	void			Push(stBlock* newData);
	stBlock*		Pop();
	void			Resize();
};


template<class DATA>
inline CMemoryPool<DATA>::CMemoryPool(int blockNum, bool placementNew)
{
	_head = new char[sizeof(stBlock)];
	_tail = new char[sizeof(stBlock)];
	_head->next = _tail;
	_tail->prev = _head;
	_placementNew = placementNew;
	_capapcity = blockNum;

	//생성자 생성 
	if (_placementNew == false) {
		for (int i = 0; i < blockNum; i++) {
			stBlock* newNode = new stBlock;
			CMemoryPool::Push(newNode);
		}

	}
	else {
		for (int i = 0; i < blockNum; i++) {
			stBlock* newNode = new char[sizeof(stBlock)]; //생성자 호출 안함 
			CMemoryPool::Push(newNode);
		}
	}
}

template<class DATA>
inline CMemoryPool<DATA>::~CMemoryPool()
{
	
	//placementNew가 아닐 경우만 소멸자 호출
	if (_placementNew == true) {
	for (auto iter = _head; iter != _tail;) {
		
		delete iter;
		iter = iter->next;
		}
	}
}


template<class DATA>
inline DATA* CMemoryPool<DATA>::Alloc(void)
{
	//꽉 찬 경우 
	if (_capapcity == _useCount) {

		//재할당 하기!
		CMemoryPool::Resize();
	}
	_useCount++;
	DATA* newData = (DATA*)CMemoryPool::Pop();

	if (_placementNew == true) {
		new(newData)DATA();
		return newData;
	}
	else {

		return newData;
	}

}

template<class DATA>
inline void CMemoryPool<DATA>::Free(DATA* pData)
{

	CMemoryPool<DATA>::Push((stBlock*)pData);
	_useCount--;

}


template<class DATA>
inline void CMemoryPool<DATA>::Push(stBlock* newData)
{
	_tail->prev->next = newData;
	newData->prev = _tail->prev;
	newData->next = _tail;
	_tail->prev = newData;

}
template<class DATA>
typename CMemoryPool<DATA>::stBlock* CMemoryPool<DATA>::Pop() {
	stBlock* popData = _tail->prev;
	_tail->prev = _tail->prev->prev;
	_tail->prev->next = _tail;

	return popData;
}

template<class DATA>
inline void CMemoryPool<DATA>::Resize() {
	
	_capapcity = _capapcity+1;
	if (_placementNew == true) {
		
				stBlock* newNode = new char[sizeof(stBlock)]; //생성자 호출 안함 
				CMemoryPool::Push(newNode);
	}
	else {
			stBlock* newNode = new stBlock;
			CMemoryPool::Push(newNode);
	}

}
