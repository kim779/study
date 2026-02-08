// AroonDown.cpp: implementation of the CAroonDown class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "AroonDown.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAroonDown::CAroonDown()
{
	m_strName = "AROONDOWN";
}

CAroonDown::~CAroonDown()
{

}

CString CAroonDown::GetInputs(long lType)
{
	return "Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.5
 Modifier		: 
 Comments		: {{{ 저가유지기간 : 저가의 최저가가 저가인 날과 당일 일수 차 }}}
				  {{{ Aroon Down =  (N-저가유지기간)/N*100 }}}
-----------------------------------------------------------------------------*/
double CAroonDown::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);
	return yesAroonDown(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetITempMemMng());
}
