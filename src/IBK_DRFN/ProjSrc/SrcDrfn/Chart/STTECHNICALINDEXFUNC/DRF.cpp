// DRF.cpp: implementation of the CDRF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "DRF.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDRF::CDRF()
{
	m_strName = "DRF";
}

CDRF::~CDRF()
{

}

CString CDRF::GetInputs(long lType)
{
	return "";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.19
 Modifier		: 
 Comments		: {{{ DRF = (BP + SP) / [2*(고가-저가)] * 100 }}}
				  {{{ BP = 고가 - [(고가-전일종가)/(고가-저가)] }}}
				  {{{ SP = 종가 - 저가 }}}
-----------------------------------------------------------------------------*/
double CDRF::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	return yesDRF(pIController->IsUpdateData()
		,pIController->BarIndex()
		,pIController->GetRawData(RAWHIGHDATA)
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetRawData(RAWCLOSEDATA)
		,pIController->GetITempMemMng());

}
