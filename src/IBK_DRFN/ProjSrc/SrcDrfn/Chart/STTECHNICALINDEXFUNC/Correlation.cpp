// Correlation.cpp: implementation of the CCorrelation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "Correlation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCorrelation::CCorrelation()
{
	m_strName = "CORRELATION";
}

CCorrelation::~CCorrelation()
{

}

CString CCorrelation::GetInputs(long lType)
{
	return "Numeric,Numeric,Numeric";
}

double CCorrelation::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dIndVal = _variant_t(varArg1);
	double dDepVal = _variant_t(varArg2);
	long lPeriod =  _variant_t(varArg3);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"IndVal",NumericSeries, dIndVal);
		pIController->SetVariables(1,"DepVal ",NumericSeries, dDepVal);
	}
	pIController->SetVarHolderData(0,0,dIndVal);
	pIController->SetVarHolderData(1,0,dDepVal);

	return yesCorrelation(pIController->IsUpdateData()
		,pIController->BarIndex(),lPeriod
		,pIController->GetVarHolderDataItem(0),pIController->GetVarHolderDataItem(1)
		,pIController->GetITempMemMng());

}

