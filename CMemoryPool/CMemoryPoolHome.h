#pragma once
#include"stdafx.h"
//�޸� Ǯ Ŭ����(������Ʈ Ǯ / ��������Ʈ)
//Ư�� ����Ÿ(����ü, Ŭ����, ����)�� ������ �Ҵ� �� ��������.
//
//- ����.
//
//procademy::CMemoryPool<DATA> MemPool(300, FALSE);
//DATA* pData = MemPool.Alloc();
//
//pData ���
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
	// ������, �ı���.
	//
	// Parameters:	(int) �ʱ� �� ����.
	//				(bool) Alloc �� ������ / Free �� �ı��� ȣ�� ����
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CMemoryPool(int blockNum, bool placementNew = false);
	virtual	~CMemoryPool();


	//////////////////////////////////////////////////////////////////////////
	// �� �ϳ��� �Ҵ�޴´�.  
	//
	// Parameters: ����.
	// Return: (DATA *) ����Ÿ �� ������.
	//////////////////////////////////////////////////////////////////////////
	DATA* Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// ������̴� ���� �����Ѵ�.
	//
	// Parameters: (DATA *) �� ������.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	void	Free(DATA* pData);


	//////////////////////////////////////////////////////////////////////////
	// ���� Ȯ�� �� �� ������ ��´�. (�޸�Ǯ ������ ��ü ����)
	//
	// Parameters: ����.
	// Return: (int) �޸� Ǯ ���� ��ü ����
	//////////////////////////////////////////////////////////////////////////
	int		GetCapacityCount(void) { return _capapcity; }

	//////////////////////////////////////////////////////////////////////////
	// ���� ������� �� ������ ��´�.
	//
	// Parameters: ����.
	// Return: (int) ������� �� ����.
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

	//������ ���� 
	if (_placementNew == false) {
		for (int i = 0; i < blockNum; i++) {
			stBlock* newNode = new stBlock;
			CMemoryPool::Push(newNode);
		}

	}
	else {
		for (int i = 0; i < blockNum; i++) {
			stBlock* newNode = new char[sizeof(stBlock)]; //������ ȣ�� ���� 
			CMemoryPool::Push(newNode);
		}
	}
}

template<class DATA>
inline CMemoryPool<DATA>::~CMemoryPool()
{
	
	//placementNew�� �ƴ� ��츸 �Ҹ��� ȣ��
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
	//�� �� ��� 
	if (_capapcity == _useCount) {

		//���Ҵ� �ϱ�!
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
		
				stBlock* newNode = new char[sizeof(stBlock)]; //������ ȣ�� ���� 
				CMemoryPool::Push(newNode);
	}
	else {
			stBlock* newNode = new stBlock;
			CMemoryPool::Push(newNode);
	}

}
