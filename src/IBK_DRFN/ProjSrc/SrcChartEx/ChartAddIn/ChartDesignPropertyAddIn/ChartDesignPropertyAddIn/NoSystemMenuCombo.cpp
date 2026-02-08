// NoSystemMenuCombo.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "NoSystemMenuCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoSystemMenuCombo

CNoSystemMenuCombo::CNoSystemMenuCombo()
{
}

CNoSystemMenuCombo::~CNoSystemMenuCombo()
{
}


BEGIN_MESSAGE_MAP(CNoSystemMenuCombo, CComboBox)
	//{{AFX_MSG_MAP(CNoSystemMenuCombo)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoSystemMenuCombo message handlers

void CNoSystemMenuCombo::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	return;	
}
