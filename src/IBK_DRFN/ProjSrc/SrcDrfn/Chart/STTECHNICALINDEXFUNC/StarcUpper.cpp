// StarcUpper.cpp: implementation of the CStarcUpper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "StarcUpper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStarcUpper::CStarcUpper()
{
	m_strName = "STARCUPPER";
}

CStarcUpper::~CStarcUpper()
{

}

CString CStarcUpper::GetInputs(long lType)
{
//	return "Numeric, Numeric, Numeric=close";
	return "Numeric, Numeric, Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.19
 Modifier		: 
 Comments		: {{{ starc upper line = center line + 2*ATR }}}
-----------------------------------------------------------------------------*/
double CStarcUpper::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	long lPeriod1 = _variant_t(varArg2);
	double dValue = _variant_t(varArg3);
	
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries,dValue);		
		pIController->SetVariables(1,"Value2",NumericSeries,0);	
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesStarcUpper(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod ,lPeriod1
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetITempMemMng());

}
