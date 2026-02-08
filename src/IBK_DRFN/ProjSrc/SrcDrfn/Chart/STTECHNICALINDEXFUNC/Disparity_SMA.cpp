// Disparity_SMA.cpp: implementation of the CDisparity_SMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Disparity_SMA.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDisparity_SMA::CDisparity_SMA()
{
	m_strName = "DISPARITY_SMA";
}

CDisparity_SMA::~CDisparity_SMA()
{

}


CString CDisparity_SMA::GetInputs(long lType)
{
	return "Numeric, Numeric";
}

double CDisparity_SMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	double dValue = _variant_t(varArg2);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries,dValue);
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesDisparity(pIController->IsUpdateData()
		,pIController->BarIndex() ,lPeriod
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}

