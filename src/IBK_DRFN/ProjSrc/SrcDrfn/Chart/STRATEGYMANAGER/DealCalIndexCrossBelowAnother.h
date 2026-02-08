// DealCalIndexCrossBelowAnother.h: interface for the CDealCalIndexCrossBelowAnother class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALCALINDEXCROSSBELOWANOTHER_H__140CE4C1_EE3A_42EE_AC3E_76B820755BB4__INCLUDED_)
#define AFX_DEALCALINDEXCROSSBELOWANOTHER_H__140CE4C1_EE3A_42EE_AC3E_76B820755BB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Common_ST/DataItem.h"
#include "DealCal.h"
class CDealDataItem;
class CDealCalIndexCrossBelowAnother    : public CDealCal
{
public:
	CDealCalIndexCrossBelowAnother(CDealDataItem *pDealDataItem, long lDealValue, CIDataItem *pPlotItemAnother);
	virtual ~CDealCalIndexCrossBelowAnother();
	void SetDeal(int nPos, double dValue);
private:
	long m_lDealValue;
	BOOL m_bSetFirst;
	CDealDataItem *m_pDealDataItem;
	double m_dValueLast;
	double m_dValueAnotherLast;
	CIDataItem *m_pPlotItemAnother;
	int m_nStartPosAnother;
};

#endif // !defined(AFX_DEALCALINDEXCROSSBELOWANOTHER_H__140CE4C1_EE3A_42EE_AC3E_76B820755BB4__INCLUDED_)
