// LastCalMMTime.cpp: implementation of the CLastCalMMTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "LastCalMMTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLastCalMMTime::CLastCalMMTime()
{
	m_strName = "LASTCALMMTIME";
}

CLastCalMMTime::~CLastCalMMTime()
{

}


CString CLastCalMMTime::GetName(long lType)
{
	return m_strName;
}

CString CLastCalMMTime::GetInputs(long lType)
{
	return "";
}

double CLastCalMMTime::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{	
	time_t liTime;
	struct tm *ltpTm;

	liTime = time(NULL);
	ltpTm = localtime(&liTime);

	return ((ltpTm->tm_hour * 60) + ltpTm->tm_min);
}

