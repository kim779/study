// ADXR.cpp: implementation of the CADXR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "ADXR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CADXR::CADXR()
{
	m_strName = "ADXR";
}

CADXR::~CADXR()
{

}


CString CADXR::GetInputs(long lType)
{
	return "Numeric, Numeric";
}


/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.22
 Modifier		: 
 Comments		: {{{ ADXR = ADX의 n일 이동평균 }}}
-----------------------------------------------------------------------------*/
double CADXR::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod1 = _variant_t(varArg1);
	long lPeriod2 = _variant_t(varArg2);
	
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
		pIController->SetVariables(8,"OptVar9",NumericSeries,0);
		pIController->SetVariables(9,"OptVar10",NumericSeries,0);
		pIController->SetVariables(10,"OptVar11",NumericSeries,0);
		pIController->SetVariables(11,"OptVar12",NumericSeries,0);
	}
	return yesADXR(pIController->IsUpdateData()
		,pIController->BarIndex() ,lPeriod1 ,lPeriod2
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
		,pIController->GetVarHolderDataItem(8)
		,pIController->GetVarHolderDataItem(9)
		,pIController->GetVarHolderDataItem(10)
		,pIController->GetVarHolderDataItem(11)
		,pIController->GetITempMemMng());
}

