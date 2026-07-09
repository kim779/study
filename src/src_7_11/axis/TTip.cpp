// TTip.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "TTip.h"

#define TM_CHECKTIP	101
#define TMI_CHECKTIP	100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTip

CTTip::CTTip(CWnd* mainframe)
{
	m_mainframe = mainframe;
	m_font.CreatePointFont(90, "±¼¸²");
}

CTTip::~CTTip()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CTTip, CWnd)
	//{{AFX_MSG_MAP(CTTip)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTTip message handlers

void CTTip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	 cRc;

	GetClientRect(cRc);
	dc.SetBkColor(RGB(255, 255, 225));
	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);

	CFont*	font = dc.SelectObject(&m_font);
	cRc.OffsetRect(0, 1);
	dc.DrawText(m_tips, cRc, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	dc.SelectObject(font);
}

void CTTip::ShowTips(CRect tRc, CString tips)
{
	m_tips = tips;
	m_rect.CopyRect(&tRc);

	CDC*	pDC = GetDC();
	CFont*	font = pDC->SelectObject(&m_font);
	CSize	size = pDC->GetTextExtent(tips + "  ");

	pDC->SelectObject(font);
	ReleaseDC(pDC);

	CRect	dispRc;
	tRc.OffsetRect(0, tRc.Height()*2);
	dispRc.SetRect(tRc.left, tRc.top, tRc.left + size.cx, tRc.top + size.cy + 5);
	MoveWindow(dispRc);
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	Invalidate(FALSE);
	KillTimer(TM_CHECKTIP);
	SetTimer(TM_CHECKTIP, TMI_CHECKTIP, NULL);
}

void CTTip::HideTips()
{
	ShowWindow(SW_HIDE);

	KillTimer(TM_CHECKTIP);
	m_tips = _T("");
	m_rect.SetRectEmpty();
}

void CTTip::OnTimer(UINT_PTR nIDEvent) 
{
	if (nIDEvent == TM_CHECKTIP)
	{
		CRect	wRc, chkRc;
		CPoint	pt;

		m_mainframe->GetWindowRect(wRc);
		chkRc.CopyRect(&m_rect);
		chkRc.OffsetRect(-wRc.left, -wRc.top);

		GetCursorPos(&pt);
//		if (!chkRc.PtInRect(pt))

		CRect rc;
		rc = m_rect;
		rc.InflateRect(7, 7);

		if (!rc.PtInRect(pt))
			HideTips();
	}
	CWnd::OnTimer(nIDEvent);
}
