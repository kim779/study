// LRL.cpp: implementation of the CLRL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "LRL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLRL::CLRL()
{
	m_strName = "LRL";
}

CLRL::~CLRL()
{

}


CString CLRL::GetInputs(long lType)
{
	return "Numeric,Numeric";
}


double CLRL::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	double dValue = _variant_t(varArg2);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);		
		pIController->SetVariables(1,"OptVal1",NumericSeries,0);		
		pIController->SetVariables(2,"OptVal2",NumericSeries,0);		
		pIController->SetVariables(3,"OptVal3",NumericSeries,0);		
		pIController->SetVariables(4,"OptVal4",NumericSeries,0);		
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesLRL(pIController->IsUpdateData()
			,pIController->BarIndex()
			,lPeriod
			,pIController->GetVarHolderDataItem(0)
			,pIController->GetVarHolderDataItem(1)
			,pIController->GetVarHolderDataItem(2)
			,pIController->GetVarHolderDataItem(3)
			,pIController->GetVarHolderDataItem(4)
			,pIController->GetITempMemMng());
}


