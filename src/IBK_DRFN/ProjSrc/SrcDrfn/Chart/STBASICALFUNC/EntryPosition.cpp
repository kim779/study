// EntryPosition.cpp: implementation of the CEntryPosition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EntryPosition.h"
#include "../Include_ST/ReservedCommandDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEntryPosition::CEntryPosition()
{

}

CEntryPosition::~CEntryPosition()
{

}

CString CEntryPosition::GetInputs(long lType)
{
	CString strInputs;
	strInputs.Format("[%s]String="",Numeric=0,Numeric=0,Numeric=0",m_strInputInfo);
	return strInputs;
}


double CEntryPosition::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	CString strFuncName = (LPCTSTR)_bstr_t(_variant_t(varArg1));
	long lOrderType = _variant_t(varArg2);
	double dPrice = _variant_t(varArg3);
	double dContracts = _variant_t(varArg4);
	double dRetValue  = 0;
	long lValueOrderType = 0;
	switch(lOrderType)
	{
	case AtMarket:
		{				
			lOrderType = DEALATMARKET;
		}
		break;
	case AtLimit:
		{
			lOrderType = DEALATLIMIT;
		}
		break;
	case AtStop:
		{
			lOrderType = DEALATSTOP;
		}
		break;
	default: //case OnClose:
		{
			lOrderType = NULL;
		}		
		break;
	}
	pIController->SetDealPosData(m_dPosValue,lOrderType,strFuncName,dContracts,dPrice,NULL);
	return dRetValue;
}


