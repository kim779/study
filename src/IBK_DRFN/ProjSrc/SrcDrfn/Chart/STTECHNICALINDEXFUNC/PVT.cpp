// PVT.cpp: implementation of the CPVT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "PVT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPVT::CPVT()
{
	m_strName = "PVT";
}

CPVT::~CPVT()
{

}

CString CPVT::GetInputs(long lType)
{
//	return "Numeric=close";
	return "Numeric";
}

double CPVT::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue = _variant_t(varArg1);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,0);
		pIController->SetVariables(1,"OptVal1",NumericSeries,0);		
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesPVT(pIController->IsUpdateData()
		,pIController->BarIndex()
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetRawData(RAWVOLUMEDATA)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetITempMemMng());

}
