// Sdibar.cpp : implementation file
//

#include "stdafx.h"
#include "Sdibar.h"
#include "resource.h"
#include "smdefine.h"
#include "nbutton.h"
#include "MenuXP.h"

#include "../../axis/axmsg.hxx"

#define	GAP	2
#define IMGWH	16
#define SRCMASK	0x00220326    // mask

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSdibar

CSdibar::CSdibar(bool (*callback)(int, WPARAM, LPARAM))
{
	m_axiscall	= callback;
	m_width		= SDIBAR_WIDTH;
	m_active	= TRUE;
	m_ToolTip	= NULL;
	m_hoverI	= -1;
	m_downI		= -1;
	m_pinON		= false;
	m_mouseOver	= false;

	if( Axis::isCustomer )
		m_logo = Axis::GetBitmap(IDB_CUSTOM);
	else
		m_logo = Axis::GetBitmap(IDB_IBK);


	m_bitmap[SDIBAR_PIN] = Axis::GetBitmap(IDB_SDI_PIN);
	m_bitmap[SDIBAR_SWITCH] = Axis::GetBitmap(IDB_SDI_RETURN);
	m_bitmap[SDIBAR_CLOSE] = Axis::GetBitmap(IDB_SDI_CLOSE);

	for (int ii = 0; ii <= SDIBUTTON_CNT; ii++)
		m_arRc.Add(CRect(0, 0, 0, 0));
}

CSdibar::~CSdibar()
{
	XMSG();
}


BEGIN_MESSAGE_MAP(CSdibar, CDialogBar)
	//{{AFX_MSG_MAP(CSdibar)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSdibar message handlers

void CSdibar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

CSize CSdibar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	CSize	size;
	CRect	fRc;

	size.cy = 32767;
	size.cx = m_width;
	size.cx += (rect.Width());

	if (IsFloating())
	{
		GetDockingFrame()->GetParent()->GetWindowRect(fRc);
		size.cy = fRc.Height();
	}
	else
	{
		GetDockingFrame()->GetWindowRect(fRc);
		size.cy = fRc.Height();
	}

	return size;
}

void CSdibar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);
	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		CBitmap* oldbuffBitmap = memDC.SelectObject(&bitmap);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			drawBackground(&memDC);
			drawButton(&memDC);
			
			dc.BitBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CSdibar::OnDestroy() 
{
	XMSG();
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
			//m_ToolTip->RelayEvent(NULL);
		}

#ifndef DF_USESTL_SDIBAR
		delete m_ToolTip;
#endif
	}
	
	CDialogBar::OnDestroy();	
}

void CSdibar::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);
	SetRegion();
	Invalidate(FALSE);
}

void CSdibar::drawBackground(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(rc, RGB(60, 66, 79));

	BITMAP bm;
	m_logo->GetBitmap(&bm);
	DrawBitmapByMask(pDC, m_logo, CRect(rc.top + 2, rc.left + 2, rc.left + 2 + bm.bmWidth, rc.top + 2 + bm.bmHeight));
}

void CSdibar::drawButton(CDC* pDC)
{
	if (!m_arRc.GetSize())	return;

	CRect	bRc;
	for (int ii = 0; ii <= SDIBAR_CLOSE; ii++)
	{
		bRc = m_arRc.GetAt(ii);
		if (bRc.IsRectEmpty())
			continue;
		drawImg(pDC, bRc, ii);
	}
}

void CSdibar::drawImg(CDC* pDC, CRect bRc, int idx, bool bDown)
{
	if (!idx)
		return;

	DrawBitmapByMask(pDC, m_bitmap[idx], bRc, bDown);
}

void CSdibar::SetActive(BOOL bActive)
{
	m_active = bActive;
	Invalidate(FALSE);
}

void CSdibar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	const int nItem = GetSelectItem(point);
	switch (nItem)
	{
	case SDIBAR_PIN:
	case SDIBAR_SWITCH:
	case SDIBAR_CLOSE:
		{
			SetCapture();
			CClientDC	dc(this);
			drawImg(&dc, m_arRc.GetAt(nItem), nItem, true);
			m_downI = nItem;
		}
		
		break;;
	default:
		GetParent()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		break;
	}
	CDialogBar::OnLButtonDown(nFlags, point);
}

void CSdibar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	const int nItem = GetSelectItem(point);
	if (m_downI != -1)
	{
		CClientDC	dc(this);
		drawImg(&dc, m_arRc.GetAt(m_downI), m_downI);
		if (nItem == m_downI)
		{
			switch (nItem)
			{
			case SDIBAR_PIN:
				m_pinON = !m_pinON;
				if (m_pinON)
					GetParent()->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
				else	GetParent()->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
				drawImg(&dc, m_arRc.GetAt(m_downI), m_downI);
				break;
			case SDIBAR_SWITCH:
				(*m_axiscall)(AXI_APPSWITCH, 0, 0);
				break;
			case SDIBAR_CLOSE:
				GetParent()->PostMessage(WM_CLOSE);
				break;
			}
		}
	}
	m_downI = -1;
	CDialogBar::OnLButtonUp(nFlags, point);
}

void CSdibar::SetRegion()
{
	CRect	cRc, iRc, bRc;
	GetClientRect(cRc);

	const int	cx = GetSystemMetrics(SM_CXSMICON);
	const int	cy = GetSystemMetrics(SM_CYSMICON);
	const CPoint	pt = CPoint((cRc.Width() - cy)/2, (cRc.Width() - cy)/2 + GAP);
	iRc.SetRect(pt.x, pt.y, pt.x + cx, pt.y + cy);
	m_arRc.SetAt(SDIBAR_ICON, iRc);

	//m_arRc.Add(iRc);
	BITMAP	bm;
	bRc.CopyRect(&iRc);
	bRc.OffsetRect(0, iRc.Height() + GAP*2);
	m_bitmap[SDIBAR_PIN]->GetBitmap(&bm);

	bRc.left = 2;
	bRc.top = -10;
	bRc.right = bRc.left + bm.bmWidth / 2;
	bRc.bottom = bRc.top + bm.bmHeight;

	for (int ii = 0; ii <= SDIBAR_CLOSE; ii++)
	{
		bRc.OffsetRect(0, IMGWH + GAP);
		m_arRc.SetAt(ii, bRc);
	}
}

int CSdibar::GetSelectItem(CPoint pt)
{
	CRect	bRc;
	int	nItem = -1;
	for (int ii = 0; ii <= SDIBAR_CLOSE; ii++)
	{
		bRc = m_arRc.GetAt(ii);
		if (!bRc.PtInRect(pt))
			continue;
		nItem = ii;
		break;
	}
	return nItem;
}

void CSdibar::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*			wndUnderMouse = NULL;
	const CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME{};

	CDialogBar::OnMouseMove(nFlags, point);

	const CPoint	lpoint = point;
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_mouseOver)
		{
			m_mouseOver = true;

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		}

		HoverItem(lpoint);
	}
	else 
		m_mouseOver = false;
}

LRESULT CSdibar::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_mouseOver	= false;
	m_hoverI	= -1;
	return 0;
}

void CSdibar::SetToolTipText(CString text, BOOL bActivate)
{
	if (text.IsEmpty())	return;

	InitToolTip();
	if (m_ToolTip->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip->AddTool(this, text, rectBtn, 1);
		 m_ToolTip->SetDelayTime(100);
	}

	m_ToolTip->UpdateTipText(text, this, 1);
	m_ToolTip->Activate(bActivate);
	//int delaytime = m_ToolTip->GetDelayTime(TTDT_AUTOPOP);
}

void CSdibar::InitToolTip()
{
	if (!m_ToolTip)
	{
#ifdef DF_USESTL_SDIBAR
		m_ToolTip = std::make_unique<CToolTipCtrl>();
#else
		m_ToolTip = new CToolTipCtrl();
#endif
		
		m_ToolTip->Create(this, TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

void CSdibar::HoverItem(CPoint point)
{
	CRect	bRc;
	int	nItem = -1;
	for (int ii = 0; ii <= SDIBAR_CLOSE; ii++)
	{
		bRc = m_arRc.GetAt(ii);
		if (!bRc.PtInRect(point))
			continue;
		nItem = ii;
		break;
	}
	if (nItem == m_hoverI)	return;
	m_hoverI = nItem;

	CString	tipS;
	switch (nItem)
	{
	case SDIBAR_PIN:	tipS =  _T("압정");		break;
	case SDIBAR_SWITCH:	tipS =  _T("MDI 전환");		break;
	case SDIBAR_CLOSE:	tipS =  _T("닫기");	break;
	default:		tipS = _T("");			break;
	}
	if (tipS.IsEmpty())	return;
	SetToolTipText(tipS);
	m_ToolTip->SetToolRect(this, 1, m_arRc.GetAt(nItem));
}

LRESULT CSdibar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			if (!m_ToolTip)
				break;
			MSG	msg{};
			POINT	point;
			
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;
			msg.pt = point;
		
			m_ToolTip->RelayEvent(&msg);
		}
		break;
	default:
		break;
	}
	return CDialogBar::WindowProc(message, wParam, lParam);
}


CString CSdibar::GetSMString(int idx)
{
	CString	string;
	switch (idx)
	{
	case SDIBAR_PIN:
		m_pinON ? string = _T("압정해제") : string = _T("압정설정");	break;
	case SDIBAR_SWITCH:	string = _T("MDI 전환");	break;
	case SDIBAR_CLOSE:	string = _T("닫기");	break;
	}
	return string;
}

void CSdibar::DrawBitmapByMask(CDC* pDC, CBitmap* bitmap, CRect bRC, BOOL bDown, int maskcolor)
{
	const int	bx = bRC.Width();
	const int	by = bRC.Height();

	const int offset = !bDown ? 0: IMGWH;

	CDC		buffDC, maskDC, memoryDC, copyDC;
	CBitmap		buffBitmap, maskBitmap, copyBitmap;
	CBitmap* oldbuffBitmap{}, * oldmaskBitmap{}, * oldmemoryBitmap{}, * oldcopyBitmap{};

	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bx, by, 1, 1, NULL);
	oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	const CRect		maskRc( 0, 0, bx, by);
	buffDC.FillSolidRect(&maskRc, maskcolor);

	memoryDC.CreateCompatibleDC(pDC);
	oldmemoryBitmap = memoryDC.SelectObject(bitmap);
	buffDC.BitBlt( 0, 0, bx, by, &memoryDC, offset, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCCOPY);

	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	copyDC.BitBlt( 0, 0, bx, by, pDC,  bRC.left, bRC.top, SRCCOPY);
	copyDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCPAINT);
	pDC->BitBlt(bRC.left, bRC.top, bx, by, &copyDC, 0, 0, SRCCOPY);

	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);

	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
}