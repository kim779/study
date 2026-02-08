// Max.cpp: implementation of the CMax class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Max.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMax::CMax()
{
	m_strName0 = "MAXLIST";
	m_strName1 = "MAX";
}

CMax::~CMax()
{

}

CString CMax::GetName(long lType)
{
	switch(lType)
	{
	case YES_LANG: 
		return m_strName1; break;
	default:// TS_LANG
		return m_strName0;	break;
	}
}

CString CMax::GetInputs(long lType)
{
	return "Numeric,Numeric,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL,Numeric=HUGE_VAL";
}

double CMax::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
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
	
	double dResult = dValue1;
	if(dResult<dValue2)		dResult = dValue2;
	if(dValue3 != HUGE_VAL && dResult<dValue3)		dResult = dValue3;
	if(dValue4 != HUGE_VAL && dResult<dValue4)		dResult = dValue4;
	if(dValue5 != HUGE_VAL && dResult<dValue5)		dResult = dValue5;
	if(dValue6 != HUGE_VAL && dResult<dValue6)		dResult = dValue6;
	if(dValue7 != HUGE_VAL && dResult<dValue7)		dResult = dValue7;
	if(dValue8 != HUGE_VAL && dResult<dValue8)		dResult = dValue8;
	if(dValue9 != HUGE_VAL && dResult<dValue9)		dResult = dValue9;
	if(dValue10 != HUGE_VAL && dResult<dValue10)	dResult = dValue10;
	return dResult;
}
