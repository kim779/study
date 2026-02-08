// StochasticsD_Raw.cpp: implementation of the CStochasticsD_Raw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StochasticsD_Raw.h"
#include "indi_func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStochasticsD_Raw::CStochasticsD_Raw()
{
	m_strName = "STOCHASTICSD_RAW";
}

CStochasticsD_Raw::~CStochasticsD_Raw()
{

}

CString CStochasticsD_Raw::GetInputs(long lType)
{
//	return "Numeric,Numeric,Numeric,Numeric=close";
	return "Numeric,Numeric,Numeric,Numeric";
}


double CStochasticsD_Raw::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod1 = 	_variant_t(varArg1);
	long lPeriod2 = 	_variant_t(varArg2);
	long lPeriod3 = 	_variant_t(varArg3);
	double dValue = _variant_t(varArg4);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,0);
		pIController->SetVariables(1,"OptValue1",NumericSeries,0);
		pIController->SetVariables(2,"OptValue2",NumericSeries,0);
		pIController->SetVariables(3,"OptValue3",NumericSeries,0);
		pIController->SetVariables(4,"OptValue4",NumericSeries,0);
		pIController->SetVariables(5,"OptValue5",NumericSeries,0);
		pIController->SetVariables(6,"OptValue6",NumericSeries,0);
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesStochasticsD(pIController->IsUpdateData()
		,pIController->BarIndex()
		, lPeriod1, lPeriod2, lPeriod3
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetVarHolderDataItem(3)
		,pIController->GetVarHolderDataItem(4)
		,pIController->GetVarHolderDataItem(5)
		,pIController->GetVarHolderDataItem(6)
		,pIController->GetITempMemMng());
}


