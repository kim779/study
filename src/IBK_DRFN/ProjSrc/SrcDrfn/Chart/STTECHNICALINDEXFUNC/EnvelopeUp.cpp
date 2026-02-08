// EnvelopeUp.cpp: implementation of the CEnvelopeUp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "EnvelopeUp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnvelopeUp::CEnvelopeUp()
{
	m_strName = "ENVELOPEUP";
}

CEnvelopeUp::~CEnvelopeUp()
{

}

CString CEnvelopeUp::GetInputs(long lType)
{
//	return "Numeric,Numeric,Numeric=Close";
	return "Numeric,Numeric,Numeric";
}


double CEnvelopeUp::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	long lPercent = _variant_t(varArg2);
	double dValue = _variant_t(varArg3);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries,dValue);
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesEnvelopeUp(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod, lPercent
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}
