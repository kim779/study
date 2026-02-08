// DealCalIndexCrossAboveAnother.h: interface for the CDealCalIndexCrossAboveAnother class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALCALINDEXCROSSABOVEANOTHER_H__32E39602_0B24_44B8_AED1_26007A5E2795__INCLUDED_)
#define AFX_DEALCALINDEXCROSSABOVEANOTHER_H__32E39602_0B24_44B8_AED1_26007A5E2795__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Common_ST/DataItem.h"
#include "DealCal.h"
class CDealDataItem;
class CDealCalIndexCrossAboveAnother  : public CDealCal
{
public:
	CDealCalIndexCrossAboveAnother(CDealDataItem *pDealDataItem, long lDealValue, CIDataItem *pPlotItemAnother);
	virtual ~CDealCalIndexCrossAboveAnother();
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

#endif // !defined(AFX_DEALCALINDEXCROSSABOVEANOTHER_H__32E39602_0B24_44B8_AED1_26007A5E2795__INCLUDED_)
