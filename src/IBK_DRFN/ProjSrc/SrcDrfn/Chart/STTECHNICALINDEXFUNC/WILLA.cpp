// WILLA.cpp: implementation of the CWILLA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "WILLA.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWILLA::CWILLA()
{
	m_strName = "WILLA";
}

CWILLA::~CWILLA()
{

}


CString CWILLA::GetInputs(long lType)
{
//	return "Numeric=close";
	return "Numeric";
}

double CWILLA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue =		_variant_t(varArg1);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries,dValue);		
		pIController->SetVariables(1,"OptValue",NumericSeries,0);		
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesWILLA(pIController->IsUpdateData()
		,pIController->BarIndex()
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetITempMemMng());
}