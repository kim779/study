// STD_SMA.cpp: implementation of the CSTD_SMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STD_SMA.h"
#include "indi_func.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSTD_SMA::CSTD_SMA()
{
	m_strName = "STD_SMA";
}

CSTD_SMA::~CSTD_SMA()
{

}
CString CSTD_SMA::GetInputs(long lType)
{
	return "Numeric,Numeric";
}


double CSTD_SMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue = _variant_t(varArg1);
	long lPeriod = 	_variant_t(varArg2);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesStd(pIController->IsUpdateData()
		,pIController->BarIndex(),lPeriod
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}