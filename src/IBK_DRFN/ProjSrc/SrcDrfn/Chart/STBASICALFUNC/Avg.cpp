// Avg.cpp: implementation of the CAvg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Avg.h"
#include <math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAvg::CAvg()
{
	m_strName1 = "AVG";
	m_strName0 = "AVGLIST";
}

CAvg::~CAvg()
{

}

CString CAvg::GetName(long lType)
{
	switch(lType)
	{
	case YES_LANG: 
		return m_strName1; break;
	default:// TS_LANG
		return m_strName0;	break;
	}
}

CString CAvg::GetInputs(long lType)
{
	return "Numeric,Numeric,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL";
}

double CAvg::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue1 = _variant_t(varArg1);
	double dValue2 = _variant_t(varArg2);
	double dValue3 = _variant_t(varArg3);
	double dValue4 = _variant_t(varArg4);
	double dValue5 = _variant_t(varArg5);
	double dValue6 = _variant_t(varArg6);
	double dValue7 = _variant_t(varArg7);
	double dValue8 = _variant_t(varArg8);
	double dValue9 = _variant_t(varArg9);
	double dValue10 = _variant_t(varArg10);

	double dSum = 0;
	double dCount = 0;
	if(dValue1!=HUGE_VAL)	{	dSum += dValue1;	dCount += 1;	}
	if(dValue2!=HUGE_VAL)	{	dSum += dValue2;	dCount += 1;	}
	if(dValue3!=HUGE_VAL)	{	dSum += dValue3;	dCount += 1;	}
	if(dValue4!=HUGE_VAL)	{	dSum += dValue4;	dCount += 1;	}
	if(dValue5!=HUGE_VAL)	{	dSum += dValue5;	dCount += 1;	}
	if(dValue6!=HUGE_VAL)	{	dSum += dValue6;	dCount += 1;	}
	if(dValue7!=HUGE_VAL)	{	dSum += dValue7;	dCount += 1;	}
	if(dValue8!=HUGE_VAL)	{	dSum += dValue8;	dCount += 1;	}
	if(dValue9!=HUGE_VAL)	{	dSum += dValue9;	dCount += 1;	}
	if(dValue10!=HUGE_VAL)	{	dSum += dValue10;	dCount += 1;	}

	return (dSum/dCount);
}
