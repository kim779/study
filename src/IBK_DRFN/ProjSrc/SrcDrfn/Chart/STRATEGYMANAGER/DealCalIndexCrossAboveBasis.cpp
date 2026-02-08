// DealCalIndexCrossAboveBasis.cpp: implementation of the CDealCalIndexCrossAboveBasis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealCalIndexCrossAboveBasis.h"
#include "../Common_ST/DealDataItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealCalIndexCrossAboveBasis::CDealCalIndexCrossAboveBasis(CDealDataItem *pDealDataItem, long lDealValue, double dBasis)
{
	m_lDealValue = lDealValue;
	m_pDealDataItem = pDealDataItem;
	m_dBasis = dBasis;
	m_bSetFirst = TRUE;
	m_dValueLast = 0.;
}

CDealCalIndexCrossAboveBasis::~CDealCalIndexCrossAboveBasis()
{

}


void CDealCalIndexCrossAboveBasis::SetDeal(int nPos, double dValue)
{
	if(m_bSetFirst)
	{
		m_dValueLast = dValue;
		m_bSetFirst = FALSE;
		return;
	}
	if(m_dValueLast<=m_dBasis&&dValue>m_dBasis)
	{
		m_pDealDataItem->SetData(nPos,m_lDealValue);
	}
	m_dValueLast = dValue;
}