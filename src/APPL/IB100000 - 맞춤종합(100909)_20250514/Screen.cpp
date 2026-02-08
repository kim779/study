// Screen.cpp : implementation file
//

#include "stdafx.h"
#include "IB100000.h"
#include "Screen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen

CScreen::CScreen()
{
}

CScreen::~CScreen()
{
}


BEGIN_MESSAGE_MAP(CScreen, CWnd)
	//{{AFX_MSG_MAP(CScreen)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScreen message handlers

void CScreen::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	 cRc;

	GetClientRect(cRc);
//	dc.SetBkColor(GetSysColor(COLOR_BTNFACE));
//	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);

//	dc.FillSolidRect(cRc, 0x0000ff); 
}
