// EditUsr.cpp : implementation file
//

#include "stdafx.h"
#include "EditUsr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditUsr

CEditUsr::CEditUsr()
{
}

CEditUsr::~CEditUsr()
{
}


BEGIN_MESSAGE_MAP(CEditUsr, CExEdit)
	//{{AFX_MSG_MAP(CEditUsr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditUsr message handlers
LRESULT CEditUsr::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message==WM_KEYUP&&wParam==VK_RETURN)
	{
		MSG Msg;
		Msg.hwnd = GetSafeHwnd();
		Msg.message = WM_KEYDOWN;
		Msg.wParam = wParam;
		GetParent()->PreTranslateMessage(&Msg);
		return TRUE;
	}
	return CExEdit::WindowProc(message, wParam, lParam);
}
