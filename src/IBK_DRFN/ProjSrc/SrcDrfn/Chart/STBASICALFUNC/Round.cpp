// Round.cpp: implementation of the CRound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Round.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRound::CRound()
{
	m_strName = "ROUND";
}

CRound::~CRound()
{

}

CString CRound::GetInputs(long lType)
{
	return "Numeric,Numeric";
}

double CRound::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue1 = _variant_t(varArg1);
	double dValue2 = _variant_t(varArg2);

	double dResult = 0;
	double dInteger = 0;
	double dFracti = 0;
	dResult = dValue1 * pow((double)10.,(double)dValue2);
	dFracti = modf(dResult,&dInteger);
	if(dFracti>=0.5)		dInteger += 1;
	dResult = dInteger * pow((double)10.,(double)(dValue2*-1));	
	return dResult;
}


