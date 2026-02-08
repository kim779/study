// WILLR.cpp: implementation of the CWILLR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "WILLR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWILLR::CWILLR()
{
	m_strName = "WILLR";
}

CWILLR::~CWILLR()
{

}

CString CWILLR::GetInputs(long lType)
{
//	return "Numeric,Numeric=close";
	return "Numeric,Numeric";
}

double CWILLR::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);
	double dValue = _variant_t(varArg2);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);	
/*		pIController->SetVariables(1,"OptValue",NumericSeries,0);		*/
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesWILLR(pIController->IsUpdateData()
		,pIController->BarIndex()
		, lPeriod
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}

