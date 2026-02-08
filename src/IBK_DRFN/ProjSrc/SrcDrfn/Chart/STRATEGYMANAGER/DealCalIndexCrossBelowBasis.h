// DealCalIndexCrossBelowBasis.h: interface for the CDealCalIndexCrossBelowBasis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALCALINDEXCROSSBELOWBASIS_H__A16D0B42_633A_4386_B833_0CB7A27973C0__INCLUDED_)
#define AFX_DEALCALINDEXCROSSBELOWBASIS_H__A16D0B42_633A_4386_B833_0CB7A27973C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DealCal.h"
class CDealDataItem;
class CDealCalIndexCrossBelowBasis : public CDealCal
{
public:
	CDealCalIndexCrossBelowBasis(CDealDataItem *pDealDataItem, long lDealValue, double dBasis);
	virtual ~CDealCalIndexCrossBelowBasis();
	void SetDeal(int nPos, double dValue);
private:
	long m_lDealValue;
	BOOL m_bSetFirst;
	CDealDataItem *m_pDealDataItem;
	double m_dValueLast;
	double m_dBasis;

};

#endif // !defined(AFX_DEALCALINDEXCROSSBELOWBASIS_H__A16D0B42_633A_4386_B833_0CB7A27973C0__INCLUDED_)
