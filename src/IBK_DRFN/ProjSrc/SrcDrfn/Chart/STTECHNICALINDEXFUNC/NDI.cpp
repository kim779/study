// NDI.cpp: implementation of the CNDI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "NDI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNDI::CNDI()
{
	m_strName = "NDI";
}

CNDI::~CNDI()
{

}


CString CNDI::GetInputs(long lType)
{
	return "Numeric";
}


/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.22
 Modifier		: 
 Comments		: {{{ NDI = PDI - MDI }}}
-----------------------------------------------------------------------------*/
double CNDI::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod1 = _variant_t(varArg1);
	
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"OptVar1",NumericSeries,0);
		pIController->SetVariables(1,"OptVar2",NumericSeries,0);
		pIController->SetVariables(2,"OptVar3",NumericSeries,0);
		pIController->SetVariables(3,"OptVar4",NumericSeries,0);
		pIController->SetVariables(4,"OptVar5",NumericSeries,0);
		pIController->SetVariables(5,"OptVar6",NumericSeries,0);
		pIController->SetVariables(6,"OptVar7",NumericSeries,0);
		pIController->SetVariables(7,"OptVar8",NumericSeries,0);
	}
	return yesNDI(pIController->IsUpdateData()
		,pIController->BarIndex() ,lPeriod1
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetRawData(RAWCLOSEDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetVarHolderDataItem(3)
		,pIController->GetVarHolderDataItem(4)
		,pIController->GetVarHolderDataItem(5)
		,pIController->GetVarHolderDataItem(6)
		,pIController->GetVarHolderDataItem(7)
		,pIController->GetITempMemMng());
}
