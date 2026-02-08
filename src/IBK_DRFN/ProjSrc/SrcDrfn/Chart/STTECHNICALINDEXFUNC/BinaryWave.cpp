// BinaryWave.cpp: implementation of the CBinaryWave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "BinaryWave.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBinaryWave::CBinaryWave()
{
	m_strName = "BINARYWAVE";
}

CBinaryWave::~CBinaryWave()
{

}

CString CBinaryWave::GetInputs(long lType)
{
	return "Numeric, Numeric, Numeric, Numeric, Numeric, Numeric, Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.18
 Modifier		: 
 Comments		: {{{ MACD Osc, 이격도, ROC, Stockasric 지표조합지표 }}}
-----------------------------------------------------------------------------*/
double CBinaryWave::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
//	long lShortPeriod = _variant_t(varArg1);
//	long lLongPeriod = 	_variant_t(varArg2);
//	long lSignalPeriod = 	_variant_t(varArg3);
//	long lDisparityPeriod = _variant_t(varArg4);
//	long lROCPeriod = _variant_t(varArg5);
//	long lStoKPeriod = 	_variant_t(varArg6);
//	long lStoDPeriod = 	_variant_t(varArg7);
	long lDisparityPeriod = _variant_t(varArg1);
	long lROCPeriod = _variant_t(varArg2);
	long lStoKPeriod = _variant_t(varArg3);
	long lStoDPeriod = _variant_t(varArg4);
	long lShortPeriod = _variant_t(varArg5);
	long lLongPeriod = _variant_t(varArg6);
	long lSignalPeriod = _variant_t(varArg7);
	
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"OptValue1",NumericSeries,0);	
		pIController->SetVariables(1,"OptValue2",NumericSeries,0);
		pIController->SetVariables(2,"OptValue3",NumericSeries,0);
		pIController->SetVariables(3,"OptValue4",NumericSeries,0);
		pIController->SetVariables(4,"OptValue5",NumericSeries,0);
		pIController->SetVariables(5,"OptValue6",NumericSeries,0);
		pIController->SetVariables(6,"OptValue7",NumericSeries,0);
		pIController->SetVariables(7,"OptValue8",NumericSeries,0);
	}
	return yesBinaryWave(pIController->IsUpdateData()
			,pIController->BarIndex()
			,lShortPeriod, lLongPeriod, lSignalPeriod, lDisparityPeriod
			, lROCPeriod, lStoKPeriod, lStoDPeriod
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
