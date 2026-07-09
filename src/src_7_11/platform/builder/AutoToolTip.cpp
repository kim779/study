// AutoToolTip.cpp : implementation file
//

#include "stdafx.h"
#include "axiswork.h"
#include "AutoToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoToolTip dialog


CAutoToolTip::CAutoToolTip(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoToolTip::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoToolTip)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}


void CAutoToolTip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoToolTip)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoToolTip, CDialog)
	//{{AFX_MSG_MAP(CAutoToolTip)
	ON_WM_MOUSEACTIVATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoToolTip message handlers

int CAutoToolTip::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return MA_NOACTIVATE;
	
	return CDialog::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

LRESULT CAutoToolTip::OnMessage(WPARAM wParam, LPARAM lParam)
{
	m_pParent->PostMessage(WM_KEYDOWN, wParam, 0);
	return 0;
}

void CAutoToolTip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CString sText;
	CRect rc;
	GetClientRect(&rc);

	GetWindowText(sText);
	dc.FillSolidRect(rc, RGB(0, 0, 0));
	rc.DeflateRect(1, 1, 1, 1);
	dc.FillSolidRect(rc, RGB(249, 254, 210));
	dc.DrawText(sText, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	// Do not call CDialog::OnPaint() for painting messages
}

int CAutoToolTip::getWidth()
{
	CString sText;
	CDC *pDC = GetDC();
	long nWidth = 0;

	GetWindowText(sText);
	nWidth = pDC->GetTextExtent(sText).cx;
	ReleaseDC(pDC);

	return nWidth;

}
