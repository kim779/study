// StoFastD.cpp: implementation of the CStoFastD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "StoFastD.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStoFastD::CStoFastD()
{
	m_strName = "STOFASTD";
}

CStoFastD::~CStoFastD()
{

}

CString CStoFastD::GetInputs(long lType)
{
//	return "Numeric, Numeric,Numeric=close";
	return "Numeric, Numeric,Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.20
 Modifier		: 
 Comments		: {{{ fast %D = %K를 n일 지수이동평균 }}}
-----------------------------------------------------------------------------*/
double CStoFastD::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod1 = 	_variant_t(varArg1);
	long lPeriod2 = 	_variant_t(varArg2);
	double dValue = _variant_t(varArg3);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);
		pIController->SetVariables(1,"OptValue1",NumericSeries,0);
		pIController->SetVariables(2,"OptValue2",NumericSeries,0);
		pIController->SetVariables(3,"OptValue3",NumericSeries,0);
		pIController->SetVariables(4,"OptValue3",NumericSeries,0);
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesStoFastD(pIController->IsUpdateData()
		,pIController->BarIndex()
		, lPeriod1, lPeriod2
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetVarHolderDataItem(3)
		,pIController->GetVarHolderDataItem(4)
		,pIController->GetITempMemMng());
}
