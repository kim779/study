// SChild.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "SChild.h"
#include "axMsg.hxx"
#include "axtabview.h"
#include "axMisc.h"
#include "Linkbar.h"

#include "../h/axisfire.h"
#include "../appl/appl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define Y_POS	107
/////////////////////////////////////////////////////////////////////////////
// CSChild

//IMPLEMENT_DYNCREATE(CSChild, CFrameWnd)

CSChild::CSChild(CWnd* mainframe, bool stealth)
{
	m_mainframe	= mainframe;
	m_bstealth	= stealth;
	m_axisres	= NULL;
	m_bIconic	= false;
	m_tView		= false;
	m_sdiMode	= false;
	m_bCaption	= true;
	m_fixW		= false;
	m_fixH		= false;
	m_linkBar	= NULL;
	m_cursor	= 0;

	m_mdiPt		= CPoint(0, 0);
	m_rectMDI.SetRectEmpty();
	m_restoreRc.SetRectEmpty();

	m_enableHook = false;
	m_wndHook = NULL;

	m_cxBorder  = GetSystemMetrics(SM_CXBORDER);
	m_cyBorder  = GetSystemMetrics(SM_CYBORDER);
	m_cxEdge    = GetSystemMetrics(SM_CXFRAME);
	m_cyEdge    = GetSystemMetrics(SM_CYFRAME);
	m_cyCaption = GetSystemMetrics(SM_CYCAPTION);

	m_strSetConfig = _T("");

	m_level = osLevel();
}

CSChild::~CSChild()
{
	if (m_linkBar)	delete m_linkBar;
}


BEGIN_MESSAGE_MAP(CSChild, CFrameWnd)
	//{{AFX_MSG_MAP(CSChild)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_WM_SIZING()
	ON_WM_SIZE()  
	ON_WM_NCPAINT()
	ON_WM_SETCURSOR()
	ON_WM_SYSCOMMAND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_AXIS, OnAXIS)
	ON_MESSAGE(WM_CHILDMSG, OnCHILDMSG)
	ON_MESSAGE(WM_LINKOPEN, OnLinkOpen)
	ON_WM_NCCALCSIZE()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSChild message handlers

LPARAM CSChild::OnLinkOpen(WPARAM wp, LPARAM lp)
{
	char*	mapN = (char *) lp;
	if (wp)		// show link edit
		m_mainframe->SendMessage(WM_AXIS, MAKEWPARAM(axLINKEDIT, 0), lp);
	else	(*m_axiscall)(AXI_LINKOPEN, MAKEWPARAM(m_xcaption.GetGroup(), m_key), lp);
	return 0;
}

LPARAM CSChild::OnAXIS(WPARAM wParam, LPARAM lParam)
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
		m_mainframe->SendMessage(WM_AXIS, wParam, lParam);
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

LPARAM CSChild::OnCHILDMSG(WPARAM wParam, LPARAM lParam)
{
	m_mainframe->SendMessage(WM_CHILDMSG, wParam, lParam);

	return 0;
}

int CSChild::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_xcaption.Create(NULL, NULL, WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_TOP,		    
		         CRect(0,0,XCAP_HEIGHT,XCAP_HEIGHT), this, AFX_IDW_STATUS_BAR);
	m_xcaption.SetBarStyle(CBRS_ALIGN_TOP);
	m_xcaption.SetSingle();

	m_defaultpos = CPoint(lpCreateStruct->x, lpCreateStruct->y + Y_POS);
	m_defaultpos.y += GetSystemMetrics(SM_CYCAPTION) - 19;
	CRect	wRc;
	GetDesktopWindow()->GetWindowRect(wRc);
	if (m_defaultpos.x < wRc.left)	m_defaultpos.x = wRc.left;
	if (m_defaultpos.y < wRc.top)	m_defaultpos.y = wRc.top;
	if (m_defaultpos.x >= wRc.right)	m_defaultpos.x = wRc.left;
	if (m_defaultpos.y >= wRc.bottom)	m_defaultpos.y = wRc.top;
	SetWindowPos(NULL, 10000, 10000, 0, 0, SWP_HIDEWINDOW);
	ModifyStyle(WS_CAPTION|WS_SYSMENU, 0, SWP_DRAWFRAME);
	return 0;
}

void CSChild::OnClose() 
{
	SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
	CFrameWnd::OnClose();
}

void CSChild::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	if (m_bActive == (nState != WA_INACTIVE))
		return;

	switch (nState)
	{
	case WA_INACTIVE:	m_bActive = false;	break;
	default:		m_bActive = true;	break;
	}
	m_xcaption.ActiveateApp(m_bActive);
	//OnNcPaint();
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}

void CSChild::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CFrameWnd::OnSizing(fwSide, pRect);
	
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
		if (prect->Height() > m_height)
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
			break;
	default:break;
	}
	
}

void CSChild::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	
	if (m_level >= 6)	// windows vista
	{
		CRect	clientRC;
		int	frameSZ = m_cxEdge - m_cxBorder;

		GetClientRect(&clientRC); 
		clientRC.DeflateRect(frameSZ, frameSZ);
		m_rgn = ::CreateRectRgn(clientRC.left, clientRC.top, clientRC.right+(frameSZ*2) + 1, clientRC.bottom+(frameSZ*2) + 2);
		SetWindowRgn(m_rgn, TRUE);
	}
	

	if (m_tView)
	{
		CView*	view = GetActiveView();
		((CAxTabView *)view)->JustWindow(m_mapN, true);
	}
}

BOOL CSChild::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!m_bstealth)
		return TRUE;//CFrameWnd::PreCreateWindow(cs);
	if (!::IsWindow(m_wndInvisible.m_hWnd))
	{
		UINT	nRes = IDI_CLIENT;
		LPCTSTR pstrOwnerClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW, ::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH) ::GetStockObject(WHITE_BRUSH),	AfxGetApp()->LoadIcon(nRes));
		if (!m_wndInvisible.CreateEx(0, pstrOwnerClass, _T(""), WS_POPUP,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, 0))
			return FALSE;
		LONG style = GetWindowLong(m_wndInvisible.GetSafeHwnd(), GWL_EXSTYLE);
		style = WS_EX_TOOLWINDOW;
		SetWindowLong(m_wndInvisible.GetSafeHwnd(), GWL_EXSTYLE, style);
	}
	cs.hwndParent = m_wndInvisible.m_hWnd;

	return TRUE;//CFrameWnd::PreCreateWindow(cs);
	//return CFrameWnd::PreCreateWindow(cs);
}

void CSChild::SetMisc(UINT msg, CWnd* parent, bool (*axiscall)(int, WPARAM, LPARAM),
			  int fontsize, bool sdiMode, class _axisRes* axisres, bool bCaption)
{
	m_axiscall = axiscall;
	m_bCaption = bCaption;
	m_axisres  = axisres;
	m_sdiMode  = sdiMode;

	if (m_bCaption)
	{
		m_xcaption.SetMisc(parent, msg, m_key, fontsize, axisres, m_sdiMode);
		(*m_axiscall)(AXI_HIDECAPTIONBUTTON, 0, m_key);
	}

	SendMessage(WM_NCPAINT, (WPARAM) 1);
}

void CSChild::OnNcPaint() 
{
	CFrameWnd::OnNcPaint();
//	CWindowDC dc(this);
//	DrawFrame(&dc);
}

void CSChild::DrawFrame(CDC* pDC) 
{
	/*
	if (GetStyle() & WS_CAPTION)
	{
		SetWindowRgn(0, FALSE);
	}
	
	CRect winRc;
	GetWindowRect(winRc);
*/
	
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

	for (int ii = 0; ii < arColor.GetSize(); ii++)
	{
		color = arColor.GetAt(ii);
		pDC->Draw3dRect(rc, color, color);
		rc.DeflateRect(1, 1);
	}
	arColor.RemoveAll();
	
}

void CSChild::ChangeSkin(CString skinName)
{
	m_xcaption.ChangeSkin();
	SendMessage(WM_NCPAINT, (WPARAM) 1);
	GetActiveView()->SendMessage(WM_NCPAINT, (WPARAM) 1);
	m_xcaption.Invalidate(FALSE);

	if (m_linkBar)
		m_linkBar->ChangeSkin(skinName);
}

CView* CSChild::AttachView(CCreateContext context)
{
	CView* pNewView = STATIC_DOWNCAST(CView, CreateView(&context));
	if (pNewView != NULL)
	{
		pNewView->ShowWindow(SW_SHOW);
		pNewView->OnInitialUpdate();
		SetActiveView(pNewView);
		RecalcLayout();
		//SetOwner(NULL);
	}
	return pNewView;
}

void CSChild::sMax()
{
	WINDOWPLACEMENT	pl;
	GetWindowPlacement(&pl);
	
	int vtype = m_vwTYPE & vtypeMSK;
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
			GetDesktopWindow()->GetWindowRect(cRc);
			if (!m_bIconic)
			{
				m_restoreRc.SetRect(pl.rcNormalPosition.left, pl.rcNormalPosition.top,
					pl.rcNormalPosition.right, pl.rcNormalPosition.bottom);
			}
			SetWindowPos(NULL, 0, 0, cRc.Width(), cRc.Height(), SWP_SHOWWINDOW);
		}
		break;
	}
}

void CSChild::SetRestore()
{
	SetWindowPos(&wndTop, m_restoreRc.left, m_restoreRc.top, 
			m_restoreRc.Width(), m_restoreRc.Height(), SWP_SHOWWINDOW);

	m_bIconic = false;
	m_xcaption.SetIconic(m_bIconic);
	m_xcaption.SetMax(false);
	m_xcaption.Invalidate(FALSE);
}

void CSChild::SetMax()
{
	int	vtype = m_vwTYPE & vtypeMSK;
	CRect	wRc;
	GetWindowRect(wRc);

	switch (vtype)
	{
	case vtypeSCR:case vtypeFIX:case vtypeFEX:
		if (m_bIconic)
			SetWindowPos(&wndTop, m_restoreRc.left, m_restoreRc.top, m_width, m_height, SWP_SHOWWINDOW);
		else
		{
			m_restoreRc.CopyRect(&wRc);
			SetWindowPos(&wndTop, 0, 0, m_width, m_height, SWP_SHOWWINDOW|SWP_NOMOVE);
		}
		break;
	case vtypeRSZ:
		if (m_fixH || m_fixW)	break;
	default:
		{
			CRect	cRc, workArea;
			GetWindowRect(cRc);
			if (!m_bIconic)		m_restoreRc.CopyRect(&cRc);
			
			SystemParametersInfo(SPI_GETWORKAREA, 0, (LPVOID)&workArea, 0);
			SetWindowPos(&wndTop, 0, 0, workArea.Width(), workArea.Height() - 2, SWP_SHOWWINDOW);
		}
		break;
	}

	m_bIconic = false;
	m_xcaption.SetIconic(m_bIconic);
	m_xcaption.SetMax(true);
	m_xcaption.Invalidate(FALSE);
}


bool CSChild::createLink(class CTMenu* menu, CString mapN, bool (*axiscall)(int, WPARAM, LPARAM),
			 CString home, CString userName, CString skinName, bool bShow)
{
	CString	mapname = mapN.Left(8);
	CString	tail = mapname.Mid(6, 2);
	if (tail != "00") bShow = false;
	
	m_linkBar = new CLinkbar(menu, mapN, axiscall, home, userName, skinName);
	UINT nFlags = WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_BOTTOM;
	if (!m_linkBar->Create(NULL, NULL, nFlags, CRect(0,0,0,LBAR_HEIGHT), this, AFX_IDW_STATUS_BAR))
	{
		delete m_linkBar;
		m_linkBar = NULL;
		return false;
	}

	m_linkBar->SetBarStyle(CBRS_ALIGN_BOTTOM);
	ShowControlBar(m_linkBar, bShow ? TRUE : FALSE, FALSE);
	return true;
}

#include "MainFrm.h"

BOOL CSChild::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CMainFrame* pFrame =(CMainFrame*) AfxGetMainWnd();
	if (pFrame)
	{
		if (pFrame->m_nFlags &WF_MODALDISABLE)
			return FALSE;	
		pFrame->m_pFormMgr->EnableChild(TRUE);
	}

	if (m_cursor == 1)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		return TRUE;	
	}
	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CSChild::OnSysCommand(UINT nID, LPARAM lParam) 
{
	CFrameWnd::OnSysCommand(nID, lParam);

	switch (nID)
	{
	case SC_MINIMIZE:
		m_bIconic = true;
		m_xcaption.SetIconic(m_bIconic);
		m_xcaption.SetMax(false);
		m_xcaption.Invalidate(FALSE);
	//	SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);
		break;
	case 0xF122:	
	case SC_RESTORE:
		if (m_bIconic)
		{
			if (m_restoreRc.IsRectEmpty())
			{
				WINDOWPLACEMENT	pl;
				GetWindowPlacement(&pl);
				m_restoreRc.SetRect(pl.rcNormalPosition.left, pl.rcNormalPosition.top,
					pl.rcNormalPosition.right, pl.rcNormalPosition.bottom);
			}
			SetWindowPos(&wndTop, m_restoreRc.left, m_restoreRc.top,
				m_restoreRc.Width(), m_restoreRc.Height(), SWP_SHOWWINDOW);
		}
		m_bIconic = false;
		m_xcaption.SetIconic(m_bIconic);
		m_xcaption.SetMax(false);
		m_xcaption.Invalidate(FALSE);
		break;
	}
}

LRESULT CSChild::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case SC_MAXIMIZE:
			SetMax();
			return TRUE;
		}
		break;
	}
	
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

long CSChild::runHookedKey(WPARAM wParam, LPARAM lParam)
{
	if (m_enableHook && m_wndHook)
		return (long)m_wndHook->SendMessage(WM_AXCHILD, (WPARAM)wParam, (LPARAM)lParam);

	return 0;
}

void CSChild::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_xcaption.IsVisible())
	{
		DefWindowProc(WM_NCLBUTTONDOWN, HTCAPTION, 0);
		return;
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CSChild::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
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
	CFrameWnd::OnLButtonDblClk(nFlags, point);
}

int CSChild::osLevel()
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

void CSChild::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: Add your message handler code here and/or call default
	/*
	int interFrame = 0;
	if (!IsZoomed() && !IsIconic())
	{
		lpncsp->rgrc[0].left -= interFrame;
		lpncsp->rgrc[0].right += interFrame;
		lpncsp->rgrc[0].top -= GetSystemMetrics(SM_CYFRAME) - 2;//Axis::interFrame;
		lpncsp->rgrc[0].bottom += interFrame;

		if (GetStyle() & WS_CAPTION) {
			lpncsp->rgrc[0].top -= GetSystemMetrics(SM_CYCAPTION) + 1;
			lpncsp->rgrc[0].left -= 1;
			lpncsp->rgrc[0].right += 1;
			lpncsp->rgrc[0].bottom += 1;
		}
	} 
	*/
	CFrameWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}


void CSChild::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CFrameWnd::OnWindowPosChanged(lpwndpos);
	if(m_mapN == INTERMAP)
	{
		CPoint cp(lpwndpos->x, lpwndpos->y);
		AfxGetMainWnd()->PostMessage(WM_AXIS, axINTERSDIMOVE, (LPARAM)&cp);
	}
}
