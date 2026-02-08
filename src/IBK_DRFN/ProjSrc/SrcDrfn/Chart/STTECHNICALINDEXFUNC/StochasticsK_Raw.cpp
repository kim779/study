// StochasticsK_Raw.cpp: implementation of the CStochasticsK_Raw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StochasticsK_Raw.h"
#include "indi_func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStochasticsK_Raw::CStochasticsK_Raw()
{

	m_strName = "STOCHASTICSK_RAW";
}

CStochasticsK_Raw::~CStochasticsK_Raw()
{

}

CString CStochasticsK_Raw::GetInputs(long lType)
{
	return "Numeric,Numeric,Numeric";
}


double CStochasticsK_Raw::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod1 = 	_variant_t(varArg1);
	long lPeriod2 = 	_variant_t(varArg2);
	double dValue = _variant_t(varArg3);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,0);
		pIController->SetVariables(1,"OptValue1",NumericSeries,0);
		pIController->SetVariables(2,"OptValue2",NumericSeries,0);
		pIController->SetVariables(3,"OptValue3",NumericSeries,0);
		pIController->SetVariables(4,"OptValue4",NumericSeries,0);
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesStochasticsK(pIController->IsUpdateData()
		,pIController->BarIndex()
		, lPeriod1, lPeriod2
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetVarHolderDataItem(3)
		,pIController->GetVarHolderDataItem(4)
		,pIController->GetITempMemMng());
}

