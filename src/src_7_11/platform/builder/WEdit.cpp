// WEdit.cpp : implementation file
//

#include "stdafx.h"
#include "axiswork.h"
#include "WEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWEdit

CWEdit::CWEdit()
{
}

CWEdit::~CWEdit()
{
}


BEGIN_MESSAGE_MAP(CWEdit, CEdit)
	//{{AFX_MSG_MAP(CWEdit)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWEdit message handlers

void CWEdit::OnKillFocus(CWnd* pNewWnd) 
{
	ShowWindow(SW_HIDE);
	CEdit::OnKillFocus(pNewWnd);	
}
