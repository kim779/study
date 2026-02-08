// CMF.cpp: implementation of the CCMF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "CMF.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCMF::CCMF()
{
	m_strName = "CMF";
}

CCMF::~CCMF()
{

}

CString CCMF::GetInputs(long lType)
{
	return "Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.19
 Modifier		: 
 Comments		: {{{ CMF = AD / n일 거래량 합 }}}
				  {{{ 가중거래량 = [(종가-저가) - (고가-종가)] / (고가-저가) * 거래량 }}}
				  {{{ AD = 가중거래량의 n일 합 }}}
-----------------------------------------------------------------------------*/
double CCMF::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = _variant_t(varArg1);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,0);	
	}
	
	return yesCMF(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetRawData(RAWCLOSEDATA)
		,pIController->GetRawData(RAWVOLUMEDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());

}
