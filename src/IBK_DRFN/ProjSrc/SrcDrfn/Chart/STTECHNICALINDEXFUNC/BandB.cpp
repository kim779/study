// BandB.cpp: implementation of the CBandB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "BandB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBandB::CBandB()
{
	m_strName = "BANDB";
}

CBandB::~CBandB()
{

}

CString CBandB::GetInputs(long lType)
{
	return "Numeric,Numeric,Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.21
 Modifier		: 
 Comments		: {{{ Band%B = (종가 - 볼린저 하한선) / (볼린저 상한선 - 볼린저 하한선) * 100 }}}
-----------------------------------------------------------------------------*/
double CBandB::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	double dStdDev = _variant_t(varArg2);
	double dValue = _variant_t(varArg3);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);	
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesBandB(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,dStdDev
		,pIController->GetRawData(RAWCLOSEDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}