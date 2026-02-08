// MFI.cpp: implementation of the CMFI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "MFI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMFI::CMFI()
{
	m_strName = "MFI";
}

CMFI::~CMFI()
{

}

CString CMFI::GetInputs(long lType)
{
//	return "Numeric,Numeric=(Open+High+Low)/3";
	return "Numeric,Numeric";
}


double CMFI::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);
	double dValue = _variant_t(varArg2);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"SrcVal1",NumericSeries,dValue);		
		pIController->SetVariables(1,"OptVal1",NumericSeries,0);		
		pIController->SetVariables(2,"OptVal2",NumericSeries,0);		
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesMFI(pIController->IsUpdateData()
			,pIController->BarIndex()
			,lPeriod
			,pIController->GetRawData(RAWVOLUMEDATA)
			,pIController->GetVarHolderDataItem(0)
			,pIController->GetVarHolderDataItem(1)
			,pIController->GetVarHolderDataItem(2)
			,pIController->GetITempMemMng());
}

