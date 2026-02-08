// MI.cpp: implementation of the CMI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "MI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMI::CMI()
{
	m_strName = "MI";
}

CMI::~CMI()
{

}

CString CMI::GetInputs(long lType)
{
	return "Numeric";
}


double CMI::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"OptVal1",NumericSeries,0);		
		pIController->SetVariables(1,"OptVal2",NumericSeries,0);		
		pIController->SetVariables(2,"OptVal3",NumericSeries,0);		
		pIController->SetVariables(3,"OptVal4",NumericSeries,0);		
		pIController->SetVariables(4,"OptVal5",NumericSeries,0);		
	}
	return yesMI(pIController->IsUpdateData()
			,pIController->BarIndex()
			,lPeriod
			,pIController->GetRawData(RAWHIGHDATA)
			,pIController->GetRawData(RAWLOWDATA)
			,pIController->GetVarHolderDataItem(0)
			,pIController->GetVarHolderDataItem(1)
			,pIController->GetVarHolderDataItem(2)
			,pIController->GetVarHolderDataItem(3)
			,pIController->GetVarHolderDataItem(4)
			,pIController->GetITempMemMng());
}
