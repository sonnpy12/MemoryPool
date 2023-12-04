#pragma once
#define SINGLETON(T) friend CTemplateSingleton; \
private: T()=default; \

template<typename T>
class CTemplateSingleton
{
protected:
	CTemplateSingleton() {};
	virtual	~CTemplateSingleton() {};

public:
	static T* GetInstance() {
		if (_childInstance == nullptr) {
			_childInstance = new T;
		}
		return _childInstance;
	};
	static void DestroyInstance(){
		if (_childInstance) {
			delete _childInstance;
			_childInstance = nullptr;
		}
	};
private:
	static T*		_childInstance;
};
template <typename T>
T* CTemplateSingleton<T>::_childInstance = 0;
