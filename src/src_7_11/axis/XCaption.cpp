// XCaption.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "XCaption.h"
#include "axMisc.h"
#include "axMsg.hxx"
#include "../dll/axissm/MenuXP.h"

#include <algorithm>
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//using std::_cpp_max;
//using std::_cpp_min;

#define	GAP	2
#define SRCMASK	0x00220326
#define INFOSTRING	""
#define CAPTION_TIMER	1

class CStaticCmdUI : public CCmdUI      
{
public: 
	virtual void SetText(LPCTSTR lpszText);
};

void CStaticCmdUI::SetText(LPCTSTR lpszText)
{
	CXCaption* pCaption = (CXCaption*)m_pOther;
	ASSERT(pCaption != NULL);

	pCaption->SetText(lpszText);
}

/////////////////////////////////////////////////////////////////////////////
// CXCaption

CXCaption::CXCaption()
{
	m_cyBottomBorder= m_cyTopBorder = 4;	// l&r default to 6; t&b was 1
	m_title		= "";	// must have some text for CalcFixedLayout
	m_activeCap	= TRUE;
	m_syscom=m_func = 0;
	m_ToolTip	= NULL;
	m_mouseOver	= false;
	m_main		= false;
	m_single	= false;
	m_sdiMode	= false;
	m_bIconic	= false;
	m_pinON		= false;
	m_bMax		= false;
	m_axisres	= NULL;
	m_mainframe	= NULL;
	m_hideflag	= SHOW_ALL;
	m_hoverI	= -1;
	m_downI		= -1;
	m_action	= -1;
	m_vsN		= 0;

	for (int ii = 0; ii < MBMP_CNT; ii++)
		m_arRc.Add(CRect(0, 0, 0, 0));
}

CXCaption::~CXCaption()
{
	m_arRc.RemoveAll();
	if (m_main)
	{
		m_mainbitmap[0].DeleteObject();
		m_mainbitmap[1].DeleteObject();
	}

	if (m_bmCaption[0].GetSafeHandle())
		m_bmCaption[0].DeleteObject();
	if (m_bmCaption[1].GetSafeHandle())
		m_bmCaption[1].DeleteObject();

	if (m_bmCapright[0].GetSafeHandle())
		m_bmCapright[0].DeleteObject();
	if (m_bmCapright[1].GetSafeHandle())
		m_bmCapright[1].DeleteObject();

	if (m_bmVS[0].GetSafeHandle())
		m_bmVS[0].DeleteObject();
	if (m_bmVS[1].GetSafeHandle())
		m_bmVS[1].DeleteObject();
	if (m_bmVS[2].GetSafeHandle())
		m_bmVS[2].DeleteObject();
	if (m_bmVS[3].GetSafeHandle())
		m_bmVS[3].DeleteObject();	
}


BEGIN_MESSAGE_MAP(CXCaption, CControlBar)
	//{{AFX_MSG_MAP(CXCaption)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXCaption message handlers

int CXCaption::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CXCaption::OnDestroy() 
{
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
			m_ToolTip->RelayEvent(NULL);
		}
		delete m_ToolTip;
	}

	CControlBar::OnDestroy();
}

void CXCaption::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	WINDOWPLACEMENT	pl;
	GetParent()->GetWindowPlacement(&pl);

	int nItem = GetSelectItem(point);
	if (m_main)
	{
		if (nItem != -1)
		{
			m_action = nItem;
			m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, (int) m_action), m_key);
		}
		else
		{
			if (m_mainframe->IsZoomed())
				m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, MIDX_RESTORE), m_key);
			else	m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, MIDX_MAX), m_key);
		}
	}
	else
	{
		if (nItem != -1)
		{
			m_action = nItem;
			m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, (int) m_action), m_key);
		}
		else
		{
			m_action = -1;
			if (m_single)
				m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, m_bMax ? IDX_RESTORE:IDX_MAX), m_key);
			else
			{
				if (m_bMax)
					m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, IDX_RESTORE), m_key);
				else
				{
					if (m_bIconic)
					{
						m_bIconic = false;
						m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, IDX_RESTORE), m_key);
					}
					else	m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, IDX_MAX), m_key);
				}
			}
		}
	}

	CControlBar::OnLButtonDblClk(nFlags, point);
}


void CXCaption::OnLButtonDown(UINT nFlags, CPoint point) 
{

	m_downI = -1;
	int nItem = GetSelectItem(point);
	if (nItem == -1)
	{
		//GetParent()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		GetParent()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
		SetTimer(CAPTION_TIMER, 100, NULL);
	}
	else
	{
		CClientDC	dc(this);
		CRect	rc = m_arRc.GetAt(nItem);
		if (m_main)
		{
			switch (nItem)
			{
			case MIDX_MIN:	case MIDX_MAX:	case MIDX_CLOSE:
			case MIDX_RESTORE:
			//case MIDX_DUAL:
				SetCapture();
				m_downI = nItem;
				DrawBitmap(&dc, nItem, rc, false, true);
				break;
			default:
				break;
			}
		}
		else
		{
			switch (nItem)
			{
			case IDX_ICON:
				{
				}
				break;
			default:
				SetCapture();
				m_downI = nItem;
				if (m_bMax && IDX_RESTORE == nItem)
					rc = m_arRc.GetAt(IDX_MAX);
				else	rc = m_arRc.GetAt(nItem);
				DrawBitmap(&dc, nItem, rc, false, true);
				break;
			}
		}
	}

	CControlBar::OnLButtonDown(nFlags, point);
}


void CXCaption::OnLButtonUp(UINT nFlags, CPoint point) 
{

	if (m_downI != -1)
	{
		ReleaseCapture();
		int nItem = GetSelectItem(point);
		if (nItem == m_downI)
		{ 
			m_action = nItem;
			m_mainframe->PostMessage(m_nMsg, MAKEWPARAM(axCAPTION, (int) m_action), m_key);
		}
		else	m_action = -1;

		CClientDC	dc(this);
		CRect		rc;
		if (m_bMax && IDX_RESTORE == m_downI)
			rc = m_arRc.GetAt(IDX_MAX);
		else	rc = m_arRc.GetAt(m_downI);
		DrawBitmap(&dc, m_downI, rc, m_activeCap);
	}

	m_action = -1;
	m_downI = -1;
	CControlBar::OnLButtonUp(nFlags, point);
}

LRESULT CXCaption::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_mouseOver	= false;
	m_hoverI	= -1;
	return 0;
}

void CXCaption::OnMouseMove(UINT nFlags, CPoint point) 
{
	CString tmpS;
	CWnd*			wndUnderMouse = NULL;
	CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME;

	CControlBar::OnMouseMove(nFlags, point);

	CPoint	lpoint = point;
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

void CXCaption::OnPaint() 
{
	CPaintDC	dc(this);
	if (m_main)
	{
		drawMainTitle(&dc);
		return;
	}

	CString tmpS;
	CDC		memDC;
	CRect		cRc, iRc, tRc, clipRc, tmprc;
	CRgn		clipRgn;
	int		round, x, y, width, height;

	GetClientRect(cRc);
	tRc.CopyRect(&cRc);

	round	= cRc.Height()/2;
	x	= cRc.left;
	y	= cRc.top;
	width	= cRc.Width();
	height	= cRc.Height();

	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(&bitmap);

		if (m_sdiMode)
			iRc = m_arRc.GetAt(IDX_PIN);
		else
		{
			if (m_single)
				iRc = m_arRc.GetAt(IDX_PIN);
			else
				iRc = m_arRc.GetAt(m_bIconic ? IDX_MIN : IDX_SINGLE);
		}

		drawTitleBitmap(&memDC, cRc, m_activeCap);

		if (m_axisres)
		{
// draw icon
			iRc = m_arRc.GetAt(IDX_CHILD);
			if (iRc.IsRectEmpty())
			{
				SetRegion();
				iRc = m_arRc.GetAt(IDX_CHILD);
			}
			//DrawBitmap(&memDC, IDX_CHILD, iRc, m_activeCap);

// draw system button
			iRc = m_arRc.GetAt(IDX_CLOSE);	
			DrawBitmap(&memDC, IDX_CHILD_CLOSE, iRc, m_activeCap);
			iRc = m_arRc.GetAt(IDX_MAX);	
			DrawBitmap(&memDC, m_bMax ? IDX_CHILD_RESTORE : IDX_CHILD_MAX, iRc, m_activeCap);
			iRc = m_arRc.GetAt(IDX_MIN);	
			DrawBitmap(&memDC, m_sdiMode ? IDX_MIN : m_bIconic ? IDX_CHILD_RESTORE : IDX_CHILD_MIN, iRc, m_activeCap);

// draw user button
			if (!m_bIconic || m_sdiMode)
			{
				iRc = m_arRc.GetAt(IDX_CAPTURE);	
				DrawBitmap(&memDC, IDX_CAPTURE, iRc, m_activeCap);
				
				iRc = m_arRc.GetAt(IDX_DESIZE);
				DrawBitmap(&memDC, IDX_DESIZE, iRc, m_activeCap);
				
				if (m_sdiMode)
				{
					iRc = m_arRc.GetAt(IDX_PIN);
					DrawBitmap(&memDC, IDX_PIN, iRc, m_activeCap);
				}
				else
				{
					iRc = m_arRc.GetAt(IDX_SINGLE);
					DrawBitmap(&memDC, IDX_SINGLE, iRc, m_activeCap);
					if (m_single)
					{
						iRc = m_arRc.GetAt(IDX_PIN);
						DrawBitmap(&memDC, IDX_PIN, iRc, m_activeCap);
					}
				}
				
				iRc = m_arRc.GetAt(IDX_COPY);	
				DrawBitmap(&memDC, IDX_COPY, iRc, m_activeCap);
				
			}

// draw text
			tRc.left  = m_arRc.GetAt(IDX_CHILD).right + GAP;
			if (m_sdiMode)
				tRc.right = m_arRc.GetAt(IDX_PIN).left - GAP;
			else
			{
				if (m_single)
					tRc.right = m_arRc.GetAt(IDX_PIN).left - GAP;
				else
					tRc.right = m_arRc.GetAt(m_bIconic ? IDX_MIN : IDX_SINGLE).left - GAP;
			}

			CFont*	font = memDC.SelectObject(m_axisres->GetFont());

			if (m_activeCap)
				memDC.SetTextColor(RGB(255, 255, 255));//::GetSysColor(COLOR_CAPTIONTEXT));
			else	memDC.SetTextColor(RGB(192, 192, 192));//::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
	
			memDC.SetBkMode(TRANSPARENT);

			tRc.OffsetRect(0,2);
#ifdef _DEBUG
			int ilen = m_title.Find(_T(")")) + 1;
			CSize size = memDC.GetTextExtent(m_title.Left(ilen));
			tmprc = tRc;
			tmprc.right = size.cx + 20;
#else
			int ilen = m_title.Find(_T("]")) + 1;
			CSize size = memDC.GetTextExtent(m_title.Left(ilen));
			tmprc = tRc;
			tmprc.right = size.cx + 20;
#endif
			tmprc.OffsetRect(-10, 0);
			memDC.SetTextColor(RGB(255,216,100));
			memDC.DrawText(m_title.Left(ilen), tmprc, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
			
			tRc.left = tmprc.right;
			memDC.SetTextColor(RGB(255,255,255));
			memDC.DrawText(m_title.Mid(ilen), tRc, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
			memDC.SelectObject(font);
		}

		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbmp);

		memDC.DeleteDC();
		bitmap.DeleteObject();
	}
	/*
	CPaintDC	dc(this);
	if (m_main)
	{
		drawMainTitle(&dc);
		return;
	}

	CDC		memDC;
	CRect		cRc, iRc, tRc, clipRc;
	CRgn		clipRgn;
	int		round, x, y, width, height;

	GetClientRect(cRc);
	tRc.CopyRect(&cRc);

	round	= cRc.Height()/2;
	x	= cRc.left;
	y	= cRc.top;
	width	= cRc.Width();
	height	= cRc.Height();

	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(&bitmap);

		if (m_sdiMode)
			iRc = m_arRc.GetAt(IDX_PIN);
		else
		{
			if (m_single)
				iRc = m_arRc.GetAt(IDX_PIN);
			else
				iRc = m_arRc.GetAt(m_bIconic ? IDX_MIN : IDX_SINGLE);
		}

		drawTitleBitmap(&memDC, cRc, m_activeCap);

		if (m_axisres)
		{
// draw icon
			iRc = m_arRc.GetAt(IDX_CHILD);
			if (iRc.IsRectEmpty())
			{
				SetRegion();
				iRc = m_arRc.GetAt(IDX_CHILD);
			}
			DrawBitmap(&memDC, IDX_CHILD, iRc, m_activeCap);

// draw system button
			iRc = m_arRc.GetAt(IDX_CLOSE);	DrawBitmap(&memDC, IDX_CLOSE, iRc, m_activeCap);
			iRc = m_arRc.GetAt(IDX_MAX);	DrawBitmap(&memDC, m_bMax ? IDX_RESTORE : IDX_MAX, iRc, m_activeCap);
			iRc = m_arRc.GetAt(IDX_MIN);	DrawBitmap(&memDC, m_sdiMode ? IDX_MIN : m_bIconic ? IDX_RESTORE : IDX_MIN, iRc, m_activeCap);

// draw user button
			if (!m_bIconic || m_sdiMode)
			{
//				iRc = m_arRc.GetAt(IDX_HELP);	DrawBitmap(&memDC, IDX_HELP, iRc);
				iRc = m_arRc.GetAt(IDX_COPY);	DrawBitmap(&memDC, IDX_COPY, iRc, m_activeCap);
				if (!(m_hideflag & HIDE_FONT))
				{
					iRc = m_arRc.GetAt(IDX_FONT);
					DrawBitmap(&memDC, IDX_FONT, iRc, m_activeCap);
				}
				iRc = m_arRc.GetAt(IDX_GROUP);	DrawBitmap(&memDC, IDX_GROUP, iRc, m_activeCap);
				if (m_sdiMode)
				{
					iRc = m_arRc.GetAt(IDX_PIN);
					DrawBitmap(&memDC, IDX_PIN, iRc, m_activeCap);
				}
				else
				{
					iRc = m_arRc.GetAt(IDX_SINGLE);
					DrawBitmap(&memDC, IDX_SINGLE, iRc, m_activeCap);
					if (m_single)
					{
						iRc = m_arRc.GetAt(IDX_PIN);
						DrawBitmap(&memDC, IDX_PIN, iRc, m_activeCap);
					}
				}
			}

// draw text
			tRc.left  = m_arRc.GetAt(IDX_CHILD).right + GAP;
			if (m_sdiMode)
				tRc.right = m_arRc.GetAt(IDX_PIN).left - GAP;
			else
			{
				if (m_single)
					tRc.right = m_arRc.GetAt(IDX_PIN).left - GAP;
				else
					tRc.right = m_arRc.GetAt(m_bIconic ? IDX_MIN : IDX_SINGLE).left - GAP;
			}

			CFont*	font = memDC.SelectObject(m_axisres->GetFont());

			if (m_activeCap)
				memDC.SetTextColor(RGB(255, 255, 255));//::GetSysColor(COLOR_CAPTIONTEXT));
			else	memDC.SetTextColor(RGB(192, 192, 192));//::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
	
			memDC.SetBkMode(TRANSPARENT);
			memDC.DrawText(m_title, tRc, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
			memDC.SelectObject(font);
		}

		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbmp);

		memDC.DeleteDC();
		bitmap.DeleteObject();
	}
	*/
}

void CXCaption::drawMainTitle(CDC* pDC)
{
}

LRESULT CXCaption::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CString tmpS;
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
			MSG	msg;
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

	
	return CControlBar::WindowProc(message, wParam, lParam);
}

LRESULT CXCaption::OnSizeParent(WPARAM, LPARAM lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
	return CControlBar::OnSizeParent(0, lParam);
}

void CXCaption::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CStaticCmdUI state;
	state.m_pOther = this;
	state.m_nIndexMax = 1;		// there's only one thing to update
	state.m_nID = AFX_IDW_STATUS_BAR;

	// allow the statusbar itself to have update handlers
	if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;

	// allow target (owner) to handle the remaining updates
	state.DoUpdate(pTarget, FALSE);
}

CSize CXCaption::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	// The Y value is the sum of the calculated height from DrawText,
	// plus the top and bottom border.
	CSize size;
	size.cx = 32767;
	size.cy = m_main ? 19 : XCAP_HEIGHT;

	return size;
}

void CXCaption::DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
{
	CRect	rect;
	int	nWidth, nHeight;

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width(); 
	nHeight = drawRC.Height();

	int rs,gs,bs;
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re,ge,be;
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float	rStep, gStep, bStep;
	CPen	*sPen;
	if (bRight)
	{
		if (!nWidth)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nWidth);
		gStep = float(gs - ge)/float(nWidth);
		bStep = float(bs - be)/float(nWidth);

		if (!nHeight)
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left + ii, drawRC.top);
				pDC->LineTo(drawRC.left + ii + 1, drawRC.top);
				pDC->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				rect.SetRect(drawRC.left + ii, drawRC.top, drawRC.left + ii + 1, drawRC.bottom);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
	else
	{
		if (!nHeight)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nHeight);
		gStep = float(gs - ge)/float(nHeight);
		bStep = float(bs - be)/float(nHeight);

		if (!nWidth)
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left, drawRC.top + ii);
				pDC->LineTo(drawRC.left, drawRC.top + ii + 1);
				pDC->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
}

void CXCaption::SetText(LPCTSTR lpszNew)
{
	ASSERT(lpszNew && AfxIsValidString(lpszNew));

	if (m_title != lpszNew)
	{
		m_title = lpszNew;
		Invalidate();
	}
}

void CXCaption::ChangeSkin()
{
	CString file;
	//file.Format("%s\\image\\child_caption_act.bmp", m_axisres->GetHome());
	file.Format("%s\\image\\child_act_caption.bmp", m_axisres->GetHome());

	m_bmCaption[0].LoadBitmap(file);

	//file.Format("%s\\image\\child_caption_inact.bmp", m_axisres->GetHome());
	file.Format("%s\\image\\child_inact_caption.bmp", m_axisres->GetHome());
	m_bmCaption[1].LoadBitmap(file);

	file.Format("%s\\image\\child_act_caption.bmp", m_axisres->GetHome());
	m_bmVS[0].LoadBitmap(file);

	file.Format("%s\\image\\child_act_rcaption.bmp", m_axisres->GetHome());
	m_bmCapright[0].LoadBitmap(file);

	file.Format("%s\\image\\child_inact_rcaption.bmp", m_axisres->GetHome());
	m_bmCapright[1].LoadBitmap(file);


	/*
	file.Format("%s\\image\\VS_Title1.bmp", m_axisres->GetHome());
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (m_bmVS[1].GetSafeHandle())
		m_bmVS[1].DeleteObject();
	if (hBitmap)	m_bmVS[1].Attach(hBitmap);

	file.Format("%s\\image\\VS_Title1.bmp", m_axisres->GetHome());
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (m_bmVS[2].GetSafeHandle())
		m_bmVS[2].DeleteObject();
	if (hBitmap)	m_bmVS[2].Attach(hBitmap);

	file.Format("%s\\image\\VS_Title1.bmp", m_axisres->GetHome());
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (m_bmVS[3].GetSafeHandle())
		m_bmVS[3].DeleteObject();
	if (hBitmap)	m_bmVS[3].Attach(hBitmap);
	*/
	m_aVSColor = m_axisres->GetColor(ACTTITLE_VSBK);
	m_aVEColor = m_axisres->GetColor(ACTTITLE_VEBK);
	m_aHSColor = m_axisres->GetColor(ACTTITLE_HSBK);
	m_aHEColor = m_axisres->GetColor(ACTTITLE_HEBK);

	m_iVSColor = m_axisres->GetColor(INACTTITLE_VSBK);
	m_iVEColor = m_axisres->GetColor(INACTTITLE_VEBK);
	m_iHSColor = m_axisres->GetColor(INACTTITLE_HSBK);
	m_iHEColor = m_axisres->GetColor(INACTTITLE_HEBK);
}

void CXCaption::ChangeVS(int vsN)
{
	m_vsN = vsN;
	Invalidate(FALSE);
}

int CXCaption::GetLuminosity(COLORREF color) const
{
	const int HlsMax = 240;	// This is what Display Properties uses
	const int RgbMax = 255;	// max r/g/b value is 255

	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	
	int rgbMax = max( max(r, g), b);
	int rgbMin = min( min(r, g), b);
	return (((rgbMax + rgbMin) * HlsMax) + RgbMax ) / (2 * RgbMax);
}

bool CXCaption::ActiveateApp(BOOL bActive)
{
	if (m_activeCap != bActive)
	{
		m_activeCap = bActive;
		if (GetSafeHwnd())
			Invalidate();
		if (!m_main && m_activeCap && m_mainframe)
		{
			if (GetParent()->GetStyle() & WS_VISIBLE)
			{
				m_mainframe->SendMessage(WM_AXIS, MAKEWPARAM(axFOCUS, m_key), 0);
			}
		}
		return true;
	}

	return false;
}

BOOL CXCaption::ShowBmp(CDC* pDC, int x, int y, CString filename, int maskcolor)
{
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),filename,
	          IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if( hBitmap == NULL )	return FALSE;


	BITMAP  bm;   
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
	CRect  r(0,0,bm.bmWidth,bm.bmHeight);

	CDC		 buffDC ;
	buffDC.CreateCompatibleDC( pDC ) ;
	CBitmap  buffBitmap ;
	buffBitmap.CreateCompatibleBitmap( pDC, bm.bmWidth, bm.bmHeight ) ;
	CBitmap* oldbuffBitmap = buffDC.SelectObject( &buffBitmap ) ;

	CDC		 maskDC ;
	maskDC.CreateCompatibleDC( pDC ) ;
	CBitmap  maskBitmap ;  
	maskBitmap.CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL ) ;
	CBitmap* oldmaskBitmap = maskDC.SelectObject( &maskBitmap ) ;

	buffDC.FillSolidRect( &r, maskcolor) ;

	CDC		 memoryDC ;
	memoryDC.CreateCompatibleDC( pDC ) ;
	CBitmap* oldmemoryBitmap = memoryDC.SelectObject( CBitmap::FromHandle( hBitmap ) ) ;
	buffDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &memoryDC, 0, 0, SRCCOPY ) ;
	maskDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC, 0, 0, SRCCOPY ) ;

	CDC		 copyDC ;
	copyDC.CreateCompatibleDC( pDC ) ;
	CBitmap  copyBitmap ;
	copyBitmap.CreateCompatibleBitmap( pDC, bm.bmWidth, bm.bmHeight ) ;
	CBitmap* oldcopyBitmap = copyDC.SelectObject( &copyBitmap ) ;

	copyDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, pDC,  x, y, SRCCOPY  ) ;
	copyDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCAND  ) ;
	buffDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCMASK  ) ;
	copyDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC,   0, 0, SRCPAINT  ) ;
	pDC->BitBlt(x,y, bm.bmWidth, bm.bmHeight, &copyDC,   0, 0, SRCCOPY  ) ;

	copyDC.SelectObject( oldcopyBitmap ) ;
	memoryDC.SelectObject( oldmemoryBitmap ) ;
	maskDC.SelectObject( oldmaskBitmap ) ;
	buffDC.SelectObject( oldbuffBitmap ) ;
	return TRUE;
}

BOOL CXCaption::ShowBmp(CDC* pDC, int x, int y, UINT nResID, int maskcolor)
{
	CBitmap	bitmap;
	if (!bitmap.LoadBitmap(nResID))
		return FALSE;
	BITMAP  bm;   
	bitmap.GetBitmap(&bm);
	CRect  r(0,0,bm.bmWidth,bm.bmHeight);

	CDC		 buffDC;
	buffDC.CreateCompatibleDC(pDC);
	CBitmap  buffBitmap;
	buffBitmap.CreateCompatibleBitmap( pDC, bm.bmWidth, bm.bmHeight);
	CBitmap* oldbuffBitmap = buffDC.SelectObject( &buffBitmap);

	CDC		 maskDC ;
	maskDC.CreateCompatibleDC(pDC) ;
	CBitmap  maskBitmap ;  
	maskBitmap.CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL ) ;
	CBitmap* oldmaskBitmap = maskDC.SelectObject( &maskBitmap ) ;

	buffDC.FillSolidRect(&r, maskcolor);

	CDC		 memoryDC ;
	memoryDC.CreateCompatibleDC( pDC ) ;
	CBitmap* oldmemoryBitmap = memoryDC.SelectObject(&bitmap/* CBitmap::FromHandle( hBitmap ) */) ;
	buffDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &memoryDC, 0, 0, SRCCOPY ) ;
	maskDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC, 0, 0, SRCCOPY ) ;

	CDC		 copyDC ;
	copyDC.CreateCompatibleDC( pDC ) ;
	CBitmap  copyBitmap ;
	copyBitmap.CreateCompatibleBitmap( pDC, bm.bmWidth, bm.bmHeight ) ;
	CBitmap* oldcopyBitmap = copyDC.SelectObject( &copyBitmap ) ;

	copyDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, pDC,  x, y, SRCCOPY  ) ;
	copyDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCAND  ) ;
	buffDC.BitBlt(   0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCMASK  ) ;
	copyDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC,   0, 0, SRCPAINT  ) ;
	pDC->BitBlt(x,y, bm.bmWidth, bm.bmHeight, &copyDC,   0, 0, SRCCOPY  ) ;

	copyDC.SelectObject( oldcopyBitmap ) ;
	memoryDC.SelectObject( oldmemoryBitmap ) ;
	maskDC.SelectObject( oldmaskBitmap ) ;
	buffDC.SelectObject( oldbuffBitmap ) ;
	return TRUE;
}

int CXCaption::GetSysCommand(CPoint point)
{
	CRect	sRc, cRc;

	GetClientRect(cRc);
	int	width = cRc.Height() - GAP*2;
	
	sRc.SetRect(cRc.right - (width+GAP), cRc.top + GAP, cRc.right - GAP, cRc.bottom - GAP);
	for (int ii = 3; ii > 0; ii--)
	{
		if (sRc.PtInRect(point))
			return ii;
		sRc.OffsetRect(-(width+GAP), 0);
	}

	sRc.SetRect(GAP, GAP, GAP + width, GAP + width);
	if (sRc.PtInRect(point))
	{
		// getsystemmenu display
		return -1;
	}
	return 0;
}

void CXCaption::SetMain()
{
	m_main = true;

	m_mainbitmap[0].LoadBitmap(IDB_VSCREEN);
	m_mainbitmap[1].LoadBitmap(IDB_MAINFUNC);
}

void CXCaption::SetToolTipText(CString text, BOOL bActivate)
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

void CXCaption::InitToolTip()
{
	if (!m_ToolTip)
	{
		m_ToolTip = new CToolTipCtrl();
		m_ToolTip->Create(this, TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

void CXCaption::HoverItem(CPoint point)
{
	int nItem = GetSelectItem(point);
	if (nItem == m_hoverI)	return;
	m_hoverI = nItem;

	CString	tipS;
	if (m_main)
	{
		switch (nItem)
		{
		case MIDX_MIN:	tipS =  _T("최소화");		break;
		case MIDX_MAX:
			if (m_mainframe->IsZoomed())
				tipS = _T("이전크기로");
			else	tipS = _T("최대화");
			break;
		case MIDX_CLOSE:	tipS = _T("닫기");	break;
		default:	break;
		}
	}
	else
	{
		switch (nItem)
		{
		case IDX_PIN:	tipS = m_bIconic ? _T("") : m_pinON  ? _T("최상단해제") : _T("최상단");	break;
		case IDX_SINGLE:tipS = m_bIconic ? _T("") : m_single  ? _T("복원") : _T("독립실행");	break;
		case IDX_GROUP:	tipS = m_bIconic ? _T("") : _T("그룹변경");	break;
		case IDX_COPY:	tipS = m_bIconic ? _T("") : _T("화면복사");	break;
//		case IDX_HELP:	tipS = m_bIconic ? _T("") : _T("도움말");	break;
		case IDX_FONT:	tipS = m_bIconic ? _T("") : _T("기본사이즈로");	break;

		case IDX_MIN:	
		case IDX_CHILD_MIN:	
			tipS =  _T("최소화");	
			nItem = IDX_MIN;
			break;
		case IDX_MAX:	
		case IDX_CHILD_MAX:	
			m_bMax ? tipS = _T("이전크기로"):tipS = _T("최대화");	
			nItem = IDX_MAX;
			break;
		case IDX_CLOSE:	
		case IDX_CHILD_CLOSE:	
			tipS = _T("닫기");	
			nItem = IDX_CLOSE;
			break;
		case IDX_RESTORE:
		case IDX_CHILD_RESTORE:
			tipS = _T("이전크기로");
			nItem = IDX_RESTORE;
			break;
		case IDX_CAPTURE:
			tipS = _T("이미지로저장");
		default:	break;
		}
	}

	if (tipS.IsEmpty())	return;
	SetToolTipText(tipS);
	
	CRect	rc;
	if (m_bMax && IDX_RESTORE == nItem)
		rc = m_arRc.GetAt(IDX_MAX);
	else	rc = m_arRc.GetAt(nItem);
	m_ToolTip->SetToolRect(this, 1, rc);
}

void CXCaption::SetFunc(int index, bool on)
{
	if (index >= FUNCNO)	return;
//	m_funcONOFF[index] = on;
}

void CXCaption::SetGroup(int group)
{
	//m_groupN = group;
	m_groupN = 1;
	//	redraw group
	Invalidate();
}

void CXCaption::SetMisc(CWnd* mainframe, UINT nMsg, int key, int fontsize, class _axisRes* axisres, bool sdi)
{
	m_mainframe	= mainframe;
	m_nMsg		= nMsg;
	m_key		= key;
	m_fontSize	= fontsize;
	m_axisres	= axisres;
	m_sdiMode	= sdi;
	//m_sdiMode	= true;

	ChangeSkin();
	SetRegion();

	if (!m_main && m_activeCap && m_mainframe)
		m_mainframe->SendMessage(WM_AXIS, MAKEWPARAM(axFOCUS, m_key), 0);
}

void CXCaption::SetSDI()
{
	m_sdiMode = true;
	m_single  = false;
	SetRegion();
	Invalidate(FALSE);
}

int CXCaption::ChangeFont()
{
	CMenuXP *menu = new CMenuXP;
	CString	str;

	menu->CreatePopupMenu();
	int	base = 25000;

	menu->AppendMenuX(MF_STRING, base + INITMAPSIZE, "◁");
	menu->AppendSeparator();
	for (int ii = 8; ii < 13; ii++)
	{
		str.Format("%d", ii);
		menu->AppendMenuX(MF_STRING, base + ii, str);
		if (ii == m_fontSize)
			menu->CheckMenuItem(base + ii, MF_CHECKED);
	}

	UINT	nFlags = TPM_RETURNCMD|TPM_LEFTALIGN|TPM_RIGHTBUTTON;
	CRect	rect   = m_arRc.GetAt(IDX_FONT);
	CPoint	point  = CPoint(rect.left, rect.bottom);
	ClientToScreen(&point);
	int	comm = menu->TrackPopupMenu(nFlags, point.x, point.y + GAP, this);
	if (comm > base)
	{
		if (comm - base != INITMAPSIZE)
			m_fontSize = comm - base;
	}
	delete menu;
	return comm - base;
}

int CXCaption::ChangeGroup()
{
	CMenuXP *menu = new CMenuXP;
	CString	str;

	menu->CreatePopupMenu();
	int base = 25000;
	for (int ii = 0; ii <= 5; ii++)
	{
		if (ii)
			str.Format("%d", ii);
		else	str = _T("N");
		menu->AppendMenuX(MF_STRING, base + ii, str);
		if (ii == m_groupN)
			menu->CheckMenuItem(base + ii, MF_CHECKED);
	}
	menu->AppendMenuX(MF_STRING, base + ALL_GROUP, _T("A"));
	if (ALL_GROUP == m_groupN)
		menu->CheckMenuItem(base + ALL_GROUP, MF_CHECKED);
	
	UINT	nFlags = TPM_RETURNCMD|TPM_LEFTALIGN|TPM_RIGHTBUTTON;
	CRect	rect   = m_arRc.GetAt(IDX_GROUP);
	CPoint	point  = CPoint(rect.left, rect.bottom);
	ClientToScreen(&point);
	int	comm = menu->TrackPopupMenu(nFlags, point.x, point.y + GAP, this);
	delete menu;
	return comm - base;
}

int CXCaption::DrawBitmap(CDC* pDC, int index, CRect bRC, BOOL bActive, bool bDown)
{
	if (bRC.IsRectEmpty())	
		return 0;
	return 
		DrawBitmapByMask(pDC, index, bRC, bDown, bActive);
	
	CDC	memDC;
	if (!memDC.CreateCompatibleDC(pDC))
		return 0;

	BITMAP	bm;
	CBitmap* bitmap	= m_axisres->GetBmp(index);
	CBitmap* pbitmap= memDC.SelectObject(bitmap);
	bitmap->GetBitmap(&bm);
	
	int	bx = bRC.Width();
	int	by = bRC.Height();
	int	sx = bDown ? bx : 0;
	int	iCnt;
	switch (index)
	{
	case IDX_GROUP:
		iCnt = bm.bmWidth/(2*UB_WIDTH);
		if (m_groupN == ALL_GROUP)
			sx = bDown ? iCnt*UB_WIDTH + (iCnt-1)*UB_WIDTH : (iCnt-1)*UB_WIDTH;
		else	sx = bDown ? iCnt*UB_WIDTH + m_groupN*UB_WIDTH : m_groupN*UB_WIDTH;
		break;
	case IDX_SINGLE:
		if (m_single)
			sx = bDown ? UB_WIDTH*3 : UB_WIDTH*2;
		break;
	case IDX_PIN:
		break;
	}

	if (pbitmap == (CBitmap *) 0)
	{
		memDC.DeleteDC();
		return 0 ;
	}

	pDC->BitBlt(bRC.left, bRC.top, bx, by, &memDC, sx, 0, SRCCOPY);
	memDC.SelectObject(pbitmap);
	memDC.DeleteDC();

	return bx;
}

int CXCaption::DrawBitmapByMask(CDC* pDC, int index, CRect bRC, bool bDown, BOOL bActive, int maskcolor)
{
	if (bRC.IsRectEmpty())	return 0;

	BITMAP		bm;
	CBitmap*	bitmap;
	int		iCnt, bx, by, sx;

	bx = bRC.Width();
	by = bRC.Height();

	//sx = bDown ? bx : (bActive ? 0 : bx *3 - 3);

	sx = bDown ? bx : 0 ;
	if (m_main)
	{
		switch (index)
		{
		case MIDX_CLOSE:
			bitmap = m_axisres->GetBmp(IDX_CHILD_CLOSE);
			bitmap->GetBitmap(&bm);
			break;
		case MIDX_MAX:
			bitmap = m_axisres->GetBmp(IDX_CHILD_MAX);
			bitmap->GetBitmap(&bm);
			break;
		case MIDX_MIN:
			bitmap = m_axisres->GetBmp(IDX_CHILD_MIN);
			bitmap->GetBitmap(&bm);
			break;
		case MIDX_RESTORE:
			bitmap = m_axisres->GetBmp(IDX_RESTORE);
			bitmap->GetBitmap(&bm);
			break;
		default:
			return 0;
		}
	}
	else
	{
		if (!bDown)
			sx = bActive ? 0 : bx*3;

		if(index == IDX_MIN)
			index = IDX_CHILD_MIN;
		else if(index == IDX_MAX)
			index =IDX_CHILD_MAX;
		else if(index == IDX_CLOSE)
			index = IDX_CHILD_CLOSE;
		else if(index == IDX_RESTORE)
			index = IDX_CHILD_RESTORE;

		bitmap = m_axisres->GetBmp(index);
		if (bitmap)	
			bitmap->GetBitmap(&bm);
		switch (index)
		{
		case	IDX_CHILD:
			sx = bActive ? 0 : bm.bmWidth / 2;
			break;
		//	TRACE("%ld", sx);
		case 	IDX_CHILD_MIN:
		case	IDX_CHILD_MAX:
		case 	IDX_CHILD_CLOSE:
		case	IDX_CHILD_RESTORE:
				bx -= 1;
				sx = bDown ? bx : bActive? 0 : bx *3;			
			break;
		case IDX_GROUP:
			iCnt = bm.bmWidth/(3*UB_WIDTH);
			if (m_groupN == ALL_GROUP)
				sx = bDown ? iCnt*UB_WIDTH + (iCnt-1)*UB_WIDTH : (iCnt-1)*UB_WIDTH;
			else	
				sx = bDown ? iCnt*UB_WIDTH + m_groupN*UB_WIDTH : (bActive? m_groupN*UB_WIDTH : iCnt*UB_WIDTH*2 + m_groupN*UB_WIDTH);
			break;
		case IDX_SINGLE:
			if (m_single)
			{
				if(bActive)
					sx = 0;
				//	sx = bDown ? UB_WIDTH*3 : UB_WIDTH*2;
				else
					sx = UB_WIDTH*3;
				//	sx = bDown ? UB_WIDTH*3 : UB_WIDTH*2;
			}
			break;
		case IDX_PIN:
			if (m_pinON)
				//sx = bDown ? bm.bmWidth/3 + UB_WIDTH : bm.bmWidth/3;//(bActive ? bm.bmWidth/3 : bm.bmWidth/3 + UB_WIDTH*2 );
				sx = bDown ? bm.bmWidth/3 + UB_WIDTH : (bActive ? bm.bmWidth/3 : bm.bmWidth*2/3 + UB_WIDTH );
			else	
				sx = bDown ? UB_WIDTH : (bActive ? 0 : bm.bmWidth*2/3);
				//sx = bDown ? UB_WIDTH : 0;//(bActive ? 0 : bm.bmWidth/2 + UB_WIDTH);
	
				
				break;
		case IDX_ICON:
			bitmap = m_axisres->GetIcon();
			break;
		}
	}


	if(!bActive)
		CString str;

	CDC		buffDC, maskDC, memoryDC, copyDC;
	CBitmap		buffBitmap, maskBitmap, copyBitmap;
	CBitmap		*oldbuffBitmap, *oldmaskBitmap, *oldmemoryBitmap, *oldcopyBitmap;

	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bx, by, 1, 1, NULL);
	oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	CRect		maskRc( 0, 0, bx, by);
	buffDC.FillSolidRect(&maskRc, maskcolor);

	memoryDC.CreateCompatibleDC(pDC);
	oldmemoryBitmap = memoryDC.SelectObject(bitmap);
	buffDC.BitBlt( 0, 0, bx, by, &memoryDC, sx/*0*/, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCCOPY);

	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	copyDC.BitBlt( 0, 0, bx, by, pDC,  bRC.left, bRC.top, SRCCOPY);
	copyDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCPAINT);
	pDC->BitBlt(bRC.left, bRC.top, bx, by, &copyDC, 0/*sx*/, 0, SRCCOPY);

	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);

	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
	return bx;
}
	
void CXCaption::SetRegion()
{
	if (!m_axisres)	return;
	CRect	cRc, iRc;
	CPoint	pt;
	BITMAP	bm;

	GetClientRect(cRc);
	m_axisres->GetBmp(IDX_CLOSE)->GetBitmap(&bm);
	if (m_main)
	{
		pt = CPoint(cRc.right - bm.bmWidth/4, (cRc.Height() - bm.bmHeight)/2);
		iRc.SetRect(pt.x, pt.y, pt.x + bm.bmWidth/4, pt.y + bm.bmHeight);
		iRc.OffsetRect(-GAP, 0);

		for (int ii = MIDX_CLOSE; ii > MIDX_ICON; ii--)
		{
			switch (ii)
			{
			case MIDX_MIN:
				m_arRc.SetAt(MIDX_MIN, iRc);
				iRc.OffsetRect(-GAP, 0);	
				break;
			case MIDX_MAX:
				m_arRc.SetAt(MIDX_RESTORE, iRc);
				m_arRc.SetAt(MIDX_MAX, iRc);
				iRc.OffsetRect(-1, 0);	
				break;
			case MIDX_CLOSE:
				m_arRc.SetAt(MIDX_CLOSE, iRc);
				iRc.OffsetRect(-1, 0);	
				break;
			default:		continue;
			}
			
			iRc.OffsetRect(-bm.bmWidth/4 - 2, 0);
		}
	}
	else
	{
		pt = CPoint(cRc.right - bm.bmWidth/4, (cRc.Height() - bm.bmHeight)/2);
		//iRc.SetRect(pt.x, pt.y, pt.x + bm.bmWidth/4 - 1, pt.y + bm.bmHeight);
		iRc.SetRect(pt.x, pt.y, pt.x + bm.bmWidth/4, pt.y + bm.bmHeight);
		iRc.OffsetRect(-GAP, 0);
	
		for (int ii = IDX_CLOSE; ii >= IDX_COPY; ii--)
		{
			switch (ii)
			{
			case IDX_COPY:
				m_arRc.SetAt(IDX_COPY, iRc);
				//iRc.OffsetRect(-GAP*5, 0);
				break;
			case IDX_SINGLE:
				if (m_hideflag & HIDE_SINGLE)	
				{
					m_arRc.SetAt(IDX_SINGLE, CRect(0, 0, 0, 0));
					continue;
				}
				if (m_sdiMode)	
					continue;
				m_arRc.SetAt(IDX_SINGLE, iRc);
				//iRc.OffsetRect(-GAP*5, 0);
				break;
			case IDX_PIN:
				if (m_sdiMode)
					m_arRc.SetAt(IDX_PIN, iRc);
				else
				{
					 if (!m_single)
					{
						m_arRc.SetAt(IDX_PIN, CRect(0,0,0,0));
						continue;
					 }
					 else
						m_arRc.SetAt(IDX_PIN, iRc);
				}
				//iRc.OffsetRect(-GAP*5, 0);
				break;
			case IDX_DESIZE:
				m_arRc.SetAt(IDX_DESIZE, iRc);
				//iRc.OffsetRect(-GAP*5, 0);
				break;
			case IDX_CAPTURE:
				m_arRc.SetAt(IDX_CAPTURE, iRc);
				//iRc.OffsetRect(-GAP*5, 0);
				break;
			case IDX_MIN:
				m_arRc.SetAt(IDX_MIN, iRc);
				iRc.OffsetRect(-GAP*5, 0);
				break;
			case IDX_MAX:
				if (m_hideflag & HIDE_MAX)	
					continue;

				
				m_arRc.SetAt(IDX_MAX, iRc);
				if (m_main)	
					m_arRc.SetAt(IDX_RESTORE, iRc);
		
				//iRc.OffsetRect(1, 0);
				break;
			case IDX_CLOSE:
				iRc.right += 1;
				m_arRc.SetAt(IDX_CLOSE, iRc);
				iRc.right -= 1;
				iRc.OffsetRect(-1, 0);
				break;
			default:

				break;
			}
			iRc.OffsetRect(-bm.bmWidth/4, 0);
			iRc.OffsetRect(-GAP * 2, 0);
			/*
			switch (ii)
			{
			case IDX_PIN:
	//			if (m_sdiMode)
	//				iRc = m_arRc.GetAt(IDX_SINGLE);
	//			else if (!m_single)
	//				iRc.SetRectEmpty();
				if (!m_sdiMode)
				{
					if (!m_single)
						iRc.SetRectEmpty();
				}
				m_arRc.SetAt(IDX_PIN, iRc);	break;
				break;
			case IDX_SINGLE:
				if (m_hideflag & HIDE_SINGLE)	
				{
					m_arRc.SetAt(IDX_SINGLE, CRect(0, 0, 0, 0));
					continue;
				}
				if (m_sdiMode)	continue;
				m_arRc.SetAt(IDX_SINGLE, iRc);
			//	iRc.OffsetRect(-1, 0);		
				break;
				
			case IDX_GROUP:
				if (m_hideflag & HIDE_GROUP)	
				{
					m_arRc.SetAt(IDX_GROUP, CRect(0, 0, 0, 0));
					continue;
				}
				m_arRc.SetAt(IDX_GROUP, iRc);
				//iRc.OffsetRect(-1, 0);		
				break;
				
			case IDX_DESIZE:
				m_arRc.SetAt(IDX_DESIZE, iRc);
				iRc.OffsetRect(-GAP*5, 0);
				
		//		if (m_hideflag & HIDE_COPY)	
		//		{
		//			m_arRc.SetAt(IDX_COPY, CRect(0, 0, 0, 0));
		//			continue;
		//		}
				m_arRc.SetAt(IDX_COPY, iRc);
				
				break;
// 			case IDX_HELP:
// 				if (m_hideflag & HIDE_HELP)	
// 				{
// 					m_arRc.SetAt(IDX_HELP, CRect(0, 0, 0, 0));
// 					continue;
// 				}
// 				m_arRc.SetAt(IDX_HELP, iRc);
// 				iRc.OffsetRect(-1, 0);		break;
			case IDX_FONT:
				if (m_hideflag & HIDE_FONT)
				{
					m_arRc.SetAt(IDX_FONT, CRect(0, 0, 0, 0));
					continue;
				}
				m_arRc.SetAt(IDX_FONT, iRc);
				//iRc.OffsetRect(-1, 0);		
				break;
			case IDX_MIN:
				m_arRc.SetAt(IDX_MIN, iRc);
				if (!m_main)	m_arRc.SetAt(IDX_MIN, iRc);
				iRc.OffsetRect(-GAP*5, 0);
				break;
			case IDX_MAX:
				if (m_hideflag & HIDE_MAX)	continue;
				m_arRc.SetAt(IDX_MAX, iRc);
				if (m_main)	m_arRc.SetAt(IDX_RESTORE, iRc);
				//iRc.OffsetRect(-1, 0);		
				iRc.OffsetRect(1, 0);
				break;
			case IDX_CLOSE:
				iRc.right += 1;
				m_arRc.SetAt(IDX_CLOSE, iRc);
				iRc.right -= 1;

		//		iRc.OffsetRect(-1, 0);	
				iRc.OffsetRect(1, 0);
				
				break;
			case IDX_PRINT:
				m_arRc.SetAt(IDX_PRINT, iRc);
				iRc.OffsetRect(-GAP*5, 0);
			default:				
				continue;
			}
			iRc.OffsetRect(-bm.bmWidth/4, 0);
			*/
		}
	}

	int	cx = GetSystemMetrics(SM_CXSMICON); 
	int	cy = GetSystemMetrics(SM_CYSMICON);
	//pt = CPoint((cRc.Height() - cy)/2 + GAP, (cRc.Height() - cy)/2);
	pt = CPoint(0, 0);
	iRc.SetRect(pt.x, pt.y, pt.x + cx, pt.y + cy);
//	m_arRc.SetAt(m_main ? MIDX_CHILD : IDX_CHILD, iRc);

	m_arRc.SetAt(IDX_CHILD, iRc);

}

void CXCaption::OnSize(UINT nType, int cx, int cy) 
{
	SetRegion();
	CControlBar::OnSize(nType, cx, cy);
}

int CXCaption::GetSelectItem(CPoint point)
{
	CRect	bRc;
	if (m_main)
	{
		for (int ii = 0; ii < m_arRc.GetSize(); ii++)
		{
			bRc = m_arRc.GetAt(ii);
			if (!bRc.PtInRect(point))
				continue;

			if (ii == MIDX_MAX && m_mainframe->IsZoomed())
				return MIDX_RESTORE;
			else	return ii;
		}
	}
	else
	{
		for (int ii = 0; ii < m_arRc.GetSize(); ii++)
		{
			bRc = m_arRc.GetAt(ii);
			if (!bRc.PtInRect(point))
				continue;

			switch (ii)
			{
			case IDX_SINGLE:
				if (m_sdiMode)	return IDX_PIN;
				return m_bIconic ? - 1: ii;
			case IDX_MAX:
				if (m_bMax)	return IDX_RESTORE;
			case IDX_PIN:
			case IDX_ICON:
			case IDX_CLOSE:	return ii;
			case IDX_MIN:	return m_bIconic ? IDX_RESTORE : IDX_MIN;
			case IDX_GROUP:	case IDX_COPY:
//			case IDX_HELP:	
			case IDX_FONT:	return m_bIconic ? -1 : ii;
			case IDX_CAPTURE : return ii;
			}
		}
	}
	return -1;
}

void CXCaption::SetTitle(CString title)
{
	m_title = title;
	GetParent()->SetWindowText(title);
	Invalidate(FALSE);
}

bool CXCaption::changePin()
{
	m_pinON = !m_pinON;

	CClientDC	dc(this);
	DrawBitmap(&dc, IDX_PIN, m_arRc.GetAt(IDX_PIN), m_activeCap);
	return m_pinON;
}

void CXCaption::HideButton(DWORD flag)
{
	m_hideflag |= flag;
	SetRegion();
	Invalidate();
}

void CXCaption::drawTitleBitmap(CDC *pDC, CRect drawRC, BOOL bActive)
{
	//if (bActive || m_title.Find(INTERMAP) >= 0)
	if (bActive)
		m_bmVS[0].DrawTilte(pDC, drawRC);
	else
		m_bmCaption[1].DrawTilte(pDC, drawRC);


	drawRC.left = drawRC.right - 70;
//	if (bActive || m_title.Find(INTERMAP) >= 0)
	if (bActive)
		m_bmCapright[0].DrawTilte(pDC, drawRC);
	else
		m_bmCapright[1].DrawTilte(pDC, drawRC);
	/*
	if (bActive)
		m_bmVS[0].Draw(pDC, drawRC);
	else
		m_bmCaption[1].Draw(pDC, drawRC);
		*/
}

void CXCaption::OnTimer(UINT_PTR nIDEvent) 
{
	if (nIDEvent == CAPTION_TIMER)
	{
		KillTimer(nIDEvent);
		GetParent()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	
	CControlBar::OnTimer(nIDEvent);
}
