// FeeCalByPercent.h: interface for the CFeeCalByPercent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEECALBYPERCENT_H__A4E0146B_53D4_40FB_9FFA_3589483ACB8A__INCLUDED_)
#define AFX_FEECALBYPERCENT_H__A4E0146B_53D4_40FB_9FFA_3589483ACB8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FeeCal.h"

class CFeeCalByPercent  : public CFeeCal
{
public:
	CFeeCalByPercent(double dFeeForEntry, double dFeeForExit);
	virtual ~CFeeCalByPercent();
	void Init();
	void Entry(double dPrice, long lVolume);
	double Exit(double dPrice, long lVolume);
private:
	double m_dFeeForEntry;
	double m_dFeeForExit;
	double m_dFee;
};

#endif // !defined(AFX_FEECALBYPERCENT_H__A4E0146B_53D4_40FB_9FFA_3589483ACB8A__INCLUDED_)
