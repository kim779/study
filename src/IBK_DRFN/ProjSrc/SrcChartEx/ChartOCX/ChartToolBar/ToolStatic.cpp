// ToolStatic.cpp : implementation file
//

#include "stdafx.h"
#include "charttoolbar.h"
#include "ToolStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolStatic

CToolStatic::CToolStatic()
{
}

CToolStatic::~CToolStatic()
{
}


BEGIN_MESSAGE_MAP(CToolStatic, CStatic)
	//{{AFX_MSG_MAP(CToolStatic)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolStatic message handlers

void CToolStatic::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here

	CWnd *pParent = GetParent();			// Getting ToolBar DLG
	if( pParent)
	{
		pParent = pParent->GetParent();
		if( pParent) pParent->SetFocus();	// Getting Chart OCX
	}
}
