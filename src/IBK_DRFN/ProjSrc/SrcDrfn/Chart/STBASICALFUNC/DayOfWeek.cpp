// DayOfWeek.cpp: implementation of the CDayOfWeek class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "DayOfWeek.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDayOfWeek::CDayOfWeek()
{
	m_strName = "DAYOFWEEK";
}

CDayOfWeek::~CDayOfWeek()
{

}

CString CDayOfWeek::GetName(long lType)
{
	return m_strName;
}

CString CDayOfWeek::GetInputs(long lType)
{
	return "Numeric";
}

double CDayOfWeek::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lValue = _variant_t(varArg1);
	
	time_t liTime;
	char lsTmp[16];
	char lsYYYY[8];
	char lsMM[8];
	char lsDD[8];
	struct tm ltTm;
	struct tm *ltpTm;

	memset(lsYYYY, 0x00, sizeof(lsYYYY));	
	memset(lsMM, 0x00, sizeof(lsMM));	
	memset(lsDD, 0x00, sizeof(lsDD));
	memset(&ltTm, 0x00, sizeof(ltTm));

	sprintf(lsTmp, "%08d", lValue);

	memcpy(lsYYYY, lsTmp, 4);	
	memcpy(lsMM, lsTmp + 4, 2);	
	memcpy(lsDD, lsTmp + 6, 2);

	ltTm.tm_year = atoi(lsYYYY) - 1900;
	ltTm.tm_mon = atoi(lsMM) - 1;
	ltTm.tm_mday = atoi(lsDD);

	liTime = mktime(&ltTm);
	ltpTm = localtime(&liTime);

	return ltpTm->tm_wday;
}
