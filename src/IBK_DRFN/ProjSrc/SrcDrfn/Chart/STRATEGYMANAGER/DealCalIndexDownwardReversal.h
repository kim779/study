// DealCalIndexDownwardReversal.h: interface for the CDealCalIndexDownwardReversal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALCALINDEXDOWNWARDREVERSAL_H__63F0E4ED_76A0_438C_ACA2_D7B66677A583__INCLUDED_)
#define AFX_DEALCALINDEXDOWNWARDREVERSAL_H__63F0E4ED_76A0_438C_ACA2_D7B66677A583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DealCal.h"
class CDealDataItem;

class CDealCalIndexDownwardReversal  : public CDealCal
{
public:
	CDealCalIndexDownwardReversal(CDealDataItem *pDealDataItem, long lDealValue);
	virtual ~CDealCalIndexDownwardReversal();
	void SetDeal(int nPos, double dValue);
private:
	long m_lDealValue;
	BOOL m_bSetFirst;
	BOOL m_bSetSecond;
	CDealDataItem *m_pDealDataItem;
	double m_dValueLast;
	double m_dValueLastBefore;

};

#endif // !defined(AFX_DEALCALINDEXDOWNWARDREVERSAL_H__63F0E4ED_76A0_438C_ACA2_D7B66677A583__INCLUDED_)
