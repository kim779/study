// OSCV.cpp: implementation of the COSCV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "../Common_ST/STControllerDef.h"
#include "OSCV.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COSCV::COSCV()
{
	m_strName = "OSCV";
}

COSCV::~COSCV()
{

}

CString COSCV::GetInputs(long lType)
{
	return "Numeric,Numeric";
}

double COSCV::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lShortPeriod = _variant_t(varArg1);
	long lLongPeriod = 	_variant_t(varArg2);
	return yesOSCV(pIController->IsUpdateData()
		,pIController->BarIndex()
		, lShortPeriod, lLongPeriod
		,pIController->GetRawData(RAWVOLUMEDATA)
		,pIController->GetITempMemMng());

}
