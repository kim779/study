// ExToolTip.cpp : implementation file
//

#include "stdafx.h"
#include "ExToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExToolTip

CExToolTip::CExToolTip()
{
}

CExToolTip::~CExToolTip()
{
}


BEGIN_MESSAGE_MAP(CExToolTip, CToolTipCtrl)
	//{{AFX_MSG_MAP(CExToolTip)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//###########################################################################
// CExToolTip message handlers
//###########################################################################




//###########################################################################
// CExToolTip Member Functions
//###########################################################################
BOOL CExToolTip::AddWindowTool(CWnd * pWnd, LPCTSTR pszText)
{
	TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_IDISHWND|TTF_SUBCLASS;
	ti.hwnd = pWnd->GetParent()->GetSafeHwnd();
	ti.uId = (UINT)pWnd->GetSafeHwnd();
	ti.hinst = AfxGetInstanceHandle();
	ti.lpszText = (LPSTR)pszText;

	return (BOOL)SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti);
}

BOOL CExToolTip::AddRectTool(CWnd * pWnd, LPCTSTR pszText, LPRECT lpRect, UINT nIDTool)
{
	TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS;
	ti.hwnd = pWnd->GetSafeHwnd();
	ti.uId = nIDTool;
	ti.hinst = AfxGetInstanceHandle();
	ti.lpszText = (LPSTR)pszText;
	::CopyRect(&ti.rect, lpRect);

	return (BOOL)SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti);
}
