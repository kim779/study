// SystemCfg.h: interface for the CSystemCfg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMCFG_H__4D097C6F_10E9_11D6_9374_0050FC28B229__INCLUDED_)
#define AFX_SYSTEMCFG_H__4D097C6F_10E9_11D6_9374_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSystemCfg  
{
public:
	void ResetCfg(const CString& strCfg);
	void ResetCfg(const CString &Investment, 
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
						  const CString &ProfitDecline);
	CSystemCfg(const CString& strCfg);
	virtual ~CSystemCfg();

public:
	CString m_Investment;
	CString m_PriceEnter;
	CString m_PriceClear;
	bool m_bUseShortSystem;
	CString m_CommissionLong;
	CString m_CommissionShort;	
	CString m_TaxLong;
	CString m_TaxShort;
	CString m_MaxLossAllow;
	CString m_GoalPrifit;
	CString m_MaxProfit;
	CString m_ProfitDecline;
};

#endif // !defined(AFX_SYSTEMCFG_H__4D097C6F_10E9_11D6_9374_0050FC28B229__INCLUDED_)
