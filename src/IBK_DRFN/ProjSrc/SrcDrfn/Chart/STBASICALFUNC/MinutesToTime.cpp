// MinutesToTime.cpp: implementation of the CMinutesToTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "MinutesToTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMinutesToTime::CMinutesToTime()
{

}

CMinutesToTime::~CMinutesToTime()
{

}

CString CMinutesToTime::GetName(long lType)
{
	return m_strName;
}

CString CMinutesToTime::GetInputs(long lType)
{
	return "Numeric";
}

double CMinutesToTime::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lValue = _variant_t(varArg1);
	
	int liHour;
	int liMin;

	liHour = lValue / 60;
	liMin = lValue % 60;

	return ((liHour * 10000) + (liMin * 100));

}

