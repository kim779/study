// DealCalIndexCrossAboveBasis.h: interface for the CDealCalIndexCrossAboveBasis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALCALINDEXCROSSABOVEBASIS_H__0DDA8A6C_E03D_4BBD_816D_D8D03A10CAF2__INCLUDED_)
#define AFX_DEALCALINDEXCROSSABOVEBASIS_H__0DDA8A6C_E03D_4BBD_816D_D8D03A10CAF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DealCal.h"
class CDealDataItem;
class CDealCalIndexCrossAboveBasis : public CDealCal
{
public:
	CDealCalIndexCrossAboveBasis(CDealDataItem *pDealDataItem, long lDealValue, double dBasis);
	virtual ~CDealCalIndexCrossAboveBasis();
	void SetDeal(int nPos, double dValue);
private:
	long m_lDealValue;
	BOOL m_bSetFirst;
	CDealDataItem *m_pDealDataItem;
	double m_dValueLast;
	double m_dBasis;
};

#endif // !defined(AFX_DEALCALINDEXCROSSABOVEBASIS_H__0DDA8A6C_E03D_4BBD_816D_D8D03A10CAF2__INCLUDED_)
