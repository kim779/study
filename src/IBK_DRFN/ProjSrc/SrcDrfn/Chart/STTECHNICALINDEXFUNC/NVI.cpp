// NVI.cpp: implementation of the CNVI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "NVI.h"
#include "../Common_ST/STControllerDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNVI::CNVI()
{
	m_strName = "NVI";
}

CNVI::~CNVI()
{

}

CString CNVI::GetInputs(long lType)
{
	return "";
}

double CNVI::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	return yesNVI(pIController->IsUpdateData()
		,pIController->BarIndex()
		,pIController->GetRawData(RAWCLOSEDATA)
		,pIController->GetRawData(RAWVOLUMEDATA)
		,pIController->GetITempMemMng());

}
