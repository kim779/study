// ForceIndex.cpp: implementation of the CForceIndex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "ForceIndex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CForceIndex::CForceIndex()
{
	m_strName = "FORCEINDEX";
}

CForceIndex::~CForceIndex()
{

}

CString CForceIndex::GetInputs(long lType)
{
//	return "Numeric, Numeric=close";
	return "Numeric, Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.8
 Modifier		: 
 Comments		: {{{ Force Index = [ (당일종가 - 전일종가) * 당일거래량 / 당일종가 ]의 지수이동평균 }}}
-----------------------------------------------------------------------------*/
double CForceIndex::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);
	double dValue = _variant_t(varArg2);
	
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1",NumericSeries,dValue);
		pIController->SetVariables(1,"Value2", NumericSeries,0);		
		pIController->SetVariables(2,"Opt1", NumericSeries,0);
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesForceIndex(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetRawData(RAWVOLUMEDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetITempMemMng());
 }
