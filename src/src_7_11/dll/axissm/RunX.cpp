// RunX.cpp : implementation file
//

#include "stdafx.h"
#include "RunX.h"
#include "nbutton.h"
#include "resource.h"

#include "../../h/axisvar.h"
#include "../../axis/axmsg.hxx"

#define	GAP		4
#define	CTRL_H		20
#define	EDIT_SPIN	201

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunX

CRunX::CRunX(CWnd* parent, CString home, COLORREF bk)
{
	m_parent = parent;
	m_home = home;
	m_bk = bk;
	m_code = true;
	m_auto = false;
	m_interval = 3;

	m_play = NULL;
	m_pause = NULL;
	m_continue = NULL;
	m_stop = NULL;
	m_prev = NULL;
	m_next = NULL;

	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);
}

CRunX::~CRunX()
{
	if (m_play)	delete m_play;
	if (m_pause)	delete m_pause;
	if (m_continue)	delete m_continue;
	if (m_stop)	delete m_stop;
	if (m_prev)	delete m_prev;
	if (m_next)	delete m_next;

	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CRunX, CWnd)
	//{{AFX_MSG_MAP(CRunX)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
	ON_NOTIFY(UDN_DELTAPOS, EDIT_SPIN, OnDeltaposSpincontrol)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRunX message handlers

void CRunX::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	client;
	GetClientRect(client);

	COLORREF color = dc.SetBkColor(m_bk);
	dc.ExtTextOut(client.left, client.top, ETO_OPAQUE, client, "", 0, NULL);
	dc.SetBkColor(color);	

	draw_Form(&dc);
}

int CRunX::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!make_FB())
		return -1;
	show_Button();
	return 0;
}

bool CRunX::make_FB()
{
	DWORD	style = WS_CHILD|WS_CHILD|WS_VISIBLE|BS_GROUPBOX|BS_LEFT|BS_RIGHT;
	if (!m_group1.Create("", style, CRect(0), this, 1))
		return false;
	if (!m_group2.Create("", style, CRect(0), this, 2))
		return false;
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_TABSTOP
			|WS_BORDER|ES_NUMBER|ES_CENTER|ES_WANTRETURN;
	if (!m_edit.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), "3", dwStyle, CRect(0), this, 200))
		return false;
	if (!m_spin.Create(WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(0), this, EDIT_SPIN))
		return false;
	m_spin.SetBuddy(&m_edit);

	m_group1.SetFont(&m_font);
	m_group2.SetFont(&m_font);
	m_edit.SetFont(&m_font);
	m_spin.SetFont(&m_font);

	CString	imgC, imgO, imgD, tipS;
	dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	m_play = new CNButton(CTRL_PLAY);
	if (!m_play->Create(NULL, "start", dwStyle, CRect(0), this, CTRL_PLAY))
	{
		delete m_play;
		m_play = NULL;
	}
	else
	{
		tipS.LoadString(ST_TEXT_RUNXPLAY);
		m_play->set_ToolTipText(tipS);
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "play");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "play_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "play_dn");
		m_play->set_Image(imgC, imgO, imgD);
	}

	m_pause = new CNButton(CTRL_PAUSE);
	if (!m_pause->Create(NULL, "pause", dwStyle, CRect(0), this, CTRL_PAUSE))
	{
		delete m_pause;
		m_pause = NULL;
	}
	else
	{
		tipS.LoadString(ST_TEXT_RUNXPAUSE);
		m_pause->set_ToolTipText(tipS);
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "pause");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "pause_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "pause_dn");
		m_pause->set_Image(imgC, imgO, imgD);
	}

	m_continue = new CNButton(CTRL_CONTINUE);
	if (!m_continue->Create(NULL, "continue", dwStyle, CRect(0), this, CTRL_CONTINUE))
	{
		delete m_continue;
		m_continue = NULL;
	}
	else
	{
		tipS.LoadString(ST_TEXT_RUNXCONTINUE);
		m_continue->set_ToolTipText(tipS);
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "continue");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "continue_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "continue_dn");
		m_continue->set_Image(imgC, imgO, imgD);
	}

	m_stop = new CNButton(CTRL_STOP);
	if (!m_stop->Create(NULL, "stop", dwStyle, CRect(0), this, CTRL_STOP))
	{
		delete m_stop;
		m_stop = NULL;
	}
	else
	{
		tipS.LoadString(ST_TEXT_RUNXSTOP);
		m_stop->set_ToolTipText(tipS);
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "stop");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "stop_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "stop_dn");
		m_stop->set_Image(imgC, imgO, imgD);
	}

	m_prev = new CNButton(CTRL_PREV);
	if (!m_prev->Create(NULL, "prev", dwStyle, CRect(0), this, CTRL_PREV))
	{
		delete m_prev;
		m_prev = NULL;
	}
	else
	{
		tipS.LoadString(ST_TEXT_RUNXPREV);
		m_prev->set_ToolTipText(tipS);
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "이전");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "이전_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "이전_dn");
		m_prev->set_Image(imgC, imgO, imgD);
	}

	m_next = new CNButton(CTRL_NEXT);
	if (!m_next->Create(NULL, "next", dwStyle, CRect(0), this, CTRL_NEXT))
	{
		delete m_next;
		m_next = NULL;
	}
	else
	{
		tipS.LoadString(ST_TEXT_RUNXNEXT);
		m_next->set_ToolTipText(tipS);
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "다음");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "다음_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "다음_dn");
		m_next->set_Image(imgC, imgO, imgD);
	}

	return true;
}

void CRunX::draw_Form(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	int	cnt = 3;
	UINT	nState;
	CRect	cRc, rRc, fRc, tRc, xRc;
	CString	text;

	GetClientRect(cRc);
	pDC->SetBkMode(TRANSPARENT);
	CFont* font = pDC->SelectObject(&m_font);

	rRc.SetRect(GAP*2, GAP*cnt, cRc.Width() - GAP*2, GAP*cnt + CTRL_H);
	fRc.SetRect(rRc.left, rRc.top, rRc.left + rRc.Height(), rRc.bottom);
	fRc.DeflateRect(GAP, GAP);
	if (m_code)	nState = DFCS_BUTTONRADIO|DFCS_CHECKED;
	else		nState = DFCS_BUTTONRADIO;
	pDC->DrawFrameControl(&fRc, DFC_BUTTON, nState);

	tRc.SetRect(fRc.right, rRc.top, GAP*2 + rRc.Width()/2, rRc.bottom);
	text.LoadString(ST_TEXT_RUNX_ACTCODE);
	pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	m_rc[0].SetRect(fRc.left, tRc.top, tRc.right, tRc.bottom);

	fRc.SetRect(m_rc[0].right, m_rc[0].top, m_rc[0].right + rRc.Height(), m_rc[0].bottom);
	fRc.DeflateRect(GAP, GAP);
	if (m_code)	nState = DFCS_BUTTONRADIO;
	else		nState = DFCS_BUTTONRADIO|DFCS_CHECKED;
	pDC->DrawFrameControl(&fRc, DFC_BUTTON, nState);
	tRc.SetRect(fRc.right, rRc.top, rRc.right, rRc.bottom);
	text.LoadString(ST_TEXT_RUNX_ACTMAP);
	pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	m_rc[1].SetRect(fRc.left, tRc.top, tRc.right, tRc.bottom);
	
	rRc.OffsetRect(0, CTRL_H*2);
	xRc.OffsetRect(0, CTRL_H*2);
	
	fRc.SetRect(rRc.left, rRc.top, rRc.left + rRc.Height(), rRc.bottom);
	fRc.DeflateRect(GAP, GAP);
	if (m_auto)	nState = DFCS_BUTTONRADIO;
	else		nState = DFCS_BUTTONRADIO|DFCS_CHECKED;
	pDC->DrawFrameControl(&fRc, DFC_BUTTON, nState);

	text.LoadString(ST_TEXT_RUNX_MANUAL);
	CSize size = pDC->GetTextExtent(text);
	tRc.SetRect(fRc.right, rRc.top, fRc.right + size.cx, rRc.bottom);
	pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	m_rc[2].SetRect(fRc.left, tRc.top, tRc.right, tRc.bottom);

	fRc.SetRect(m_rc[2].right, m_rc[2].top, m_rc[2].right + rRc.Height(), m_rc[2].bottom);
	fRc.OffsetRect(GAP, 0);	fRc.DeflateRect(GAP, GAP);
	if (m_auto)	nState = DFCS_BUTTONRADIO|DFCS_CHECKED;
	else		nState = DFCS_BUTTONRADIO;
	pDC->DrawFrameControl(&fRc, DFC_BUTTON, nState);

	text.LoadString(ST_TEXT_RUNX_AUTO);
	size = pDC->GetTextExtent(text);
	tRc.SetRect(fRc.right, rRc.top, fRc.right + size.cx, rRc.bottom);
	pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	m_rc[3].SetRect(fRc.left, tRc.top, tRc.right, tRc.bottom);

	pDC->SelectObject(font);
}

void CRunX::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if (cx <= 0)	return;

	CRect	cRc, gRc, eRc;
	GetClientRect(cRc);
	if (m_group1.GetSafeHwnd())
	{
		gRc.SetRect(GAP, 0, cRc.Width() - GAP + 1, CTRL_H*2);
		m_group1.MoveWindow(gRc);
		if (m_group2.GetSafeHwnd())
		{
			gRc.OffsetRect(0, CTRL_H*2);
			m_group2.MoveWindow(gRc);
		}
	}

	if (m_edit.GetSafeHwnd())
	{
		eRc.SetRect(GAP*2, GAP*3, cRc.Width() - GAP*2, GAP*3 + CTRL_H);
		eRc.OffsetRect(0, CTRL_H*2);
		cRc.SetRect(eRc.left + eRc.Width()/2, eRc.top,
			eRc.left + eRc.Width()/2 + GAP*10, eRc.bottom);
		cRc.OffsetRect(GAP*5, 0);
		m_edit.SetWindowPos(&wndTop, cRc.left, cRc.top,
			cRc.Width(), cRc.Height(), SWP_SHOWWINDOW);
		if (m_spin.GetSafeHwnd())
		{
			cRc.SetRect(cRc.right + 2, cRc.top, cRc.right + 20, cRc.bottom);
			m_spin.SetWindowPos(&wndTop, cRc.left, cRc.top,
			cRc.Width(), cRc.Height(), SWP_SHOWWINDOW);
		}
	}

	const int w = 18;
	if (m_play)
	{
		CRect	rc;
		GetClientRect(cRc);
		CSize size = m_play->get_ImgSize();
		
		int cy = gRc.bottom + (cRc.bottom - gRc.bottom - size.cy)/2;
		int offset = size.cx + 5;
		int move = (cRc.Width() - offset*4)/2;
		rc.SetRect(0, cy, size.cx, cy + size.cy);

		rc.OffsetRect(move, 0);		m_play->MoveWindow(rc);
		rc.OffsetRect(offset, 0);	
		m_pause->MoveWindow(rc);	m_prev->MoveWindow(rc);
		rc.OffsetRect(offset, 0);	
		m_continue->MoveWindow(rc);	m_next->MoveWindow(rc);
		rc.OffsetRect(offset, 0);	m_stop->MoveWindow(rc);
	}
}
	
void CRunX::OnLButtonDown(UINT nFlags, CPoint point) 
{
	for (int ii = 0; ii < 4; ii++)
	{
		if (m_rc[ii].PtInRect(point))
		{
			switch (ii)
			{
			case 0:	m_code = true;	break;
			case 1:	m_code = false;	break;
			case 2:	
				m_auto = false;
				show_Button();	break;
			case 3:	m_auto = true;
				show_Button();	break;
			}
			draw_Form();
			break;
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CRunX::OnDeltaposSpincontrol(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString string;
	m_edit.GetWindowText(string);
	m_interval = atoi(string);
	if (pNMUpDown->iDelta > 0)
		m_interval -= 1;
	else	m_interval += 1;
	if (m_interval < 3) m_interval = 3;	

	string.Format("%d", m_interval);
	m_edit.SetWindowText(string);
	*pResult = 0;
}

LRESULT CRunX::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_PLAY:	case CTRL_PAUSE:case CTRL_CONTINUE:
	case CTRL_STOP:	case CTRL_PREV:	case CTRL_NEXT:
		m_parent->SendMessage(WM_XMSG, wParam, lParam);
		break;
	default:
		break;
	}
	return 0;
}

void CRunX::show_Button()
{
	if (m_auto)
	{
		m_play->ShowWindow(SW_SHOW);
		m_pause->ShowWindow(SW_SHOW);
		m_continue->ShowWindow(SW_SHOW);
		m_stop->ShowWindow(SW_SHOW);

		m_play->EnableWindow(TRUE);
		m_pause->EnableWindow(FALSE);
		m_continue->EnableWindow(FALSE);
		m_stop->EnableWindow(FALSE);

		m_prev->ShowWindow(SW_HIDE);
		m_next->ShowWindow(SW_HIDE);

		m_edit.EnableWindow(TRUE);
		m_spin.EnableWindow(TRUE);

		m_edit.SetFocus();
		m_edit.SetSel(0, -1);
	}
	else
	{
		m_play->ShowWindow(SW_HIDE);
		m_pause->ShowWindow(SW_HIDE);
		m_continue->ShowWindow(SW_HIDE);
		m_stop->ShowWindow(SW_HIDE);

		m_prev->ShowWindow(SW_SHOW);
		m_next->ShowWindow(SW_SHOW);

		m_edit.EnableWindow(FALSE);
		m_spin.EnableWindow(FALSE);
	}
}

int CRunX::GetOrder()
{
	if (m_code)
		return ORDER_CODE;
	else	return ORDER_MAPN;
}

int CRunX::GetKind()
{
	if (m_auto)
		return KIND_AUTO;
	else	return KIND_MANUAL;
}

int CRunX::GetInterval()
{
	return m_interval;
}

void CRunX::EnableButton(int nID, BOOL bEnable)
{
	switch (nID)
	{
	case CTRL_PLAY:		m_play->EnableWindow(bEnable);		break;
	case CTRL_PAUSE:	m_pause->EnableWindow(bEnable);		break;
	case CTRL_CONTINUE:	m_continue->EnableWindow(bEnable);	break;
	case CTRL_STOP:		m_stop->EnableWindow(bEnable);		break;
	case CTRL_PREV:		m_prev->EnableWindow(bEnable);		break;
	case CTRL_NEXT:		m_next->EnableWindow(bEnable);		break;
	default:return;
	}
}

void CRunX::SetKind()
{
	m_auto = true;
	show_Button();
}