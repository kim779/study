#include "stdafx.h"
#include "PCtrMainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const UINT RMSG_INTERFACE = ::RegisterWindowMessage("RMSG_INTERFACE");
const UINT RMSG_GETPCTRDATA = ::RegisterWindowMessage("RMSG_GETPCTRDATA");
const UINT RMSG_SETPCTRDATA = ::RegisterWindowMessage("RMSG_SETPCTRDATA");

/////////////////////////////////////////////////////////////////////////////
// CPCtrMainWnd

CPCtrMainWnd::CPCtrMainWnd()
{
}

BEGIN_MESSAGE_MAP(CPCtrMainWnd, CWnd)
	//{{AFX_MSG_MAP(CPCtrMainWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPCtrMainWnd::OpenWindow()
{
	CString		szClassName;	

	szClassName =	AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		0, 0, 0);

	if (!CWnd::CreateEx(WS_EX_TOPMOST, szClassName, _T(""), 
		WS_POPUP, 
		0, 0, 0, 0, // size & position updated when needed
		NULL, 0, NULL))
	{
		return FALSE;
	}
	return TRUE;
}

LRESULT CPCtrMainWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == RMSG_INTERFACE)
	{
		return (LRESULT)GetInferface(wParam, lParam);
	}
	else if (message == RMSG_GETPCTRDATA)
	{
		return (LRESULT)GetPCTrData(wParam, lParam);
	}
	else if (message == RMSG_SETPCTRDATA)
	{
		return (LRESULT)SetPCTrData(wParam, lParam);
	}

	return CWnd::WindowProc(message, wParam, lParam);
}
