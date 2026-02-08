// AccDist.cpp: implementation of the CAccDist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "AccDist.h"
#include "../Common_ST/STControllerDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAccDist::CAccDist()
{
	m_strName = "ACCDIST";
}

CAccDist::~CAccDist()
{

}

CString CAccDist::GetName(long lType)
{
	switch(lType)
	{
	case YES_LANG: 
		return m_strName; break;
	default:// TS_LANG
		return m_strName;	break;
	}	
}

CString CAccDist::GetInputs(long lType)
{
	return "";
}

double CAccDist::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"optvar",NumericSeries,0);
	}	
	return yesAccDist(pIController->IsUpdateData()
		,pIController->BarIndex()
		,pIController->GetRawData(RAWOPENDATA)	
		,pIController->GetRawData(RAWHIGHDATA)	
		,pIController->GetRawData(RAWLOWDATA)
		,pIController->GetRawData(RAWCLOSEDATA)
		,pIController->GetRawData(RAWVOLUMEDATA)
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}
