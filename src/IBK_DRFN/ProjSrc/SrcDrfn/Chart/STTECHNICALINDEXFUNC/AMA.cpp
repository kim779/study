// AMA.cpp: implementation of the CAMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AMA.h"
#include "indi_func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAMA::CAMA()
{
	m_strName = "AMA";
}

CAMA::~CAMA()
{

}


CString CAMA::GetInputs(long lType)
{
	return "Numeric,Numeric";
}


double CAMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue = _variant_t(varArg1);
	long lPeriod = 	_variant_t(varArg2);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,0);		
		pIController->SetVariables(1,"Opt",NumericSeries,0);		
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesAMA(pIController->IsUpdateData()
		,pIController->BarIndex(),lPeriod
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetITempMemMng());
}

