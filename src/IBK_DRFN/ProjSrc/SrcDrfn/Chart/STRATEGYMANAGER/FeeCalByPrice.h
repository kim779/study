// FeeCalByPrice.h: interface for the CFeeCalByPrice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEECALBYPRICE_H__7A553833_D418_46D1_84A9_6C6C1DCE60AA__INCLUDED_)
#define AFX_FEECALBYPRICE_H__7A553833_D418_46D1_84A9_6C6C1DCE60AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FeeCal.h"

class CFeeCalByPrice  : public CFeeCal
{
public:
	CFeeCalByPrice(double dFeeForEntry, double dFeeForExit);
	virtual ~CFeeCalByPrice();
	void Init();
	void Entry(double dPrice, long lVolume);
	double Exit(double dPrice, long lVolume);
private:
	double m_dFeeForEntry;
	double m_dFeeForExit;
	double m_dFee;
};

#endif // !defined(AFX_FEECALBYPRICE_H__7A553833_D418_46D1_84A9_6C6C1DCE60AA__INCLUDED_)
