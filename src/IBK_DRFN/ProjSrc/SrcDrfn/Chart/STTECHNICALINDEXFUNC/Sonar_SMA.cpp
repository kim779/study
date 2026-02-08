// Sonar_SMA.cpp: implementation of the CSonar_SMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sonar_SMA.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSonar_SMA::CSonar_SMA()
{
	m_strName = "SONAR_SMA";
}

CSonar_SMA::~CSonar_SMA()
{

}
CString CSonar_SMA::GetInputs(long lType)
{
//	return "Numeric, Numeric=close";
	return "Numeric, Numeric, Numeric";
}

double CSonar_SMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	long mPeriod = _variant_t(varArg2);
	double dValue = _variant_t(varArg3);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);
		pIController->SetVariables(1,"OptVal1",NumericSeries,0);		
		pIController->SetVariables(2,"OptVal2",NumericSeries,0);		
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesSONAR(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,mPeriod
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetITempMemMng());

}