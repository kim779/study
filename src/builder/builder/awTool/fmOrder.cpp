
#include "stdafx.h"
#include "../ChildView.h"
#include "OrderWnd.h"
#include "fmOrder.h"
#include "../h/formOb.h"
#include "awObjectLoad.h"
#include "OrderWnd.h"

CfmOrder::CfmOrder(CChildView* pView, CObArray* pAryProp)
{
	m_pView = pView;
	m_pAryProp = pAryProp;
	m_curIdx = 1;
	m_dwOrderKind = 0;
}

CfmOrder::~CfmOrder()
{
	for (int ii = 0; ii < m_AryWnd.GetCount(); ii++)
	{
		COrderWnd* pOrderWnd;
		pOrderWnd = (COrderWnd*)m_AryWnd.GetAt(ii);
		pOrderWnd->DestroyWindow();
		delete pOrderWnd;
	}
	m_AryWnd.RemoveAll();
}

void CfmOrder::DrawOrderNumber(DWORD dwOrderKind)
{
	m_dwOrderKind = dwOrderKind;
	int *Order;	
	CRect rc;
	CformProp* Prop = NULL;
	for (int ii = 0; ii < m_pAryProp->GetCount(); ii++)
	{
		Prop = ((CObjectLoad*)m_pAryProp->GetAt(ii))->getProp();
		Order = (m_dwOrderKind == 1) ? (int*)&Prop->index : (int*)&Prop->iorder;
		if(*Order !=0)
		{
			rc.SetRect(Prop->rect.left,Prop->rect.top,Prop->rect.left+20,Prop->rect.top+20);
			COrderWnd* pWnd = new COrderWnd(Prop, dwOrderKind);
			pWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rc, m_pView->GetParent(), -1);
			pWnd->ShowWindow(SW_SHOW);
			m_AryWnd.Add(pWnd);
		}
	}
}

bool CfmOrder::CheckOrder(CPoint point)
{
	bool	bSort = false;
	int	iSelF = -1;
	int	OldIdx = -1;
	bool	bSame = false;
	int* Order;
	CformProp* Prop = NULL;
	for (int ii = m_pAryProp->GetCount()-1; ii >= 0 ; ii--)
	{
		Prop = ((CObjectLoad*)m_pAryProp->GetAt(ii))->getProp();
		Order = (m_dwOrderKind == 1) ? (int*)&Prop->index : (int*)&Prop->iorder;
		if(*Order !=0)
		{
			if(Prop->rect.PtInRect(point))
			{
				bSort = true;
				if(*Order == m_curIdx)
				{
					if( m_curIdx < m_AryWnd.GetCount() ) m_curIdx++;
					bSame = true;
					break;
				}
				OldIdx = *Order;
				*Order = m_curIdx;
				iSelF = ii;
				break;
			}
		}
	}
	if(bSort && bSame == false)
	{
		SortOrder(iSelF, OldIdx);
	}
	return bSort;
}

void CfmOrder::SortOrder(int iSelF, int iOldIdx)
{
	int max,min;
	max = max(m_curIdx,iOldIdx);
	min = min(m_curIdx,iOldIdx);
	int* Order;

	CformProp* Prop = NULL;
	for (int ii = m_pAryProp->GetCount()-1; ii >= 0 ; ii--)
	{
		Prop = ((CObjectLoad*)m_pAryProp->GetAt(ii))->getProp();
		if(ii == iSelF) continue;

		Order = (m_dwOrderKind == 1) ? (int*)&Prop->index : (int*)&Prop->iorder;
		if(*Order >= min && *Order <= max)
		{
			if(iOldIdx < m_curIdx)
				(*Order)--;
			else if(iOldIdx > m_curIdx)
				(*Order)++;
		}
	}

	COrderWnd* pWnd = NULL;
	for (int ii = 0; ii < m_AryWnd.GetCount(); ii++)
	{
		pWnd = (COrderWnd*)m_AryWnd.GetAt(ii);
		pWnd->Invalidate();
	}

	if( m_curIdx < m_AryWnd.GetCount() ) m_curIdx++;
}