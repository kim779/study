// TRIX.cpp: implementation of the CTRIX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "TRIX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTRIX::CTRIX()
{
	m_strName = "TRIX";
}

CTRIX::~CTRIX()
{

}

CString CTRIX::GetInputs(long lType)
{
//	return "Numeric,Numeric=close";
	return "Numeric,Numeric";
}

double CTRIX::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);
	double dValue = _variant_t(varArg2);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);	
		pIController->SetVariables(1,"OptVar1",NumericSeries,0);		
		pIController->SetVariables(2,"OptVar2",NumericSeries,0);		
		pIController->SetVariables(3,"OptVar3",NumericSeries,0);		
		pIController->SetVariables(4,"OptVar4",NumericSeries,0);		
		pIController->SetVariables(5,"OptVar5",NumericSeries,0);		
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesTRIX(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetVarHolderDataItem(3)
		,pIController->GetVarHolderDataItem(4)
		,pIController->GetVarHolderDataItem(5)
		,pIController->GetITempMemMng());
}
