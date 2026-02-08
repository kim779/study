// ExitPosition.cpp: implementation of the CExitPosition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExitPosition.h"
#include "../Include_ST/ReservedCommandDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExitPosition::CExitPosition()
{

}

CExitPosition::~CExitPosition()
{

}

CString CExitPosition::GetInputs(long lType)
{
	CString strInputs;
	strInputs.Format("[%s]String="",Numeric=0,Numeric=0,String="",Numeric=0,Numeric=0",m_strInputInfo);
	return strInputs;
}


double CExitPosition::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	CString strFuncName = (LPCTSTR)_bstr_t(_variant_t(varArg1));
	long lOrderType = _variant_t(varArg2);
	double dPrice = _variant_t(varArg3);
	CString strEntryName = (LPCTSTR)_bstr_t(_variant_t(varArg4));
	double dContracts = _variant_t(varArg5);
	long lExitOption = _variant_t(varArg6);
	double dRetValue  = 0;
	long lValueOrderType = 0;
	switch(lOrderType)
	{
	case AtMarket:
		{				
			lValueOrderType = DEALATMARKET;
		}
		break;
	case AtLimit:
		{
			lValueOrderType = DEALATLIMIT;
		}
		break;
	case AtStop:
		{
			lValueOrderType = DEALATSTOP;
		}
		break;
	default: //case OnClose:
		{
			lValueOrderType = NULL;
		}		
		break;
	}
	if(lExitOption)
	{
		lValueOrderType  |= DEALEXITDEFINED;
	}
	pIController->SetDealPosData(m_dPosValue,lValueOrderType,strFuncName,dContracts,dPrice,strEntryName);
	return dRetValue;
}


