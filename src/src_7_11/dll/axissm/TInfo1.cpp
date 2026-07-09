// TInfo1.cpp : implementation file
//

#include "stdafx.h"
#include "TInfo1.h"
#include "SimpleSplitter.h"
#include "Pane.h"
#include "resource.h"
#include "smdefine.h"
#include "nbutton.h"
#include "../../axis/axmsg.hxx"

#define	WGAP		2
#define MINPANE		20

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTInfo1

CTInfo1::CTInfo1(bool (*callback)(int, WPARAM, LPARAM), CString home, CString userN)
{
	m_axiscall	= callback;
	m_home		= home;
	m_userN		= userN;
	m_height	= TINFO_HEIGHT;
	m_base		= NULL;
	m_deal		= NULL;
	m_screen	= NULL;
	m_hide		= NULL;
	
	m_bkIMG.LoadBitmap(IDB_INFOBACK);
}

CTInfo1::~CTInfo1()
{
	if (m_base)	delete m_base;
	if (m_screen)	delete m_screen;
	if (m_deal)	delete m_deal;
	if (m_hide)	delete m_hide;

	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
		delete m_arPane.GetNext(pos);

	m_bkIMG.DeleteObject();
}


BEGIN_MESSAGE_MAP(CTInfo1, CDialogBar)
	//{{AFX_MSG_MAP(CTInfo1)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTInfo1 message handlers

void CTInfo1::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

CSize CTInfo1::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	CSize	size;
	CRect	fRc;

	size.cy = m_height;
	size.cy += (rect.Height());

	if (IsFloating())
	{
		GetDockingFrame()->GetParent()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}
	else
	{
		GetDockingFrame()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}

	return size;
}

void CTInfo1::OnDestroy() 
{
	CString	file, value, tmps;
	CRect	paneRc;
	int ii = 0;

	CArray <int, int > ary;

	file.Format("%s\\user\\%s\\%s", m_home, m_userN, TICKSETUP);
	for (ii = 0; ii < m_arPane.GetCount(); ii++)
	{
		m_base->GetPaneRect(ii, paneRc);
		ary.Add(paneRc.Width());
	}

	for (ii = 0; ii < ary.GetSize(); ii++)
	{
		if (!ii)
			value.Format("%d", ary.GetAt(ii));
		else
		{
			tmps.Format(", %d", ary.GetAt(ii));
			value += tmps;
		}
	}
	WritePrivateProfileString("GENERAL", "BAR00SIZE", value, file);

	CDialogBar::OnDestroy();
}

void CTInfo1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	 cRc;

	GetClientRect(cRc);
	COLORREF color = dc.SetBkColor(COLOR_FRAME);
	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);
	dc.SetBkColor(color);
}

void CTInfo1::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);
	ResizeBase();
}

void CTInfo1::drawBackground(CDC* pDC)
{
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(&m_bkIMG);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			BITMAP	 bm;
			m_bkIMG.GetBitmap(&bm);
			pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), 
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CTInfo1::DrawGripper(CDC* pDC)
{
	CRect	gRc = GetGRect();
	pDC->Draw3dRect(gRc, GRIP_LT, GRIP_RB);
}

void CTInfo1::Config(int configN, COLORREF bkColor)
{
	int ii = 0;
	if (m_base || !(this && GetSafeHwnd()))
		return;

	CArray	< int, int > ary;
	m_base = new CSimpleSplitter(configN, SSP_HORZ, MINPANE, SPLITWIDTH);
	m_base->Create(this, COLOR_FRAME);
	GetPaneSize(configN, ary);

	int*	size = new int[configN];
	for (ii = 0; ii < configN; ii++)
		size[ii] = ary.GetAt(ii);
	m_base->SetPaneSizes(size);
	ary.RemoveAll();

	for (ii = 0; ii < configN; ii++)
	{
		CPane*	pane = new CPane(m_home, m_userN, m_axiscall, &m_bkIMG);
		if (!m_base->CreatePane(ii, pane, 0, 0/*WS_EX_CLIENTEDGE*/, NULL))
		{
			delete pane;
			continue;
		}
		pane->SetPaneInfo(ii);
		m_arPane.AddTail(pane);
	}

	createButton();
	delete []size;
	ResizeBase();
}

void CTInfo1::ReConfig(int configN)
{
//	if (configN == m_arPane.GetCount())
//		return;

	int ii = 0;
	CArray	< int, int > ary;
	if (m_base)
	{
		CRect	paneRc;
		for (int ii = 0; ii < configN; ii++)
		{
			if (ii < m_arPane.GetCount())
				m_base->GetPaneRect(ii, paneRc);
			ary.Add(paneRc.Width());
		}
		m_base->DestroyWindow();
		delete m_base;
		for (POSITION pos = m_arPane.GetHeadPosition(); pos;)
			delete m_arPane.GetNext(pos);
		m_arPane.RemoveAll();
		m_base = NULL;
	}

	if (!(this && GetSafeHwnd()))	return;
	m_base = new CSimpleSplitter(configN, SSP_HORZ, MINPANE, SPLITWIDTH);
	m_base->Create(this, COLOR_FRAME);

	int*	size = new int[configN];
	for (ii = 0; ii < configN; ii++)
		size[ii] = ary.GetAt(ii);
	m_base->SetPaneSizes(size);
	ary.RemoveAll();

	for (ii = 0; ii < configN; ii++)
	{
		CPane*	pane = new CPane(m_home, m_userN, m_axiscall, &m_bkIMG);
		if (!m_base->CreatePane(ii, pane, 0, 0/*WS_EX_CLIENTEDGE*/, NULL))
		{
			delete pane;
			continue;
		}
		pane->SetPaneInfo(ii);
		m_arPane.AddTail(pane);
	}

	delete []size;
	ResizeBase();
}

void CTInfo1::ResizeBase()
{
	CRect	cRc;
	GetClientRect(cRc);
	if (m_base && m_base->GetSafeHwnd())
	{
		CRect	gRc(GetGRect());
		m_hide->MoveWindow(gRc);

		cRc.DeflateRect(gRc.right + WGAP*2, HORZFIT_TOP, WGAP, HORZFIT_BOTTOM);
		cRc.right -= (BUTTONWIDTH1 + BUTTONWIDTH2 + WGAP);
		m_base->MoveWindow(cRc);

		cRc.left = cRc.right + WGAP;
		cRc.right = cRc.left + BUTTONWIDTH1;
		m_deal->MoveWindow(cRc);

		cRc.left = cRc.right;
		cRc.right = cRc.left + BUTTONWIDTH2;
		m_screen->MoveWindow(cRc);
	}
}

CRect CTInfo1::GetGRect()
{
	CRect pRect;
	GetClientRect(pRect);
	pRect.OffsetRect(-pRect.left,-pRect.top);

	pRect.DeflateRect(3, 1);
	pRect.right = pRect.left + 9;
	return pRect;
}

void CTInfo1::GetPaneSize(int configN, CArray <int, int> &ary)
{
	CString	file, tmpS, value;
	char	buf[512];

	file.Format("%s\\user\\%s\\%s", m_home, m_userN, TICKSETUP);
	GetPrivateProfileString("GENERAL", "BAR00SIZE", "10", buf, sizeof(buf), file);

	tmpS = buf;
	int	total = 0;
	for (int ii = 0; ii < configN; ii++)
	{
		int pos = tmpS.Find(",");
		if (pos == -1)
		{
			pos = atoi(tmpS);
			if (pos < MINPANE)
				pos = MINPANE;
			ary.Add(pos);
			total += pos;
			continue;
		}

		value = tmpS.Left(pos++);
		tmpS = tmpS.Mid(pos);
		pos = atoi(value);
		ary.Add(pos);
		total += pos;
	}
}

void CTInfo1::SetFmx(bool setting, int kind, int key, CString symbol, CString dat)
{
	CPane*	pane;
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
	{
		pane = (CPane*) m_arPane.GetNext(pos);
		pane->SetFmx(setting, kind, key, symbol, dat);
	}
}

void CTInfo1::SetInter(char* dat, bool bRotate)
{
	CPane*	pane;
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
	{
		pane = (CPane*) m_arPane.GetNext(pos);
		pane->SetInter(dat, bRotate);
	}
}

void CTInfo1::UpdatePaneData(int kind, CString dat)
{
	CPane*	pane;
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
	{
		pane = (CPane*) m_arPane.GetNext(pos);
		pane->UpdatePaneData(kind, dat);
	}
}

void CTInfo1::ProcessRTS(CString symbol, CString dat)
{
	CPane*	pane;
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
	{
		pane = (CPane*) m_arPane.GetNext(pos);
		pane->ProcessRTS(symbol, dat);
	}
}

void CTInfo1::ConclusionNotice(CString str)
{
	CPane*	pane;
	for (POSITION pos = m_arPane.GetHeadPosition(); pos; )
	{
		pane = (CPane*) m_arPane.GetNext(pos);
		pane->ProcessNotice(str);
	}
}

void CTInfo1::createButton()
{
	if (m_deal)	return;

	CString		imgC, imgO, imgD;
	DWORD		dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	m_hide = new CNButton(CTRL_HIDE);
	if (!m_hide->Create(NULL, "숨기기", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_hide;
		m_hide = NULL;
	}
	else
	{
		m_hide->set_ToolTipText("숨기기");
		imgC.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_barhide");
		imgO.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_barhide_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_barhide_dn");
		m_hide->set_Image(imgC, imgO, imgD);
	}

	m_deal = new CNButton(CTRL_DEAL);
	if (!m_deal->Create(NULL, "체결", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_deal;
		m_deal = NULL;
	}
	else
	{
		m_deal->set_ToolTipText("체결내역");
		imgC.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_contract");
		imgO.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_contract_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_contract_dn");
		m_deal->set_Image(imgC, imgO, imgD);
	}

	m_screen = new CNButton(CTRL_JSCREEN);
	if (!m_screen->Create(NULL, "종목 스크린", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_screen;
		m_screen = NULL;
	}
	else
	{
		m_screen->set_ToolTipText("종목 스크린");
		imgC.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_screen");
		imgO.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_screen_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_screen_dn");
		m_screen->set_Image(imgC, imgO, imgD);
	}
}

LRESULT CTInfo1::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	CString imgC, imgO, imgD;
	CString data = "SE714700";
	switch (LOWORD(wParam))
	{
	case CTRL_DEAL:		(*m_axiscall)(AXI_SHOWDEAL, wParam, lParam);	break;
	case CTRL_JSCREEN:	(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *) data.operator LPCTSTR());	break;
	case CTRL_HIDE:		(*m_axiscall)(AXI_HIDETICKER, 0, 1);	break;
	}
	return 0;
}

void CTInfo1::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_base)
		m_base->DefaultSize();
	
	CDialogBar::OnLButtonDblClk(nFlags, point);
}
