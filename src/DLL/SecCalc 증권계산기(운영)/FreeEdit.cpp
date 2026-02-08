// FreeEdit.cpp : implementation file
//

#include "stdafx.h"
#include "SecCalc.h"
#include "FreeEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFreeEdit

CFreeEdit::CFreeEdit()
{
	m_crText = RGB(0,0,0);
}

CFreeEdit::~CFreeEdit()
{
	if (m_brBack.GetSafeHandle())
		m_brBack.DeleteObject();
}


BEGIN_MESSAGE_MAP(CFreeEdit, CEdit)
	//{{AFX_MSG_MAP(CFreeEdit)
		ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFreeEdit message handlers
HBRUSH CFreeEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{	
	pDC->SetTextColor(m_crText);
	pDC->SetBkColor(m_crBack);
	
	//return the brush used for background this sets control background
	return m_brBack;
}

void CFreeEdit::SetBackColor(COLORREF rgb)
{
	//set background color ref (used for text's background)
	m_crBack = rgb;
	
	if (m_brBack.GetSafeHandle())
		m_brBack.DeleteObject();

	m_brBack.CreateSolidBrush(rgb);
	
	Invalidate(TRUE);
}

void CFreeEdit::SetTextColor(COLORREF rgb)
{
	m_crText = rgb;
	
	Invalidate(TRUE);
}

void CFreeEdit::OnSetFocus(CWnd* pOldWnd) 
{
	GetParent()->SendMessage(WM_CHILD, EDIT_FOCUS, GetDlgCtrlID());
	CEdit::OnSetFocus(pOldWnd);
	::DestroyCaret();
}

void CFreeEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	GetParent()->SendMessage(WM_CHILD, EDIT_FOCUS, GetDlgCtrlID());	
	//CEdit::OnLButtonDown(nFlags, point);
	if (GetFocus() != this)
		SetFocus();
}

void CFreeEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	GetParent()->SendMessage(WM_CHILD, EDIT_FOCUS, GetDlgCtrlID());	
	CEdit::OnLButtonUp(nFlags, point);
}

void CFreeEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	//CEdit::OnLButtonDblClk(nFlags, point);
}

void CFreeEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//CEdit::OnRButtonDown(nFlags, point);
}
