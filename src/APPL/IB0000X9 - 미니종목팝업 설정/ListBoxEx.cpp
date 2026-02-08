// ListBoxEx.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000X1.h"
#include "ListBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListBoxEx

CListBoxEx::CListBoxEx()
{
}

CListBoxEx::~CListBoxEx()
{
}


BEGIN_MESSAGE_MAP(CListBoxEx, CListBox)
	//{{AFX_MSG_MAP(CListBoxEx)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxEx message handlers

void CListBoxEx::OnKillFocus(CWnd* pNewWnd) 
{
	CListBox::OnKillFocus(pNewWnd);
	GetParent()->SendMessage(WX_LBOX_KILLFOCUS, (WPARAM)0, (LPARAM)pNewWnd);
}