// NoSystemMenuEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"

#include "NoSystemMenuEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoSystemMenuEdit

CNoSystemMenuEdit::CNoSystemMenuEdit()
{
}

CNoSystemMenuEdit::~CNoSystemMenuEdit()
{
}


BEGIN_MESSAGE_MAP(CNoSystemMenuEdit, CEdit)
	//{{AFX_MSG_MAP(CNoSystemMenuEdit)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoSystemMenuEdit message handlers

void CNoSystemMenuEdit::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	return;	
}
