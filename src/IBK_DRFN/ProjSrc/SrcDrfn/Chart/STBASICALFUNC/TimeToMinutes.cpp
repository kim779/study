// TimeToMinutes.cpp: implementation of the CTimeToMinutes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "TimeToMinutes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeToMinutes::CTimeToMinutes()
{
	m_strName = "TIMETOMINUTES";
}

CTimeToMinutes::~CTimeToMinutes()
{

}

CString CTimeToMinutes::GetName(long lType)
{
	return m_strName;
}

CString CTimeToMinutes::GetInputs(long lType)
{
	return "Numeric";
}

double CTimeToMinutes::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lValue = _variant_t(varArg1);
	
	int liHour;
	int liMin;
	char lsTmp[16];
	char lsHour[8];
	char lsMin[8];

	memset(lsHour, 0x00, sizeof(lsHour));
	memset(lsMin, 0x00, sizeof(lsMin));

	sprintf(lsTmp, "%06d", lValue);

	memcpy(lsHour, lsTmp, 2);
	memcpy(lsMin, lsTmp + 2, 2);

	liHour = atoi(lsHour);
	liMin = atoi(lsMin);

	return ((liHour * 60) + liMin);
}

