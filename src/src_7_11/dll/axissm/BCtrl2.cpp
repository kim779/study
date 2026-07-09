// BCtrl2.cpp : implementation file
//

#include "stdafx.h"
#include "BCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCtrl2

CBCtrl2::CBCtrl2(CString home)
{
	m_home = home;
	m_key = 0;
}

CBCtrl2::~CBCtrl2()
{
}


BEGIN_MESSAGE_MAP(CBCtrl2, CWnd)
	//{{AFX_MSG_MAP(CBCtrl2)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBCtrl2 message handlers

void CBCtrl2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect	 cRc;
	GetClientRect(cRc);
	dc.SetBkColor(RGB(255, 255, 255));
	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);//*/
}
