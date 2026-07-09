// AutoCmplWnd.cpp : implementation file
//

#include "stdafx.h"
#include "axiswork.h"
#include "AutoCmplWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoCmplWnd

CAutoCmplWnd::CAutoCmplWnd()
{
}

CAutoCmplWnd::~CAutoCmplWnd()
{
}


BEGIN_MESSAGE_MAP(CAutoCmplWnd, CWnd)
	//{{AFX_MSG_MAP(CAutoCmplWnd)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAutoCmplWnd message handlers

BOOL CAutoCmplWnd::Create(CWnd* pParentWnd)
{
	return CreateEx(WS_EX_TOOLWINDOW,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, 200, 100, pParentWnd->GetSafeHwnd(), NULL);
}

void CAutoCmplWnd::OnSetFocus(CWnd* pOldWnd) 
{
//	CWnd::OnSetFocus(pOldWnd);
	GetParent()->SetFocus();
}


BOOL CAutoCmplWnd::PreTranslateMessage(MSG* pMsg) 
{
	return CWnd::PreTranslateMessage(pMsg);
}
