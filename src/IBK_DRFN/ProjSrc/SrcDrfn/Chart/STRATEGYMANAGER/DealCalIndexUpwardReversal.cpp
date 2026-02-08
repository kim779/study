// DealCalIndexUpwardReversal.cpp: implementation of the CDealCalIndexUpwardReversal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DealCalIndexUpwardReversal.h"
#include "../Common_ST/DealDataItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDealCalIndexUpwardReversal::CDealCalIndexUpwardReversal(CDealDataItem *pDealDataItem, long lDealValue)
{
	m_lDealValue = lDealValue;
	m_pDealDataItem = pDealDataItem;
	m_bSetFirst = TRUE;
	m_bSetSecond = TRUE;
	m_dValueLast = 0.;
	m_dValueLastBefore = 0.;
}

CDealCalIndexUpwardReversal::~CDealCalIndexUpwardReversal()
{

}

void CDealCalIndexUpwardReversal::SetDeal(int nPos, double dValue)
{
	if(m_bSetFirst)
	{
		m_dValueLast = dValue;
		m_bSetFirst = FALSE;
		return;
	}
	if(m_bSetSecond)
	{
		m_dValueLastBefore = m_dValueLast;
		m_dValueLast = dValue;
		m_bSetSecond = FALSE;
		return;
	}
	if(m_dValueLastBefore>m_dValueLast&&m_dValueLast<dValue)
	{
		m_pDealDataItem->SetData(nPos,m_lDealValue);
	} 
	m_dValueLastBefore = m_dValueLast;
	m_dValueLast = dValue;
}