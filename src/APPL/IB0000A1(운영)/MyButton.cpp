// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000A1.h"
#include "MyButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
	m_bTracking = FALSE;
}

CMyButton::~CMyButton()
{	
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

void CMyButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	
	ShowToolTip();
	CButton::OnMouseMove(nFlags, point);
}

void CMyButton::HideToolTip()
{
	GetParent()->SendMessage(WM_USER, MAKEWPARAM(GetDlgCtrlID(), 0));
}


void CMyButton::ShowToolTip()
{
	GetParent()->SendMessage(WM_USER, MAKEWPARAM(GetDlgCtrlID(), 1));	
}


LRESULT CMyButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	HideToolTip();
	return 0;
}
