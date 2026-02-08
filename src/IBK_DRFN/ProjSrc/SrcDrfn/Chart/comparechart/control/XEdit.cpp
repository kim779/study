// XEdit.cpp : implementation file
//

#include "stdafx.h"
#include "XEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXEdit

CXEdit::CXEdit()
{
}

CXEdit::~CXEdit()
{
}


BEGIN_MESSAGE_MAP(CXEdit, CEdit)
	//{{AFX_MSG_MAP(CXEdit)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXEdit message handlers

void CXEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if ( VK_RETURN == nChar )
	{
		CString clstrText;
		GetWindowText( clstrText );
		GetParent()->SendMessage( WM_XLISTCTRL_EDIT_RETURN, (WPARAM)(LPCTSTR)clstrText );		
		return;
	}

	if ( VK_ESCAPE == nChar )
	{
		GetParent()->SendMessage( WM_XLISTCTRL_EDIT_RETURN, NULL, (LPARAM)"esc" );
		return;
	}		
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
