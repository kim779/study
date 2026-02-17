// cbList.cpp : implementation file
//

#include "stdafx.h"
#include "cbList.h"
#include "fmbase.h"
#include "axform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CcbList

CcbList::CcbList(CfmBase* form, CStringArray& items, CFont* font, int height, CRect oRc)
{
	m_form  = form;
	m_font  = font;
	m_oRc   = oRc;
	m_items.Copy(items);

	m_tRGB = PALETTERGB(0, 0, 0);
	m_pRGB = PALETTERGB(255, 255, 255);
	m_height = height;
}

CcbList::~CcbList()
{
}


BEGIN_MESSAGE_MAP(CcbList, CListBox)
	//{{AFX_MSG_MAP(CcbList)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CcbList message handlers

BOOL CcbList::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= (WS_BORDER|WS_VSCROLL|LBS_OWNERDRAWFIXED|LBS_HASSTRINGS|LBS_NOTIFY);
	return CListBox::PreCreateWindow(cs);
}

int CcbList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style |= (LBS_OWNERDRAWFIXED|LBS_HASSTRINGS|LBS_NOTIFY);
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_items.GetSize() <= 0)
		return -1;

	SetFont(m_font);

	for (int ii = 0; ii < m_items.GetSize(); ii++)
		AddString(m_items.GetAt(ii));
	SetCurSel(0);
	SetCaretIndex(0);
	SetFocus();

	SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	return 0;
}

void CcbList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
// updateXXX_2022XX	
	case VK_TAB:
		m_form->DoListBox(false);
		m_form->m_axform->m_view->SetFocus();
		m_form->m_axform->m_view->PostMessage(WM_KEYDOWN, (WPARAM)VK_TAB, 0);
		break;
	case VK_RETURN:
		m_form->DoListBox(true);
		break;
	case VK_ESCAPE:
		m_form->DoListBox(false);
		break;
	}
}

void CcbList::OnKillFocus(CWnd* pNewWnd) 
{
	CListBox::OnKillFocus(pNewWnd);

	POINT	pt{ 0, 0 };
	::GetCursorPos(&pt);
	GetParent()->ScreenToClient(&pt);
	if (!m_oRc.PtInRect(pt))
		m_form->DoListBox(false);
}

void CcbList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListBox::OnLButtonDown(nFlags, point);

	BOOL out = FALSE;
	ItemFromPoint(point, out);
	m_form->DoListBox(out ? false : true);
}

void CcbList::OnMouseMove(UINT nFlags, CPoint point) 
{
	CListBox::OnMouseMove(nFlags, point);

	BOOL	out = FALSE;
	int	idx = ItemFromPoint(point, out);
	if (!out && idx != GetCurSel())
	{
		SetCurSel(idx);
		SetCaretIndex(idx);
	}
}

void CcbList::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int	result = 0;

	m_form->UpdateData(nChar, false, result);
}

void CcbList::DrawItemX(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC	dc;

	if (!dc.Attach(lpDrawItemStruct->hDC))
		return;

	CFont* font = dc.SelectObject(m_font);
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		dc.SetTextColor(m_tRGB);
		dc.SetBkColor(m_pRGB);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, m_pRGB);
	}

	if (lpDrawItemStruct->itemID != -1)
	{
		CString	text;

		CRect rc = lpDrawItemStruct->rcItem;
		GetText(lpDrawItemStruct->itemID, text);

		dc.SetBkMode(TRANSPARENT);
		rc.left += 3;
		text.Replace("&", "&&");
		dc.DrawText(text, rc, DT_VCENTER|DT_SINGLELINE); 
	}

	dc.SelectObject(font);
	dc.Detach();
}

void CcbList::MeasureItemX(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = m_height;
}
