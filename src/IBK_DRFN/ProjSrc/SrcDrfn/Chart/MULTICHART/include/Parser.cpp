// Parser.cpp: implementation of the CParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Parser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParser::CParser()
{

}

CParser::~CParser()
{

}

CString CParser::GetString(CString& szString, TCHAR deli)
{
	if(szString.GetLength()==0)	return "";
		
	CString szRet;
	int nIndex = szString.Find(deli);
	if(nIndex<0)		// 구분자가 없는 경우 인자로 들어온 스트링을 넘긴다. 
	{
		szString.TrimLeft();		szString.TrimRight();
		szRet = szString;
		szString = "";
	}
	else
	{
		szRet = szString.Left(nIndex);
		szString = szString.Mid(nIndex+1);
		szRet.TrimLeft();		szRet.TrimRight();
		szString.TrimLeft();	szString.TrimRight();
	}

	return szRet;
}