// Disparity_EMA.cpp: implementation of the CDisparity_EMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "TechnicalIndexFunc.h"
#include "Disparity_EMA.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDisparity_EMA::CDisparity_EMA()
{
	m_strName = "DISPARITY_EMA";
}

CDisparity_EMA::~CDisparity_EMA()
{

}

CString CDisparity_EMA::GetInputs(long lType)
{
//	return "Numeric, Numeric=close";
	return "Numeric, Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.18
 Modifier		: 
 Comments		: {{{ 이격도(지수) = 당일종가 / N일 지수이동평균 * 100 }}}
-----------------------------------------------------------------------------*/
double CDisparity_EMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	double dValue = _variant_t(varArg2);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries,dValue);
		pIController->SetVariables(1,"Opt1",NumericSeries,0);
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesDisparity_EMA(pIController->IsUpdateData()
		,pIController->BarIndex() ,lPeriod
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetITempMemMng());

}
