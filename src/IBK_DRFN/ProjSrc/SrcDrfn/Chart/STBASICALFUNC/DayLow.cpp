// DayLow.cpp: implementation of the CDayLow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DayLow.h"
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

CDayLow::CDayLow()
{
	m_strName = "DAYLOW";
}

CDayLow::~CDayLow()
{

}

CString CDayLow::GetInputs(long lType)
{
	return "Numeric";
}

double CDayLow::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
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
		double *pDCurrentLowValue = pDTemp+2;
		double *pDFirstDate = pDTemp+3;

		CString strDate = pIController->GetRawSData(RAWDATEDATA,0);
		strDate.Delete(strDate.GetLength()-4,4);
		double dDate = atof(strDate);			
		if(*pDFirstDate!=0&&*pDFirstDate!=dDate)
		{
			if(dDate!=*pDLastValueDay)
			{
				*pDLastValueDay = dDate;				
				*pDCurrentLowValue = pIController->GetRawData(RAWLOWDATA,0);
				pIController->SetArrayHolderData(0,*pDTemp,*pDCurrentLowValue);
				*pDTemp += 1;			
			}
			else 
			{	double dLowValue = pIController->GetRawData(RAWLOWDATA,0);
				if(*pDCurrentLowValue>dLowValue)
				{
					*pDCurrentLowValue = dLowValue;
					pIController->SetArrayHolderData(0,*pDTemp-1,*pDCurrentLowValue);
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
		double *pDCurrentLowValue = pDTemp+2;
		double *pDFirstDate = pDTemp+3;

		CString strDate = pIController->GetRawSData(RAWDATEDATA,0);
		strDate.Delete(strDate.GetLength()-6,6);
		double dDate = atof(strDate);			
		if(*pDFirstDate!=0&&*pDFirstDate!=dDate)
		{
			if(dDate!=*pDLastValueDay)
			{
				*pDLastValueDay = dDate;				
				*pDCurrentLowValue = pIController->GetRawData(RAWLOWDATA,0);
				pIController->SetArrayHolderData(0,*pDTemp,*pDCurrentLowValue);
				*pDTemp += 1;			
			}
			else 
			{	double dLowValue = pIController->GetRawData(RAWLOWDATA,0);
				if(*pDCurrentLowValue>dLowValue)
				{
					*pDCurrentLowValue = dLowValue;
					pIController->SetArrayHolderData(0,*pDTemp-1,*pDCurrentLowValue);
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
			dRetVal = pIController->GetRawData(RAWLOWDATA,lReverseIndex);
		}			
	}	
	else if(strTimeFormat=="HHMMSS")
		dRetVal = pIController->GetRawData(RAWPREVLOWDATA,0);

	return dRetVal;
}

