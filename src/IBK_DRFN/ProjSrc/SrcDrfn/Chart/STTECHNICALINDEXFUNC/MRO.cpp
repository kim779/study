// MRO.cpp: implementation of the CMRO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "MRO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMRO::CMRO()
{
	m_strName = "MRO";
}

CMRO::~CMRO()
{

}

CString CMRO::GetInputs(long lType)
{
	return "Numeric,Numeric,Numeric";
}


double CMRO::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dCondition = _variant_t(varArg1);
	long lPeriod = 	_variant_t(varArg2);
	long lOccur = 	_variant_t(varArg3);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dCondition);		
	}
	pIController->SetVarHolderData(0,0,dCondition);
	return yesMRO(pIController->IsUpdateData()
		,pIController->BarIndex(),lPeriod,lOccur
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}