// SterrBnLower.cpp: implementation of the CSterrBnLower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "SterrBnLower.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSterrBnLower::CSterrBnLower()
{
	m_strName = "STERRBNLOWER";
}

CSterrBnLower::~CSterrBnLower()
{

}


CString CSterrBnLower::GetInputs(long lType)
{
//	return "Numeric,Numeric,Numeric=Close";
	return "Numeric,Numeric,Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.8.11
 Modifier		: 
 Comments		: {{{ Standard Error Bands Center = LRI}}}
-----------------------------------------------------------------------------*/
double CSterrBnLower::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);
	double dStdDev = _variant_t(varArg2);
	double dValue = _variant_t(varArg3);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries,dValue);		
		pIController->SetVariables(1,"OptVal1",NumericSeries,0);		
		pIController->SetVariables(2,"OptVal2",NumericSeries,0);		
		pIController->SetVariables(3,"OptVal3",NumericSeries,0);		
		pIController->SetVariables(4,"OptVal4",NumericSeries,0);		
		pIController->SetVariables(5,"OptVal5",NumericSeries,0);
		pIController->SetVariables(6,"OptVal6",NumericSeries,0);
		pIController->SetVariables(7,"OptVal7",NumericSeries,0);
		pIController->SetVariables(8,"OptVal8",NumericSeries,0);
		pIController->SetVariables(9,"OptVal9",NumericSeries,0);
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesSterrBnLower(pIController->IsUpdateData()
			,pIController->BarIndex()
			,lPeriod ,dStdDev
			,pIController->GetVarHolderDataItem(0)
			,pIController->GetVarHolderDataItem(1)
			,pIController->GetVarHolderDataItem(2)
			,pIController->GetVarHolderDataItem(3)
			,pIController->GetVarHolderDataItem(4)
			,pIController->GetVarHolderDataItem(5)
			,pIController->GetVarHolderDataItem(6)
			,pIController->GetVarHolderDataItem(7)
			,pIController->GetVarHolderDataItem(8)
			,pIController->GetVarHolderDataItem(9)
			,pIController->GetITempMemMng());
}
