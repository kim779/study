// ATR.cpp: implementation of the CATR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "ATR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CATR::CATR()
{
	m_strName = "ATR";
}

CATR::~CATR()
{

}

CString CATR::GetInputs(long lType)
{
	return "Numeric";
}

double CATR::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"optvar",NumericSeries,0);
	}
	return yesATR(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetRawData(RAWCLOSEDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}
