// DealCalIndexCrossBelowBasis.cpp: implementation of the CDealCalIndexCrossBelowBasis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealCalIndexCrossBelowBasis.h"
#include "../Common_ST/DealDataItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealCalIndexCrossBelowBasis::CDealCalIndexCrossBelowBasis(CDealDataItem *pDealDataItem, long lDealValue, double dBasis)
{
	m_lDealValue = lDealValue;
	m_pDealDataItem = pDealDataItem;
	m_dBasis = dBasis;
	m_bSetFirst = TRUE;
	m_dValueLast = 0.;

}

CDealCalIndexCrossBelowBasis::~CDealCalIndexCrossBelowBasis()
{

}

void CDealCalIndexCrossBelowBasis::SetDeal(int nPos, double dValue)
{
	if(m_bSetFirst)
	{
		m_dValueLast = dValue;
		m_bSetFirst = FALSE;
		return;
	}
	if(m_dValueLast>=m_dBasis&&dValue<m_dBasis)
	{
		m_pDealDataItem->SetData(nPos,m_lDealValue);
	} 
	m_dValueLast = dValue;
}