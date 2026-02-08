// Qstick.cpp: implementation of the CQstick class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "Qstick.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQstick::CQstick()
{
	m_strName = "QSTIC";
}

CQstick::~CQstick()
{

}


CString CQstick::GetInputs(long lType)
{
	return "Numeric";
}


/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.22
 Modifier		: 
 Comments		: 
-----------------------------------------------------------------------------*/
double CQstick::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod1 = _variant_t(varArg1);
	
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,0);
	}
	return yesQstic(pIController->IsUpdateData()
		,pIController->BarIndex() ,lPeriod1 
		,pIController->GetRawData(RAWOPENDATA)	
		,pIController->GetRawData(RAWCLOSEDATA)	
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}

