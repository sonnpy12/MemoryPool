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
    //if 파일 오픈 실패시 
    //fread_s(&buf,sizeof(buf),)
    //fclose 함수 

    //만약 textParser 함수 호출하기 ->파싱은 완료하기 

    
    return false;
}

void CTextParser::TextParser()
{
    //if(FIND SERVER()==false){COUT 출력하기} 

   

    
}

bool CTextParser::FindServer()
{

    //_serverName 세팅
    //찾았으면 return true 못찾았으면 bool
    return false;
}
