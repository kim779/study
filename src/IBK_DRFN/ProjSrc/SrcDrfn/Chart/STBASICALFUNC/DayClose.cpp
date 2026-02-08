// DayClose.cpp: implementation of the CDayClose class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DayClose.h"
#include "../Common_ST/STControllerDef.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDayClose::CDayClose()
{
	m_strName = "DAYCLOSE";
}

CDayClose::~CDayClose()
{

}

CString CDayClose::GetInputs(long lType)
{
	return "Numeric";
}

double CDayClose::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dRetVal = HUGE_VAL;
	long lReverseIndex = _variant_t(varArg1);	
	long lBarIndex = pIController->BarIndex();
	CString strTimeFormat = pIController->GetRawSData(RAWDATETYPE,0);
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetArray(0,"OptVal1",NumericSeries,0,pIController->GetCntOfAllApData());		
	}
	if(strTimeFormat=="MMDDHHMM")
	{
		CITempMemMng *pTempMenMng = pIController->GetITempMemMng();
		double *pDTemp = pTempMenMng->GetDblArray(10);
		double *pDIndexDay = pDTemp;
		double *pDLastValueDay = pDTemp+1;

		CString strTime = pIController->GetRawSData(RAWDATEDATA,0);
		strTime.Delete(strTime.GetLength()-4,4);
		double dTime = atof(strTime);
		if(dTime!=*pDLastValueDay)
		{
			*pDLastValueDay = dTime;			
			pIController->SetArrayHolderData(0,*pDTemp,pIController->GetRawData(RAWCLOSEDATA,0));
			*pDTemp += 1;			
		}
		else
		{	
			pIController->SetArrayHolderData(0,*pDTemp-1,pIController->GetRawData(RAWCLOSEDATA,0));
		}

		if(lReverseIndex<*pDTemp)
		{
			dRetVal = pIController->GetArrayHolderData(0,*pDTemp-1-lReverseIndex);
		}
	}
	else if(strTimeFormat=="DDHHMMSS")
	{
		CITempMemMng *pTempMenMng = pIController->GetITempMemMng();
		double *pDTemp = pTempMenMng->GetDblArray(10);
		double *pDIndexDay = pDTemp;
		double *pDLastValueDay = pDTemp+1;

		CString strTime = pIController->GetRawSData(RAWDATEDATA,0);
		strTime.Delete(strTime.GetLength()-6,6);
		double dTime = atof(strTime);
		if(dTime!=*pDLastValueDay)
		{
			*pDLastValueDay = dTime;			
			pIController->SetArrayHolderData(0,*pDTemp,pIController->GetRawData(RAWCLOSEDATA,0));
			*pDTemp += 1;			
		}
		else
		{	
			pIController->SetArrayHolderData(0,*pDTemp-1,pIController->GetRawData(RAWCLOSEDATA,0));
		}

		if(lReverseIndex<*pDTemp)
		{
			dRetVal = pIController->GetArrayHolderData(0,*pDTemp-1-lReverseIndex);
		}
	}
	else if(strTimeFormat=="YYYYMMDD")
	{
		if(lReverseIndex<=lBarIndex)
		{
			dRetVal = pIController->GetRawData(RAWCLOSEDATA,lReverseIndex);
		}			
	}
	else if(strTimeFormat=="HHMMSS")
		dRetVal = pIController->GetRawData(RAWPREVCLOSEDATA,0);

	return dRetVal;
}

