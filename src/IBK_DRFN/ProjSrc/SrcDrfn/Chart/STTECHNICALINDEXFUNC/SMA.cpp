// SMA.cpp: implementation of the CSMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SMA.h"
#include "indi_func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSMA::CSMA()
{
	m_strName = "SMA";
}

CSMA::~CSMA()
{

}


CString CSMA::GetInputs(long lType)
{
	return "Numeric,Numeric";
}


double CSMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue = _variant_t(varArg1);
	long lPeriod = 	_variant_t(varArg2);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,0);		
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesSMA(pIController->IsUpdateData()
		,pIController->BarIndex(),lPeriod
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}

