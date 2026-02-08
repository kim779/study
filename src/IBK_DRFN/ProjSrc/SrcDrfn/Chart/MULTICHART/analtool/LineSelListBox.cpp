// LineSelListBox.cpp : implementation file
//

#include "stdafx.h"
#include "LineSelListBox.h"
#include "ctrlNotify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineSelListBox

CLineSelListBox::CLineSelListBox()
{
}

CLineSelListBox::~CLineSelListBox()
{
}


BEGIN_MESSAGE_MAP(CLineSelListBox, CListBox)
	//{{AFX_MSG_MAP(CLineSelListBox)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelchange)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineSelListBox message handlers

void CLineSelListBox::DrawItem(LPDRAWITEMSTRUCT lpdis) 
{
	// TODO: Add your code to draw the specified item
	CDC dc;
	if(!dc.Attach(lpdis->hDC))
		return;

	CRect rcItem = lpdis->rcItem;
	UINT nItemIndex = lpdis->itemID;
	
	COLORREF clr = (lpdis->itemState & ODS_SELECTED) ? RGB(192, 0, 192) : ::GetSysColor(COLOR_WINDOW);
	dc.FillSolidRect(rcItem, clr);
	

//	if(lpdis->itemState & ODS_FOCUS)
//	{
//		dc.DrawFocusRect(rcItem);
//	}
	if(lpdis->itemState & ODS_SELECTED)
	{
		
	}
	if(nItemIndex != (UINT)-1)
	{
		int nThick = (int)lpdis->itemData;
		int y = (rcItem.Height() - nThick) / 2 + rcItem.top;
		CRect rcThick(rcItem.left + 4, y, rcItem.right - 4, y + nThick);
		dc.FillSolidRect(rcThick, lpdis->itemState & ODS_SELECTED ? RGB(255, 255, 255) : RGB(0,0,0));
	}

	dc.Detach();
	
}

void CLineSelListBox::MeasureItem(LPMEASUREITEMSTRUCT lpmis) 
{
	// TODO: Add your code to determine the size of specified item
	lpmis->itemHeight = 18;
}

BOOL CLineSelListBox::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LBS_OWNERDRAWFIXED;
	cs.style |= LBS_HASSTRINGS;
	
	return CListBox::PreCreateWindow(cs);
}

void CLineSelListBox::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	int nSel = GetCurSel();
	if(nSel == LB_ERR)
		return;

	NMHDR nmhdr; ::memset(&nmhdr, 0x00, sizeof(NMHDR));
	nmhdr.hwndFrom = m_hWnd;
	nmhdr.idFrom = CHART_LINE_THICKNESS;
	nmhdr.code = (UINT)GetItemData(nSel);
	
	CWnd* pWnd = AfxGetMainWnd();
	if(::IsWindow(pWnd->GetSafeHwnd()))
		pWnd->SendNotifyMessage(WM_NOTIFY, (WPARAM)0, (LPARAM)&nmhdr);

	GetParent()->ShowWindow(SW_HIDE);
}

void CLineSelListBox::OnKillFocus(CWnd* pNewWnd) 
{
	CListBox::OnKillFocus(pNewWnd);
	
	GetParent()->ShowWindow(SW_HIDE);
}
