// ROC.cpp: implementation of the CROC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "ROC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CROC::CROC()
{
	m_strName = "ROC";
}

CROC::~CROC()
{

}

CString CROC::GetInputs(long lType)
{
//	return "Numeric,Numeric=close";
	return "Numeric,Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.18
 Modifier		: 
 Comments		: {{{ ROC = (당일종가 / n일전종가) * 100 }}}
-----------------------------------------------------------------------------*/
double CROC::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	double dValue = _variant_t(varArg2);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);	
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesROC(pIController->IsUpdateData()
		,pIController->BarIndex()
		, lPeriod
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}
