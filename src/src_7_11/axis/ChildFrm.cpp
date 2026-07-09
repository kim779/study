// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "axis.h"
#include "ChildFrm.h"
#include "axmsg.hxx"
#include "axtabview.h"
#include "axMisc.h"
#include "Linkbar.h"
#include "../h/axisvar.h"
#include "../h/axisfire.h"
#include "../appl/appl.h"

#define _WINIX

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	ON_WM_SIZING()
	ON_WM_MDIACTIVATE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SETCURSOR()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_AXIS, OnAXIS)
	ON_MESSAGE(WM_CHILDMSG, OnCHILDMSG)
	ON_MESSAGE(WM_LINKOPEN, OnLinkOpen)
	ON_COMMAND_RANGE(ID_GROUP_BASE, ID_GROUP_END, OnGroup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction


CChildFrame::CChildFrame()
{
	m_axiscall	= NULL;
	m_bActive	= FALSE;
	m_axisres	= NULL;
	m_linkBar	= NULL;

	m_vwTYPE	= 0;
	m_key		= 0;
	m_cursor	= 0;
	m_tView		= false;
	m_bIconic	= false;
	m_bCaption	= true;
	m_suspend	= false;
	m_bSingle	= false;
	m_fixW		= false;
	m_fixH		= false;

	m_iconpos	= CPoint(-1, -1);
	m_rectSDI.SetRectEmpty();

	m_wndHook = NULL;
	m_enableHook = false;

	m_szType = 0;

	m_cxBorder  = GetSystemMetrics(SM_CXBORDER);
	m_cyBorder  = GetSystemMetrics(SM_CYBORDER);
	m_cxEdge    = GetSystemMetrics(SM_CXFRAME);
	m_cyEdge    = GetSystemMetrics(SM_CYFRAME);
	m_cyCaption = GetSystemMetrics(SM_CYCAPTION);

	m_strSetConfig = _T("");

	m_level = osLevel();
}

CChildFrame::~CChildFrame()
{
	if (m_linkBar)	delete m_linkBar;
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
LRESULT CChildFrame::OnLinkOpen(WPARAM wp, LPARAM lp)
{
	char*	mapN = (char *) lp;
	if (wp)		// show link edit
		AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axLINKEDIT, 0), lp);
	else	(*m_axiscall)(AXI_LINKOPEN, MAKEWPARAM(m_xcaption.GetGroup(), m_key), lp);
	return 0;
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	long cnt = (long)lpCreateStruct->hMenu;
	if (cnt == 0x0000ff00)
	{
		cnt -= 0x0000ff00;
		lpCreateStruct->x = cnt*(GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CXDLGFRAME));
		lpCreateStruct->y = cnt*(GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYDLGFRAME));
	}
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)	return -1;
	//if (cnt == 0x0000ff00)
	SetWindowPos(NULL, lpCreateStruct->x, lpCreateStruct->y, lpCreateStruct->cx, lpCreateStruct->cy, lpCreateStruct->style);

	WINDOWPLACEMENT pl;	GetWindowPlacement(&pl);

	m_SP = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
	CRect	wRc;
	GetParent()->GetClientRect(wRc);
	if (m_SP.x < 0)	m_SP.x = 0;
	if (m_SP.y < 0)	m_SP.y = 0;
	if (m_SP.x >= wRc.right)	m_SP.x = 0;
	if (m_SP.y >= wRc.bottom)	m_SP.y = 0;
	MoveWindow(-POS_CHILDMOVE, -POS_CHILDMOVE, lpCreateStruct->cx, lpCreateStruct->cy);
	
	m_xcaption.Create(NULL, NULL, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_TOP,
		         CRect(0,0,XCAP_HEIGHT,XCAP_HEIGHT), this, AFX_IDW_STATUS_BAR);
	m_xcaption.SetBarStyle(CBRS_ALIGN_TOP);

	ModifyStyle(WS_CAPTION, 0, SWP_DRAWFRAME);

	//ModifyStyleEx(WS_EX_APPWINDOW, 0, SWP_FRAMECHANGED); 
	return 0;
}

CSize CChildFrame::GetSize()
{
	return CSize(m_width, m_height);
}

void CChildFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CRect*	prect = (CRect *) pRect;
	int	vtype = m_vwTYPE & vtypeMSK;

	switch (vtype)
	{
	case vtypeFIX:
		if (m_bIconic)	break;
		if (prect->Width() <= m_width)
		{
			switch (fwSide)
			{
			case WMSZ_RIGHT:case WMSZ_TOPRIGHT:case WMSZ_BOTTOMRIGHT:
				prect->right = prect->left + m_width;	break;
			case WMSZ_LEFT:case WMSZ_TOPLEFT:case WMSZ_BOTTOMLEFT:
				prect->left = prect->right - m_width;	break;
			}
		}
		if (prect->Height() <= m_height)
		{
			switch (fwSide)
			{
			case WMSZ_TOP:case WMSZ_TOPLEFT:case WMSZ_TOPRIGHT:
				prect->top = prect->bottom - m_height;	break;
			default:prect->bottom = prect->top + m_height;	break;
			}
		}
// 		break;
// 	case vtypeWND:
// 		break;
	case vtypeSCR:
	case vtypeFEX:
		if (m_bIconic)	break;
		if (prect->Width() > m_width)
		{
			switch (fwSide)
			{
			case WMSZ_RIGHT:case WMSZ_TOPRIGHT:case WMSZ_BOTTOMRIGHT:
				prect->right = prect->left + m_width;	break;
			case WMSZ_LEFT:case WMSZ_TOPLEFT:case WMSZ_BOTTOMLEFT:
				prect->left = prect->right - m_width;	break;
			}
		}
		if (prect->Height() < m_height)
		{
			switch (fwSide)
			{
			case WMSZ_TOP:case WMSZ_TOPLEFT:case WMSZ_TOPRIGHT:
				prect->top = prect->bottom - m_height;	break;
			default:prect->bottom = prect->top + m_height;	break;
			}
		}
		break;
	case vtypeRSZ:
		{
			switch(m_szType)
			{
			case apFIXEDW:
				if (prect->Width() != m_width)
				{
					if (fwSide == WMSZ_LEFT)
						prect->left = prect->right - m_width;
					else
						prect->right = prect->left + m_width;
				}
				if (m_fixH && prect->Height() != m_height)
				{
					switch (fwSide)
					{
					case WMSZ_TOP:case WMSZ_TOPLEFT:case WMSZ_TOPRIGHT:
						prect->top = prect->bottom - m_height;	break;
					default:prect->bottom = prect->top + m_height;	break;
					}
				}
				break;
			case apFIXEDH:
				if (m_fixW && prect->Width() != m_width)
				{
					switch (fwSide)
					{
					case WMSZ_RIGHT:case WMSZ_TOPRIGHT:case WMSZ_BOTTOMRIGHT:
						prect->right = prect->left + m_width;	break;
					default:prect->left = prect->right - m_width;	break;
					}
				}
				if (prect->Height() != m_height)
				{
					switch (fwSide)
					{
					case WMSZ_TOP:
					case WMSZ_TOPLEFT:
					case WMSZ_TOPRIGHT:
						pRect->top = pRect->bottom - m_height;
						break;
					default:
						pRect->bottom = pRect->top + m_height;
						break;
					}
				}
				break;
			default:
				if (m_fixW && prect->Width() != m_width)
				{
					switch (fwSide)
					{
					case WMSZ_RIGHT:case WMSZ_TOPRIGHT:case WMSZ_BOTTOMRIGHT:
						prect->right = prect->left + m_width;	break;
					default:prect->left = prect->right - m_width;	break;
					}
				}
				if (m_fixH && prect->Height() != m_height)
				{
					switch (fwSide)
					{
					case WMSZ_TOP:case WMSZ_TOPLEFT:case WMSZ_TOPRIGHT:
						prect->top = prect->bottom - m_height;	break;
					default:prect->bottom = prect->top + m_height;	break;
					}
				}
				break;
			}
		}
		break;
	default:break;
	}
	CMDIChildWnd::OnSizing(fwSide, pRect);
}

void CChildFrame::SetMisc(UINT msg, CWnd* parent, bool (*axiscall)(int, WPARAM, LPARAM),
				int fontsize, class _axisRes* axisres, bool bCaption)
{
	m_axiscall = axiscall;
	m_bCaption = bCaption;
	m_axisres  = axisres;

	
	if (m_bCaption)
	{
		m_xcaption.SetMisc(parent, msg, m_key, fontsize, axisres, false);
		(*m_axiscall)(AXI_HIDECAPTIONBUTTON, 0, m_key);
	}

	SendMessage(WM_NCPAINT, (WPARAM) 1);
}

void CChildFrame::OnGroup(UINT ua)
{
}

BOOL CChildFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int	index = LOWORD(wParam);
	if (index >= ID_GROUP_BASE && index < ID_GROUP_END)
	{
		if (runCommand(LOWORD(wParam), lParam))	return TRUE;
		return CMDIChildWnd::OnCommand(wParam, lParam);
	}
	
	return CMDIChildWnd::OnCommand(wParam, lParam);
}

bool CChildFrame::runCommand(int comm, LPARAM lParam)
{
	int	triggerN = comm - ID_GROUP_BASE;
	switch (triggerN)
	{
	case 0:		// none
	case 1:case 2:case 3:case 4:case 5:case 99:
		(*m_axiscall)(AXI_SETTRIGGER, m_key, triggerN == 99 ? -1 : triggerN);
		break;
	default:return false;
	}
	return true;
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

LRESULT CChildFrame::OnAXIS(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case axCLOSEX:	break;
	case axWAIT:
		m_cursor = (int) lParam;
		if (m_cursor)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		else	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	default:
		AfxGetMainWnd()->SendMessage(WM_AXIS, wParam, lParam);
		break;

	case apHOOKED:
		if (lParam)
		{
			m_enableHook = HIWORD(wParam) ? true : false;
			m_wndHook = (CWnd *) lParam;
		}
		break;

	case apFIXEDA:	case apFIXEDW:	case apFIXEDH:
		{
			m_szType  = LOWORD(wParam);
			//m_width = LOWORD(lParam) + (m_cxBorder*4) + (m_cxEdge*2);
			//m_height= HIWORD(lParam) + m_cyCaption + (m_cyBorder*4) + (m_cyEdge*2);
		}
		break;
	case apSETTG:
	case apPINCH:
		return AfxGetMainWnd()->SendMessage(WM_AXIS, wParam, lParam);

	case apSETCONFIG:
		m_strSetConfig = (char*) lParam;
		break;
	}
	return 0;
}

LRESULT CChildFrame::OnCHILDMSG(WPARAM wParam, LPARAM lParam)
{
	AfxGetMainWnd()->SendMessage(WM_CHILDMSG, wParam, lParam);
	return 0;
}

void CChildFrame::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	int	vtype = m_vwTYPE & vtypeMSK;
	switch (vtype)
	{
	case vtypeFIX:
		if (m_bIconic)	break;
		if (lpwndpos->cx != m_width)	lpwndpos->cx = m_width;
		if (lpwndpos->cy != m_height)	lpwndpos->cy = m_height;
		break;
	case vtypeSCR:
	case vtypeFEX:
		if (m_bIconic)	break;
		if (lpwndpos->cx > m_width)	lpwndpos->cx = m_width;
		if (lpwndpos->cy > m_height)	lpwndpos->cy = m_height;
		break;
	default:break;
	}
	CMDIChildWnd::OnWindowPosChanging(lpwndpos);
}

BOOL CChildFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_cursor == 1)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		return TRUE;	
	}
	return CMDIChildWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CChildFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	if (m_bIconic && !m_suspend && !(lpwndpos->flags & SWP_NOMOVE))
	{
		m_iconpos = CPoint(lpwndpos->x, lpwndpos->y);
	}
	
	if(m_mapN == INTERMAP)
	{
		CPoint cp(lpwndpos->x, lpwndpos->y);
		AfxGetMainWnd()->PostMessage(WM_AXIS, axINTERMOVE, (LPARAM)&cp);
	}
	
	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
}

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);

	if (m_level >= 6)	// windows vista
	{	
		CRect	clientRC;
		int	frameSZ = m_cxEdge - m_cxBorder;

		GetClientRect(&clientRC); 
		clientRC.DeflateRect(frameSZ, frameSZ);
		//m_rgn = ::CreateRectRgn(clientRC.left, clientRC.top, clientRC.right+(frameSZ*2), clientRC.bottom+(frameSZ*2));
		m_rgn = ::CreateRectRgn(clientRC.left, clientRC.top, clientRC.right+(frameSZ*2) + 1, clientRC.bottom+(frameSZ*2) + 1);
		SetWindowRgn(m_rgn, TRUE);
	}

	if (m_tView)
	{
		CView*	view = GetActiveView();
		((CAxTabView *)view)->JustWindow(m_mapN, true);
	}	
}

void CChildFrame::OnNcPaint() 
{
	CString str;
	CMDIChildWnd::OnNcPaint();
	CWindowDC dc(this);
/*
	CRect		winRc;
	GetWindowRect(&winRc);
	winRc.OffsetRect(-winRc.left, -winRc.top);
	CBrush brush(FRAMELINE);
	dc.FrameRect(&winRc, &brush);
	
	if(m_bActive && m_mapN == _T("TT111111"))
	{
		CBrush brush(FRAMELINE);
		dc.FrameRect(&winRc, &brush);
		CString str;
		str.Format(_T("active %d %d %d %d \n"), winRc.left, winRc.top, winRc.right, winRc.bottom);
		AfxGetMainWnd()->SendMessage(WM_TRACE, 0, (LPARAM)str.operator LPCSTR());
	}
	else if(!m_bActive && m_mapN == _T("TT111111"))
	{
		if (m_level >= 6)	// windows vista
		{	
			CRect	clientRC;
			int	frameSZ = m_cxEdge - m_cxBorder;

			GetClientRect(&clientRC); 
			//clientRC.DeflateRect(frameSZ, frameSZ);
			//m_rgn = ::CreateRectRgn(clientRC.left, clientRC.top, clientRC.right+(frameSZ*2), clientRC.bottom+(frameSZ*2));
			m_rgn = ::CreateRectRgn(clientRC.left, clientRC.top, clientRC.right , clientRC.bottom );
			SetWindowRgn(m_rgn, TRUE);
			CString str;
			clientRC.OffsetRect(-clientRC.left, -clientRC.top);
			str.Format(_T("active %d %d %d %d \n"), clientRC.left, clientRC.top, clientRC.right, clientRC.bottom);
			AfxGetMainWnd()->SendMessage(WM_TRACE, 0, (LPARAM)str.operator LPCSTR());
		}
		CBrush brush(RGB(255,0,0));
		GetWindowRect(&winRc);
		winRc.OffsetRect(-winRc.left, -winRc.top);
		dc.FrameRect(&winRc, &brush);
	}
	*/
	DrawFrame(&dc);
}

BOOL CChildFrame::OnNcActivate(BOOL bActive) 
{
	m_xcaption.ActiveateApp(bActive);
	m_bActive = bActive;
	OnNcPaint();
	return CMDIChildWnd::OnNcActivate(bActive);
}

void CChildFrame::DrawFrame(CDC* pDC) 
{
	if(m_axisres == NULL)
		return;

	CRect		winRc, rc;
	COLORREF	color;
	CArray	<COLORREF, COLORREF > arColor;
	GetWindowRect(winRc);

	winRc -= winRc.TopLeft();
	rc = winRc;
	if (m_axisres)
	{
		if (m_bActive)
		{
			color = m_axisres->GetColor(ACTFLINE_1);	arColor.Add(color);
			color = m_axisres->GetColor(ACTFLINE_2);	arColor.Add(color);
			color = m_axisres->GetColor(ACTFLINE_3);	arColor.Add(color);
		}
		else
		{
			color = m_axisres->GetColor(INACTFLINE_1);	arColor.Add(color);
			color = m_axisres->GetColor(INACTFLINE_2);	arColor.Add(color);
			color = m_axisres->GetColor(INACTFLINE_3);	arColor.Add(color);
		}

		color = m_axisres->GetColor(INACTFLINE_2);
	}

	int xgap = GetSystemMetrics(SM_CXFRAME);
	while (arColor.GetSize() < xgap)
	{
		arColor.Add(color);
	}

	//rc.InflateRect(5,5);
	/*
	CString tmps;
	if(m_bActive)
	{
		tmps.Format(_T("activate %d %d %d %d\n"), rc.left, rc.top, rc.right, rc.bottom);
		TRACE(tmps);
	}
	else
			{
		tmps.Format(_T("inactivate %d %d %d %d\n"), rc.left, rc.top, rc.right, rc.bottom);
		TRACE(tmps);
	}
	*/
	for (int ii = 0; ii < arColor.GetSize(); ii++)
	{
		color = arColor.GetAt(ii);
		if(m_bActive)
			pDC->Draw3dRect(rc,FRAMELINE, FRAMELINE);
		else
			pDC->Draw3dRect(rc,RGB(103, 108,122), RGB(103, 108,122));
	//	pDC->Draw3dRect(rc, RGB(58,65, 80), RGB(58,65, 80));
		rc.DeflateRect(1, 1);
	}
	arColor.RemoveAll();
}

void CChildFrame::ChangeSkin(CString skinName)
{
	m_xcaption.ChangeSkin();
	SendMessage(WM_NCPAINT, (WPARAM) 1);
	GetActiveView()->SendMessage(WM_NCPAINT, (WPARAM) 1);
	m_xcaption.Invalidate(FALSE);

	if (m_linkBar)
		m_linkBar->ChangeSkin(skinName);
}

void CChildFrame::actionCaption(int action, CPoint pt)
{
	int		vtype = 0;
	WINDOWPLACEMENT	pl;
	GetWindowPlacement(&pl);

	switch (action)
	{
	case IDX_MIN:
		{
			m_suspend = true;
			int cx = GetSystemMetrics(SM_CXMINIMIZED);
			int cy = GetSystemMetrics(SM_CYMINIMIZED);
			cy = GetSystemMetrics(SM_CYFRAME) + XCAP_HEIGHT;//GetSystemMetrics(SM_CYMINIMIZED);
			cy += GetSystemMetrics(SM_CYBORDER);
			
			if (!m_xcaption.IsMax())
			{
				m_restoreRc.SetRect(pl.rcNormalPosition.left, pl.rcNormalPosition.top,
						pl.rcNormalPosition.right, pl.rcNormalPosition.bottom);
			}
			SetMinimize();
			SetWindowPos(&wndBottom, pt.x, pt.y, cx, cy, SWP_SHOWWINDOW);
			m_suspend = false;
		}
		break;
	case IDX_RESTORE:
		m_suspend = true;
		SetRestore();
		SetWindowPos(&wndTop, m_restoreRc.left, m_restoreRc.top, m_restoreRc.Width(),
				m_restoreRc.Height(), SWP_SHOWWINDOW);
		m_suspend = false;
		break;
	case IDX_MAX:
		m_suspend = true;
		vtype = m_vwTYPE & vtypeMSK;
		switch (vtype)
		{
		case vtypeSCR:case vtypeFIX:case vtypeFEX:
			if (m_bIconic)
				SetWindowPos(NULL, m_restoreRc.left, m_restoreRc.top, m_width, m_height, SWP_SHOWWINDOW);
			else	SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_SHOWWINDOW|SWP_NOMOVE);
			break;
		case vtypeRSZ:
			if (m_fixH || m_fixW)	break;
		default:
			{
				CRect	cRc;
				GetParent()->GetClientRect(cRc);
				if (!m_bIconic)
				{
					m_restoreRc.SetRect(pl.rcNormalPosition.left, pl.rcNormalPosition.top,
						pl.rcNormalPosition.right, pl.rcNormalPosition.bottom);
				}
				SetMax();
				SetWindowPos(NULL, 0, 0, cRc.Width(), cRc.Height(), SWP_SHOWWINDOW);
			}
			break;
		}
		if (m_bIconic)
		{
			m_bIconic = false;
			m_xcaption.SetIconic(m_bIconic);
			ModifyStyle(WS_DLGFRAME, WS_THICKFRAME, SWP_DRAWFRAME);
		}

		m_suspend = false;
		break;
	}
}

void CChildFrame::SetMinimize()
{
	m_bIconic = true;
	m_xcaption.SetIconic(m_bIconic);
	m_xcaption.SetMax(false);
	ModifyStyle(WS_THICKFRAME, WS_DLGFRAME, SWP_DRAWFRAME);
}

void CChildFrame::SetRestore()
{
	m_bIconic = false;
	m_xcaption.SetIconic(m_bIconic);
	m_xcaption.SetMax(false);
	ModifyStyle(WS_DLGFRAME, WS_THICKFRAME, SWP_DRAWFRAME);
}

void CChildFrame::SetMax()
{
	m_bIconic = false;
	m_xcaption.SetIconic(m_bIconic);
	m_xcaption.SetMax(true);
	ModifyStyle(WS_DLGFRAME, WS_THICKFRAME, SWP_DRAWFRAME);
}

bool CChildFrame::createLink(class CTMenu* menu, CString mapN, bool (*axiscall)(int, WPARAM, LPARAM),
			     CString home, CString userName, CString skinName, bool bShow)
{
	CString	mapname = mapN.Left(L_MAPN);
	CString	tail = mapname.Mid(6, 2);
	if (tail != "00") bShow = false;

	m_linkBar = new CLinkbar(menu, mapname, axiscall, home, userName, skinName);
	UINT nFlags = WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_BOTTOM;
	if (!m_linkBar->Create(NULL, NULL, nFlags, CRect(0,0,LBAR_HEIGHT,LBAR_HEIGHT), this, AFX_IDW_STATUS_BAR))
	{
		delete m_linkBar;
		m_linkBar = NULL;
		return false;
	}

	m_linkBar->SetBarStyle(CBRS_ALIGN_BOTTOM);
	ShowControlBar(m_linkBar, bShow ? TRUE : FALSE, FALSE);
	return true;
}

LRESULT CChildFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case SC_MAXIMIZE:
			actionCaption(IDX_MAX, CPoint(0, 0));
			return TRUE;
		}
		break;
	}
			
	return CMDIChildWnd::WindowProc(message, wParam, lParam);
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	 
	return CMDIChildWnd::PreCreateWindow(cs);
}

long CChildFrame::runHookedKey(WPARAM wParam, LPARAM lParam)
{
	if (m_enableHook && m_wndHook)
		return (long)m_wndHook->SendMessage(WM_AXCHILD, (WPARAM)wParam, (LPARAM)lParam);
	return 0;
}


void CChildFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_xcaption.IsVisible())
	{
		DefWindowProc(WM_NCLBUTTONDOWN, HTCAPTION, 0);
		return;
	}	
	CMDIChildWnd::OnLButtonDown(nFlags, point);
}

void CChildFrame::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (point == CPoint(-1, -1))
	{
		if(!nFlags)
			m_xcaption.ShowWindow(SW_HIDE);
		else
			m_xcaption.ShowWindow(SW_SHOW);

		ModifyStyleEx(WS_EX_DLGMODALFRAME, 0, SWP_DRAWFRAME);

		CRect rc, winRc;;
		GetClientRect(rc);
		GetWindowRect(winRc);

		SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(),SWP_NOMOVE);
		SetWindowPos(NULL, winRc.left, winRc.top, winRc.Width(), winRc.Height(),SWP_NOMOVE|SWP_DRAWFRAME);
		return;
	}
	
	CMDIChildWnd::OnLButtonDblClk(nFlags, point);
}

void CChildFrame::OnClose() 
{
	SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
	CMDIChildWnd::OnClose();
}

int CChildFrame::osLevel()
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;
	
	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	//
	// If that fails, try using the OSVERSIONINFO structure.
	
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	
	if(!(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)))
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return 0;
	}
	
	return osvi.dwMajorVersion;
}
