// ListPopup.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000X1.h"
#include "ListPopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListPopup

CListPopup::CListPopup(CEdit *pEdit) : m_pEdit(pEdit)
{
}

CListPopup::~CListPopup()
{
}


BEGIN_MESSAGE_MAP(CListPopup, CWnd)
	//{{AFX_MSG_MAP(CListPopup)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WX_LBOX_KILLFOCUS, OnListBoxKillFocus)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CListPopup message handlers

void CListPopup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CListPopup::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
}

int CListPopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_listbox.Create(WS_VISIBLE|WS_CHILD|WS_HSCROLL|WS_VSCROLL, CRect(0,0,0,0), this, -1);
	
	return 0;
}

void CListPopup::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (m_listbox.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(&rc);
		m_listbox.MoveWindow(&rc, TRUE);
	}
}

void CListPopup::SetCodeList( CStringArray *pCodeArray, CStringArray *pNameArray )
{
	if (!m_listbox.GetSafeHwnd()) return;

	m_listbox.ResetContent();

	CString value;
	const int nSize = pCodeArray->GetSize();
	for(int n=0; n<nSize; ++n)
	{
		value.Format("%-10s%s", pCodeArray->ElementAt(n), pNameArray->ElementAt(n));
		m_listbox.AddString(value);
	}
	if (nSize>0)
	{
		m_listbox.SetCurSel(0);
	}
}

void CListPopup::SetFont( CFont *pFont )
{
	CWnd::SetFont(pFont, TRUE);
	m_listbox.SetFont(pFont);
}

void CListPopup::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
}

void CListPopup::SelectKey( UINT nChar )
{
	if (!m_listbox.GetSafeHwnd()) return;

	const int nSel = m_listbox.GetCurSel();
	const int nMax = m_listbox.GetCount()-1;
	if (nSel==-1) m_listbox.SetCurSel(0);
	else
	{
		switch(nChar)
		{
		case VK_UP:		m_listbox.SetCurSel(max(nSel- 1,    0));	break;
		case VK_DOWN:	m_listbox.SetCurSel(min(nSel+ 1, nMax));	break;
		case VK_PRIOR:	m_listbox.SetCurSel(max(nSel-10,    0));	break;
		case VK_NEXT:	m_listbox.SetCurSel(min(nSel+10, nMax));	break;
		}
	}
}

LRESULT CListPopup::OnListBoxKillFocus( WPARAM wParam, LPARAM lParam )
{
	BOOL bHide = FALSE;
	const CWnd *pNewWnd = (CWnd*)lParam;
	if (pNewWnd==NULL)
	{
		bHide = TRUE;
	}
	else
	{
		if (m_hWnd!=pNewWnd->m_hWnd && m_pEdit->m_hWnd!=pNewWnd->m_hWnd)
		{
			bHide = TRUE;
		}
	}
	if (bHide)
	{
		m_pEdit->ShowWindow(SW_HIDE);
		ShowWindow(SW_HIDE);
	}
	return 0;
}

void CListPopup::GetSelection( CString &code, CString &name )
{
	code = "";
	name = "";
	const int nSel = m_listbox.GetCurSel();
	if (nSel>=0)
	{
		CString tmp;
		m_listbox.GetText(nSel, tmp);
		code = tmp.Left(10); code.TrimRight();
		name = tmp.Mid(10);  name.TrimRight();

	}
}

void CListPopup::Select( UINT index )
{
	if (!m_listbox.GetSafeHwnd()) return;
	const int nMax = m_listbox.GetCount()-1;
	m_listbox.SetCurSel(min((UINT)nMax, index));
}
