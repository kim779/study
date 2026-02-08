// DayHigh.cpp: implementation of the CDayHigh class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DayHigh.h"
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

CDayHigh::CDayHigh()
{
	m_strName = "DAYHIGH";
}

CDayHigh::~CDayHigh()
{

}

CString CDayHigh::GetInputs(long lType)
{
	return "Numeric";
}

double CDayHigh::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
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
		double *pDTemp = pTempMenMng->GetDblArray(5);
		double *pDIndexDay = pDTemp;
		double *pDLastValueDay = pDTemp+1;
		double *pDCurrentHighValue = pDTemp+2;
		double *pDFirstDate = pDTemp+3;

		CString strDate = pIController->GetRawSData(RAWDATEDATA,0);
		strDate.Delete(strDate.GetLength()-4,4);
		double dDate = atof(strDate);			
		if(*pDFirstDate!=0&&*pDFirstDate!=dDate)
		{
			if(dDate!=*pDLastValueDay)
			{
				*pDLastValueDay = dDate;				
				*pDCurrentHighValue = pIController->GetRawData(RAWHIGHDATA,0);
				pIController->SetArrayHolderData(0,*pDTemp,*pDCurrentHighValue);
				*pDTemp += 1;			
			}
			else 
			{	double dHighValue = pIController->GetRawData(RAWHIGHDATA,0);
				if(*pDCurrentHighValue<dHighValue)
				{
					*pDCurrentHighValue = dHighValue;
					pIController->SetArrayHolderData(0,*pDTemp-1,*pDCurrentHighValue);
				}				
			}
			if(lReverseIndex<*pDTemp)
			{
				dRetVal = pIController->GetArrayHolderData(0,*pDTemp-1-lReverseIndex);
			}			
		}
		else
		{
			*pDFirstDate = dDate;
		}			
	}
	else if(strTimeFormat=="DDHHMMSS")
	{
		CITempMemMng *pTempMenMng = pIController->GetITempMemMng();
		double *pDTemp = pTempMenMng->GetDblArray(5);
		double *pDIndexDay = pDTemp;
		double *pDLastValueDay = pDTemp+1;
		double *pDCurrentHighValue = pDTemp+2;
		double *pDFirstDate = pDTemp+3;

		CString strDate = pIController->GetRawSData(RAWDATEDATA,0);
		strDate.Delete(strDate.GetLength()-6,6);
		double dDate = atof(strDate);			
		if(*pDFirstDate!=0&&*pDFirstDate!=dDate)
		{
			if(dDate!=*pDLastValueDay)
			{
				*pDLastValueDay = dDate;				
				*pDCurrentHighValue = pIController->GetRawData(RAWHIGHDATA,0);
				pIController->SetArrayHolderData(0,*pDTemp,*pDCurrentHighValue);
				*pDTemp += 1;			
			}
			else 
			{	double dHighValue = pIController->GetRawData(RAWHIGHDATA,0);
				if(*pDCurrentHighValue<dHighValue)
				{
					*pDCurrentHighValue = dHighValue;
					pIController->SetArrayHolderData(0,*pDTemp-1,*pDCurrentHighValue);
				}				
			}
			if(lReverseIndex<*pDTemp)
			{
				dRetVal = pIController->GetArrayHolderData(0,*pDTemp-1-lReverseIndex);
			}			
		}
		else
		{
			*pDFirstDate = dDate;
		}			
	}
	else if(strTimeFormat=="YYYYMMDD")
	{
		if(lReverseIndex<=lBarIndex)
		{
			dRetVal = pIController->GetRawData(RAWHIGHDATA,lReverseIndex);
		}			
	}	
	else if(strTimeFormat=="HHMMSS")
		dRetVal = pIController->GetRawData(RAWPREVHIGHDATA,0);

	return dRetVal;
}

