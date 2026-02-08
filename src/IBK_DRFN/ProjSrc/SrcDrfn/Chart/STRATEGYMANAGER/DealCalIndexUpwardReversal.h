// DealCalIndexUpwardReversal.h: interface for the CDealCalIndexUpwardReversal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALCALINDEXUPWARDREVERSAL_H__E5F292BE_19CE_4FDF_9016_9B61C991D703__INCLUDED_)
#define AFX_DEALCALINDEXUPWARDREVERSAL_H__E5F292BE_19CE_4FDF_9016_9B61C991D703__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DealCal.h"
class CDealDataItem;

class CDealCalIndexUpwardReversal   : public CDealCal
{
public:
	CDealCalIndexUpwardReversal(CDealDataItem *pDealDataItem, long lDealValue);
	virtual ~CDealCalIndexUpwardReversal();
	void SetDeal(int nPos, double dValue);
private:
	long m_lDealValue;
	BOOL m_bSetFirst;
	BOOL m_bSetSecond;
	CDealDataItem *m_pDealDataItem;
	double m_dValueLast;
	double m_dValueLastBefore;
};

#endif // !defined(AFX_DEALCALINDEXUPWARDREVERSAL_H__E5F292BE_19CE_4FDF_9016_9B61C991D703__INCLUDED_)
