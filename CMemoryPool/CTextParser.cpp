#include "stdafx.h"
#include "CTextParser.h"
#include "CMalloc.h"
bool CTextParser::LoadFile()
{
    FILE* parserFile;
    CMalloc parser;
    _buf = (char*)parser.newMalloc(1000);

    fopen_s(&parserFile,_buf, "r"); 
    if (parserFile == nullptr) {
        cout << "parserFile open";
        __debugbreak();
    }
    //if ���� ���� ���н� 
    //fread_s(&buf,sizeof(buf),)
    //fclose �Լ� 

    //���� textParser �Լ� ȣ���ϱ� ->�Ľ��� �Ϸ��ϱ� 

    
    return false;
}

void CTextParser::TextParser()
{
    //if(FIND SERVER()==false){COUT ����ϱ�} 

   

    
}

bool CTextParser::FindServer()
{

    //_serverName ����
    //ã������ return true ��ã������ bool
    return false;
}
