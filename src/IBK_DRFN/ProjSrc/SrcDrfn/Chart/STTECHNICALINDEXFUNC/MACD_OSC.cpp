// MACD_OSC.cpp: implementation of the CMACD_OSC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "MACD_OSC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMACD_OSC::CMACD_OSC()
{
	m_strName = "MACD_OSC";
}

CMACD_OSC::~CMACD_OSC()
{

}

CString CMACD_OSC::GetInputs(long lType)
{
	return "Numeric,Numeric,Numeric";
}


double CMACD_OSC::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lShortPeriod = _variant_t(varArg1);
	long lLongPeriod = 	_variant_t(varArg2);
	long lPeriod = 	_variant_t(varArg3);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"OptVal1",NumericSeries,0);		
		pIController->SetVariables(1,"OptVal2",NumericSeries,0);		
		pIController->SetVariables(2,"OptVal3",NumericSeries,0);		
		pIController->SetVariables(3,"OptVal4",NumericSeries,0);		
	}
	return yesMACD_OSC(pIController->IsUpdateData()
			,pIController->BarIndex()
			,lShortPeriod, lLongPeriod, lPeriod
			,pIController->GetRawData(RAWCLOSEDATA)
			,pIController->GetVarHolderDataItem(0)
			,pIController->GetVarHolderDataItem(1)
			,pIController->GetVarHolderDataItem(2)
			,pIController->GetVarHolderDataItem(3)
			,pIController->GetITempMemMng());
}
