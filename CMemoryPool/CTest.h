#pragma once
#include "CTemplateSingleton.h"
class CTest: public CTemplateSingleton<CTest>
{
	SINGLETON(CTest)
public:
	~CTest();
private:
	int		_a;
	int		_b;

};

