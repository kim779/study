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

CcbList::CcbList(CfmBase* form, CStringArray& items, CFont* font)
{
	m_form = form;
	m_font = font;
	m_items.Copy(items);
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
	cs.style |= WS_BORDER|WS_VSCROLL;
	return CListBox::PreCreateWindow(cs);
}

int CcbList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
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
	m_form->DoListBox(false);
}

void CcbList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListBox::OnLButtonDown(nFlags, point);

	BOOL	out;
	ItemFromPoint(point, out);
	m_form->DoListBox(out ? false : true);
}

void CcbList::OnMouseMove(UINT nFlags, CPoint point) 
{
	CListBox::OnMouseMove(nFlags, point);

	BOOL	out;
	int	idx = ItemFromPoint(point, out);
	if (!out && idx != GetCurSel())
	{
		SetCurSel(idx);
		SetCaretIndex(idx);
	}
}

void CcbList::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int	result;

//	CListBox::OnChar(nChar, nRepCnt, nFlags);
	m_form->UpdateData(nChar, false, result);
}
