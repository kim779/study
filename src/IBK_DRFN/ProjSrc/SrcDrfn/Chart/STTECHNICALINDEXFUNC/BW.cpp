// BW.cpp: implementation of the CBW class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "BW.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBW::CBW()
{
	m_strName = "BW";
}

CBW::~CBW()
{

}

CString CBW::GetInputs(long lType)
{
	return "Numeric,Numeric,Numeric,Numeric";
}

double CBW::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lMaPeriod = _variant_t(varArg1);
	long lRocPeriod = _variant_t(varArg2);
	long lStoPeriod1 = _variant_t(varArg3);
	long lStoPeriod2 = _variant_t(varArg4);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"optvar1",NumericSeries,0);
		pIController->SetVariables(1,"optvar2",NumericSeries,0);
		pIController->SetVariables(2,"optvar3",NumericSeries,0);
		pIController->SetVariables(3,"optvar4",NumericSeries,0);
		pIController->SetVariables(4,"optvar5",NumericSeries,0);
		pIController->SetVariables(5,"optvar6",NumericSeries,0);
	}	
	return yesBW(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lMaPeriod, lRocPeriod, lStoPeriod1, lStoPeriod2
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)		
		,pIController->GetRawData(RAWCLOSEDATA)		
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetVarHolderDataItem(3)
		,pIController->GetVarHolderDataItem(4)
		,pIController->GetVarHolderDataItem(5)
		,pIController->GetITempMemMng());
}
