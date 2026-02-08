// Accum.cpp: implementation of the CAccum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Accum.h"
#include "indi_func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAccum::CAccum()
{
	m_strName = "ACCUM";
}

CAccum::~CAccum()
{

}

CString CAccum::GetInputs(long lType)
{
	return "Numeric";
}


double CAccum::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue = _variant_t(varArg1);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesAccum(pIController->IsUpdateData()
		,pIController->BarIndex(),pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}

