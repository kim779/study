// DiMinus.cpp: implementation of the CDiMinus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "DiMinus.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDiMinus::CDiMinus()
{
	m_strName = "DIMINUS";
}

CDiMinus::~CDiMinus()
{

}

CString CDiMinus::GetInputs(long lType)
{
	return "Numeric";
}

double CDiMinus::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"OptVar1",NumericSeries,0);
		pIController->SetVariables(1,"OptVar2",NumericSeries,0);
	}
	return yesDiMinus(pIController->IsUpdateData()
				,pIController->BarIndex()
				,lPeriod
				,pIController->GetRawData(RAWHIGHDATA)
				,pIController->GetRawData(RAWLOWDATA)
				,pIController->GetRawData(RAWCLOSEDATA)
				,pIController->GetVarHolderDataItem(0)
				,pIController->GetVarHolderDataItem(1)
				,pIController->GetITempMemMng());
}


