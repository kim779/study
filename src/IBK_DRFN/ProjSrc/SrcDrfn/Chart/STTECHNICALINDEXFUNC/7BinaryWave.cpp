// 7BinaryWave.cpp: implementation of the C7BinaryWave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "7BinaryWave.h"
#include "indi_func.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C7BinaryWave::C7BinaryWave()
{
	m_strName = "SEVENBINARYWAVE";
}

C7BinaryWave::~C7BinaryWave()
{

}

CString C7BinaryWave::GetInputs(long lType)
{
	return "";
}

/*-----------------------------------------------------------------------------
 Author			:Geun-moon Oh Date :2005.7.18
 Modifier		: 
 Comments		: 7 Binary Wave
-----------------------------------------------------------------------------*/
double C7BinaryWave::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
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
		pIController->SetVariables(8,"OptValue9",NumericSeries,0);	
		pIController->SetVariables(9,"OptValue10",NumericSeries,0);		/*CCI ÀÎÇ²º¯°æ*/
		pIController->SetVariables(10,"OptValue11",NumericSeries,0);
		pIController->SetVariables(11,"OptValue12",NumericSeries,0);
		pIController->SetVariables(12,"OptValue13",NumericSeries,0);
		pIController->SetVariables(13,"OptValue14",NumericSeries,0);
		pIController->SetVariables(14,"OptValue15",NumericSeries,0);
		pIController->SetVariables(15,"OptValue16",NumericSeries,0);
		pIController->SetVariables(16,"OptValue17",NumericSeries,0);
	}	
	return yes7BinaryWave(pIController->IsUpdateData()
			,pIController->BarIndex()
			,pIController->GetRawData(RAWHIGHDATA)
			,pIController->GetRawData(RAWLOWDATA)
			,pIController->GetRawData(RAWCLOSEDATA)
			,pIController->GetRawData(RAWVOLUMEDATA)
			,pIController->GetVarHolderDataItem(0)
			,pIController->GetVarHolderDataItem(1)
			,pIController->GetVarHolderDataItem(2)
			,pIController->GetVarHolderDataItem(3)
			,pIController->GetVarHolderDataItem(4)
			,pIController->GetVarHolderDataItem(5)
			,pIController->GetVarHolderDataItem(6)
			,pIController->GetVarHolderDataItem(7)
			,pIController->GetVarHolderDataItem(8)
			,pIController->GetVarHolderDataItem(9)
			,pIController->GetVarHolderDataItem(10)
			,pIController->GetVarHolderDataItem(11)
			,pIController->GetVarHolderDataItem(12)
			,pIController->GetVarHolderDataItem(13)
			,pIController->GetVarHolderDataItem(14)
			,pIController->GetVarHolderDataItem(15)
			,pIController->GetVarHolderDataItem(16)
			,pIController->GetITempMemMng());
}
