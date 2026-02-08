// DealCalIndexCrossAboveAnother.cpp: implementation of the CDealCalIndexCrossAboveAnother class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealCalIndexCrossAboveAnother.h"
#include "../Common_ST/DealDataItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealCalIndexCrossAboveAnother::CDealCalIndexCrossAboveAnother(CDealDataItem *pDealDataItem, long lDealValue, CIDataItem *pPlotItemAnother)
{
	m_lDealValue = lDealValue;
	m_pDealDataItem = pDealDataItem;
	m_pPlotItemAnother = pPlotItemAnother;
	m_nStartPosAnother = m_pPlotItemAnother->GetFirstDataIndex();
	m_bSetFirst = TRUE;
	m_dValueLast = 0.;
	m_dValueAnotherLast = 0.;
}

CDealCalIndexCrossAboveAnother::~CDealCalIndexCrossAboveAnother()
{

}


void CDealCalIndexCrossAboveAnother::SetDeal(int nPos, double dValue)
{
	if(m_nStartPosAnother<nPos) return;
	if(m_bSetFirst)
	{
		m_dValueLast = dValue;
		m_bSetFirst = FALSE;
		return;
	}
	double dValueAnother = m_pPlotItemAnother->GetAt(nPos);
	if(m_dValueLast<=m_dValueAnotherLast&&dValue>dValueAnother)
	{
		m_pDealDataItem->SetData(nPos,m_lDealValue);
	}
	m_dValueLast = dValue;
	m_dValueAnotherLast = dValueAnother;
}