#pragma once
#include <new.h>


template<class DATA>
class CMemoryPool
{
private:
	struct stBlockNode {

#ifdef EXCEPTION 
		__int64				underflow; //define���� �����ֱ�
#endif 
		DATA				data;
		stBlockNode*		next;
#ifdef EXCEPTION
		__int64				overflow;
#endif 
	};

public:

	//////////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Parameters:	(int) �ʱ� �� ����.
	//				(bool) Alloc �� ������ / Free �� �ı��� ȣ�� ����
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CMemoryPool(int blockNum=0, bool placementNew = false);
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
	void	Free(DATA* data);


	//////////////////////////////////////////////////////////////////////////
	// ���� Ȯ�� �� �� ������ ��´�. (�޸�Ǯ ������ ��ü ����)
	//
	// Parameters: ����.
	// Return: (int) �޸� Ǯ ���� ��ü ����
	//////////////////////////////////////////////////////////////////////////
	int		GetCapacityCount(void) {
		return		_capacity;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� ������� �� ������ ��´�.
	//
	// Parameters: ����.
	// Return: (int) ������� �� ����.
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
	int			_count; //Ȯ�ο� ī��Ʈ 
};
//static ->inline���� ����� �������ִ��� �ƴ� ������ ������  
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

	//[]->[]->[] ó�� ���� ������ ȣ������ �ʱ�, alloc�� �� ������ ȣ���ϱ� 
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
	//placementNew ��� ���ϴ°ɷ� ������ 1ȸ ȣ�� 
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

	//placementNew�� ����Ҷ� �޸� ������ ��-> 
	if (_placementNew == false) {
		for (stBlockNode* iter = _head->next; iter != nullptr;) {
			stBlockNode* temp = iter->next;
			delete iter; //�Ҹ��� ȣ�� DATA->��ü �⺻ �Ҹ��� ȣ�� 
			iter = temp;
		}
	}
	else {
		for (auto iter = _head->next; iter != nullptr;) {
			free(iter); //�Ҹ��� ȣ�� �ȉ� delete �Ҹ��� ȣ��
			iter = iter->next;
		}
	}
	free(_head); //delete[] _head;
}


template<class DATA>
inline DATA* CMemoryPool<DATA>::Alloc(void)
{
	if (_capacity == _useCount) {
		//���Ҵ� 
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


