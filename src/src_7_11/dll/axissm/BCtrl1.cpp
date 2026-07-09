// BCtrl1.cpp : implementation file
//

#include "stdafx.h"
#include "BCtrl1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCtrl1

CBCtrl1::CBCtrl1(CString home, COLORREF bk)
{
	m_home = home;
	m_bk = bk;
}

CBCtrl1::~CBCtrl1()
{
}


BEGIN_MESSAGE_MAP(CBCtrl1, CWnd)
	//{{AFX_MSG_MAP(CBCtrl1)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCtrl1 message handlers

void CBCtrl1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect	 cRc;
	GetClientRect(cRc);
	dc.SetBkColor(m_bk);
	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);
}
