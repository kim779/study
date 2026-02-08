// CrossDown.cpp: implementation of the CCrossDown class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "CrossDown.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrossDown::CCrossDown()
{
	m_strName0 = "CROSSBELOW";
	m_strName1 = "CROSSDOWN";
}

CCrossDown::~CCrossDown()
{

}

CString CCrossDown::GetName(long lType) 
{
	switch(lType)
	{
	case 0:
		return m_strName0;
	case 1:
		return m_strName1;
	}
	return "";
}

CString CCrossDown::GetInputs(long lType)
{
	return "Numeric,Numeric";
}

double CCrossDown::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dSrcValue = _variant_t(varArg1);
	double dBasis = _variant_t(varArg2);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Src",NumericSeries, dSrcValue);
		pIController->SetVariables(1,"Basis",NumericSeries, dBasis);
	}
	pIController->SetVarHolderData(0,0,dSrcValue);
	pIController->SetVarHolderData(1,0,dBasis);	
	return yesCrossDown(pIController->IsUpdateData()
		,pIController->BarIndex(),pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1),pIController->GetITempMemMng());

}

