#pragma once
#include <new.h>


template<class DATA>
class CMemoryPool
{
private:
	struct stBlockNode {

#ifdef EXCEPTION 
		__int64				underflow; //define으로 막아주기
#endif 
		DATA				data;
		stBlockNode*		next;
#ifdef EXCEPTION
		__int64				overflow;
#endif 
	};

public:

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Parameters:	(int) 초기 블럭 개수.
	//				(bool) Alloc 시 생성자 / Free 시 파괴자 호출 여부
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CMemoryPool(int blockNum=0, bool placementNew = false);
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
	void	Free(DATA* data);


	//////////////////////////////////////////////////////////////////////////
	// 현재 확보 된 블럭 개수를 얻는다. (메모리풀 내부의 전체 개수)
	//
	// Parameters: 없음.
	// Return: (int) 메모리 풀 내부 전체 개수
	//////////////////////////////////////////////////////////////////////////
	int		GetCapacityCount(void) {
		return		_capacity;
	}

	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//
	// Parameters: 없음.
	// Return: (int) 사용중인 블럭 개수.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return _useCount; }

private:
	void				pop();
	void				push(stBlockNode* newData);
	void				resize();
private:
	int					_capacity;
	int					_useCount;
	bool				_placementNew;
	stBlockNode*		_head;
private:
	//static
	int			_count; //확인용 카운트 
};
//static ->inline으로 헤더에 선언해주던가 아님 밖으로 빼던가  
template<class DATA>
inline CMemoryPool<DATA>::CMemoryPool(int blockNum, bool placementNew)
{
#ifdef EXCEPTION 
	_count++;
#endif
	_head = reinterpret_cast<stBlockNode*>(new char[sizeof(stBlockNode)]);
	_head->next = nullptr;

	_capacity = blockNum;
	_placementNew = placementNew;

	//[]->[]->[] 처음 시작 생성자 호출하지 않기, alloc할 때 생성자 호출하기 
	if (placementNew == true) {
		for (int i = 0; i < blockNum; i++) {
			stBlockNode* newNode = reinterpret_cast <stBlockNode*>(new char[sizeof(stBlockNode)]);
#ifdef EXCEPTION
			newNode->underflow = reinterpret_cast<__int64>(this);
			newNode->overflow = reinterpret_cast<__int64>(this);
#endif
			newNode->next = nullptr;
			CMemoryPool::push(newNode);
		}
	}
	//placementNew 사용 안하는걸로 생성자 1회 호출 
	else {
		for (int i = 0; i < blockNum; i++) {
			stBlockNode* newNode = new stBlockNode;
#ifdef EXCEPTION
			newNode->underflow = reinterpret_cast<__int64>(this);
			newNode->overflow = reinterpret_cast<__int64>(this);
#endif
			newNode->next = nullptr;
			CMemoryPool::push(newNode);
		}
	}

}
template<class DATA>
inline CMemoryPool<DATA>::~CMemoryPool()
{

	//placementNew를 사용할때 메모리 해제할 때-> 
	if (_placementNew == false) {
		for (stBlockNode* iter = _head->next; iter != nullptr;) {
			stBlockNode* temp = iter->next;
			delete iter; //소멸자 호출 DATA->객체 기본 소멸자 호출 
			iter = temp;
		}
	}
	else {
		for (auto iter = _head->next; iter != nullptr;) {
			free(iter); //소멸자 호출 안됌 delete 소멸자 호출
			iter = iter->next;
		}
	}
	free(_head); //delete[] _head;
}


template<class DATA>
inline DATA* CMemoryPool<DATA>::Alloc(void)
{
	if (_capacity == _useCount) {
		//재할당 
		CMemoryPool::resize();
	}
#ifdef EXCEPTION
	reinterpret_cast<stBlockNode*>(_head->next)->underflow = reinterpret_cast<__int64>(this);
#endif
		//reinterpret_cast<stBlockNode*>(_head->next)->overflow;
	DATA* allocData = reinterpret_cast<DATA*>(reinterpret_cast<char*>(_head->next) + sizeof(__int64));
	CMemoryPool::pop();
	_useCount++;
	if (_placementNew == true) {
		DATA* ret = new(allocData)DATA();
		return ret;
	}

	return allocData;
}


template<class DATA>
inline void CMemoryPool<DATA>::Free(DATA* data)
{

	stBlockNode* freeData = reinterpret_cast<stBlockNode*>(reinterpret_cast<char*>(data) - sizeof(__int64));
#ifdef EXCEPTION
	if (freeData->underflow != reinterpret_cast<__int64>(this) || freeData->overflow != reinterpret_cast<__int64>(this)) {
		__debugbreak();
	}
#endif
	if (_placementNew == true) {
		data->~DATA();
	}
#ifdef EXCEPTION
	freeData->underflow = 1;
#endif
	_useCount--;
	CMemoryPool::push(freeData);

}

//HEAD->[]->[]->TAIL
template<class DATA>
inline void CMemoryPool<DATA>::pop()
{
	
	_head->next = _head->next->next;

}

template<class DATA>
inline void CMemoryPool<DATA>::push(stBlockNode* newData)
{
	newData->next = _head->next;
	_head->next = newData;
}

template<class DATA>
inline void CMemoryPool<DATA>::resize()
{
	_capacity = _capacity + 1;
	stBlockNode* newNode = new stBlockNode;
#ifndef  EXCEPTION
	newNode->overflow = reinterpret_cast<__int64>(this);
#endif
	newNode->next = nullptr;
	CMemoryPool::push(newNode);
}


