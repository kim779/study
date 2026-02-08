// MACD_SMA.cpp: implementation of the CMACD_SMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MACD_SMA.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMACD_SMA::CMACD_SMA()
{
	m_strName = "MACD_SMA";

}

CMACD_SMA::~CMACD_SMA()
{

}
CString CMACD_SMA::GetInputs(long lType)
{
	return "Numeric,Numeric,Numeric";
}


double CMACD_SMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lShortPeriod = _variant_t(varArg1);
	long lLongPeriod = 	_variant_t(varArg2);
	double dValue = _variant_t(varArg3);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);	
		pIController->SetVariables(1,"OPt1",NumericSeries,dValue);	
		pIController->SetVariables(2,"OPt2",NumericSeries,dValue);	
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesMACD(pIController->IsUpdateData()
			,pIController->BarIndex()
			,lShortPeriod, lLongPeriod
			,pIController->GetVarHolderDataItem(0)
			,pIController->GetVarHolderDataItem(1)
			,pIController->GetVarHolderDataItem(2)
			,pIController->GetITempMemMng());
}