// PsyhologicalLine.cpp: implementation of the CPsyhologicalLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "PsyhologicalLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPsyhologicalLine::CPsyhologicalLine()
{
	m_strName = "PSYHOLOGICALLINE";
}

CPsyhologicalLine::~CPsyhologicalLine()
{

}

CString CPsyhologicalLine::GetInputs(long lType)
{
//	return "Numeric, Numeric=close";
	return "Numeric, Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.20
 Modifier		: 
 Comments		: {{{ 심리도 = (n일 중 상승일 수 / n) * 100 }}}
-----------------------------------------------------------------------------*/
double CPsyhologicalLine::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	double dValue = _variant_t(varArg2);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries,dValue);
		pIController->SetVariables(1,"Value2",NumericSeries,0);		
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesPsyhologicalLine(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetITempMemMng());

}
