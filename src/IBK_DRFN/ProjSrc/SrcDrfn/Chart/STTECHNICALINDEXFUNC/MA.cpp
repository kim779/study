// MA.cpp: implementation of the CMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MA.h"
#include "indi_func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMA::CMA()
{
	m_strName = "MA";
}

CMA::~CMA()
{

}

CString CMA::GetInputs(long lType)
{
//	return "Numeric,Numeric,Numeric=1";
	return "Numeric,Numeric,Numeric";
}


double CMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue = _variant_t(varArg1);
	long lPeriod = 	_variant_t(varArg2);
	long lMAType = 	_variant_t(varArg3);	// 1:SMA, 2:EMA, 3:WMA, 4:AMA

	switch(lMAType)
	{
	case 2:
		if(pIController->IsFirstInThisMem())	// init
		{
			pIController->SetVariables(0,"Value",NumericSeries,0);		
			pIController->SetVariables(1,"Opt1",NumericSeries,0);		
		}
		pIController->SetVarHolderData(0,0,dValue);
		return yesEMA(pIController->IsUpdateData()
			,pIController->BarIndex()
			,lPeriod
			,pIController->GetVarHolderDataItem(0)
			,pIController->GetVarHolderDataItem(1)
			,pIController->GetITempMemMng());
	case 3:
		if(pIController->IsFirstInThisMem())	// init
		{
			pIController->SetVariables(0,"Value",NumericSeries,0);		
		}
		pIController->SetVarHolderData(0,0,dValue);
		return yesWMA(pIController->IsUpdateData()
			,pIController->BarIndex()
			,lPeriod
			,pIController->GetVarHolderDataItem(0)
			,pIController->GetITempMemMng());
	case 4:
		if(pIController->IsFirstInThisMem())	// init
		{
			pIController->SetVariables(0,"Value",NumericSeries,0);		
			pIController->SetVariables(1,"Opt1",NumericSeries,0);		
		}
		pIController->SetVarHolderData(0,0,dValue);
		return yesAMA(pIController->IsUpdateData()
			,pIController->BarIndex()
			,lPeriod
			,pIController->GetVarHolderDataItem(0)
			,pIController->GetVarHolderDataItem(1)
			,pIController->GetITempMemMng());
	default://1
		if(pIController->IsFirstInThisMem())	// init
		{
			pIController->SetVariables(0,"Value",NumericSeries,0);		
		}
		pIController->SetVarHolderData(0,0,dValue);
		return yesSMA(pIController->IsUpdateData()
			,pIController->BarIndex()
			,lPeriod
			,pIController->GetVarHolderDataItem(0)
			,pIController->GetITempMemMng());
	}
	return 0;
}

