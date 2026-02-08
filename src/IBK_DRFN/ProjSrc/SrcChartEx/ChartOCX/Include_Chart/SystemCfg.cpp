// SystemCfg.cpp: implementation of the CSystemCfg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SystemCfg.h"
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSystemCfg::CSystemCfg(const CString& strCfg)
{	
	CString szCfg = strCfg;

	CDataConversion::GetStringData(szCfg, "=");
	m_Investment = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_PriceEnter = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_PriceClear = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	CString tmp = CDataConversion::GetStringData(szCfg, "\r\n");
	m_bUseShortSystem = tmp == "1" ? true : false;
	CDataConversion::GetStringData(szCfg, "=");
	m_CommissionLong = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_CommissionShort = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_TaxLong = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_TaxShort = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_MaxLossAllow = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_GoalPrifit = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_MaxProfit = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_ProfitDecline = CDataConversion::GetStringData(szCfg, "\r\n");
}

CSystemCfg::~CSystemCfg()
{

}

void CSystemCfg::ResetCfg(const CString &strCfg)
{
	CString szCfg = strCfg;

	CDataConversion::GetStringData(szCfg, "=");
	m_Investment = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_PriceEnter = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_PriceClear = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	CString tmp = CDataConversion::GetStringData(szCfg, "\r\n");
	m_bUseShortSystem = tmp == "1" ? true : false;
	CDataConversion::GetStringData(szCfg, "=");
	m_CommissionLong = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_CommissionShort = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_TaxLong = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_TaxShort = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_MaxLossAllow = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_GoalPrifit = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_MaxProfit = CDataConversion::GetStringData(szCfg, "\r\n");
	CDataConversion::GetStringData(szCfg, "=");
	m_ProfitDecline = CDataConversion::GetStringData(szCfg, "\r\n");
}

void CSystemCfg::ResetCfg(const CString &Investment, 
						  const CString &PriceEnter,
						  const CString &PriceClear,
						  const bool &bUseShortSystem,
						  const CString &CommissionLong,
						  const CString &CommissionShort,
						  const CString &TaxLong,
						  const CString &TaxShort,
						  const CString &MaxLossAllow,
						  const CString &GoalPrifit,
						  const CString &MaxProfit,
						  const CString &ProfitDecline)
{	
	m_Investment = Investment;	
	m_PriceEnter = PriceEnter;
	m_PriceClear = PriceClear;
	m_bUseShortSystem = bUseShortSystem;
	m_CommissionLong = CommissionLong;
	m_CommissionShort = CommissionShort;
	m_TaxLong = TaxLong;
	m_TaxShort = TaxShort;
	m_MaxLossAllow = MaxLossAllow;
	m_GoalPrifit = GoalPrifit;
	m_MaxProfit = MaxProfit;
	m_ProfitDecline = ProfitDecline;
}