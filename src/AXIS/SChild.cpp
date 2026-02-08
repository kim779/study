// SChild.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "SChild.h"
#include "axMsg.hxx"
#include "axMisc.h"
#include "axTabView.h"	
//#define	XGAP	GetSystemMetrics(SM_CXFRAME)

#include "MainFrm.h"
#include "MapHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSChild

//IMPLEMENT_DYNCREATE(CSChild, CFrameWnd)

CSChild::CSChild(CWnd* mainframe, bool stealth)
{
	m_axiscall  = NULL;
	m_mainframe	= mainframe;
	m_bstealth	= stealth;
	m_bIconic	= false;
	m_tView		= false;
	m_sdiMode	= false;
	m_bCaption	= true;
	m_fixW		= false;
	m_fixH		= false;
	m_bBackGround = false;
	m_cursor	= 0;

	m_usrData	= "";

	m_mdiPt		= CPoint(0, 0);
	m_rectMDI.SetRectEmpty();
	m_restoreRc.SetRectEmpty();

	memset(&m_minMaxInfo, 0x00, sizeof(MINMAXINFO));
	m_minMaxInfo.ptMinTrackSize.x = MMI_MINWIDTH;
	m_minMaxInfo.ptMinTrackSize.y = MMI_MINHEIGHT;

	m_revised = FALSE;
}

CSChild::~CSChild()
{
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
	ON_WM_NCCALCSIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_AXIS, OnAXIS)
	ON_MESSAGE(WM_CHILDMSG, OnCHILDMSG)
	ON_WM_NCACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSChild message handlers

LONG CSChild::OnAXIS(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case axCLOSEX:	break;
	case axWAIT:
		m_cursor = (int) lParam;
		if (m_cursor)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		else	
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	default:
		m_mainframe->SendMessage(WM_AXIS, wParam, lParam);
		break;
	}
	return 0;
}

LONG CSChild::OnCHILDMSG(WPARAM wParam, LPARAM lParam)
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

	m_defaultpos = CPoint(lpCreateStruct->x, lpCreateStruct->y);
	CRect	wRc;
	GetDesktopWindow()->GetWindowRect(wRc);
	if (m_defaultpos.x < wRc.left)	m_defaultpos.x = wRc.left;
	if (m_defaultpos.y < wRc.top)	m_defaultpos.y = wRc.top;
	if (m_defaultpos.x >= wRc.right)	m_defaultpos.x = wRc.left;
	if (m_defaultpos.y >= wRc.bottom)	m_defaultpos.y = wRc.top;
	
	SetWindowPos(NULL, 10000, 10000, 0, 0, SWP_HIDEWINDOW);

	//ModifyStyle(WS_CAPTION|WS_MAXIMIZEBOX, 0, SWP_DRAWFRAME);  //vc2019? test
	ModifyStyle( WS_MAXIMIZEBOX, 0, SWP_DRAWFRAME);
	return 0;
}

void CSChild::OnClose() 
{
	if (m_axiscall)
		(*m_axiscall)(AXI_LASTMAPS, (WPARAM)(LPCTSTR)m_mapN, (LPARAM)(LPCTSTR)"");
	if (m_mainframe)
		m_mainframe->SendMessage(WM_AXIS, MAKEWPARAM(axCLOSES, m_key), 0);
	CFrameWnd::OnClose();
}

void CSChild::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	switch (nState)
	{
	case WA_INACTIVE:	m_bActive = false;	break;
	default:		m_bActive = true;	break;
	}
	m_xcaption.ActivateApp(m_bActive);
	OnNcPaint();
	if (Axis::isVista) return;
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}

void CSChild::OnSizing(UINT fwSide, LPRECT pRect) 
{
		CRect*	prect = (CRect *) pRect;
		const int	vtype = m_vwTYPE & vtypeMSK;
	/*
	CString tmp; tmp.Format("%d",vtype);
	AfxMessageBox(tmp);
	*/
	switch (vtype)
	{
	case vtypeFIX:
	case vtypeWND:
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
	default:break;
	}
	CFrameWnd::OnSizing(fwSide, pRect);
/*	
	CRect*	prect = (CRect *) pRect;
	int	vtype = m_vwTYPE & vtypeMSK;

	switch (vtype)
	{
	case vtypeFIX:
	case vtypeWND:
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
	default:break;
	}
*/
}

void CSChild::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// cej 20070109, SDI의 Tabview화면에서 resize하면 Tab이 사라지는 현상 없앰
	if (m_tView)
	{
		CView*	view = GetActiveView();
		((CAxTabView *)view)->JustWindow(m_mapN, true);
	}
	
	CRect form;
	GetWindowRect(form);
	form.NormalizeRect();
	form.OffsetRect(-form.left, -form.top);

	const int gap = 4;
	const int xx = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXSIZEFRAME);
	const int yy = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYSIZEFRAME);

	//form.DeflateRect(xx - gap, 0);
	form.top += GetSystemMetrics(SM_CYFRAME) + gap;
	form.left += gap;
	form.right -= gap;
	form.bottom -= yy ;

	CRgn rgn;
	rgn.CreateRectRgn(form.left, form.top, form.right, form.bottom);
	SetWindowRgn(rgn, TRUE);
	DeleteObject(rgn);
	
}

BOOL CSChild::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!m_bstealth)
		return TRUE;//CFrameWnd::PreCreateWindow(cs);
	if (!::IsWindow(m_wndInvisible.m_hWnd))
	{
		UINT	nRes{};
		CString	siteID  = AfxGetApp()->GetProfileString("Workstation", "SiteID");
		if (siteID == "D")
			nRes = IDI_STAFF;
		else	
			nRes = IDI_HCLIENT;
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
	
   // cs.style = WS_POPUP;
	//return CFrameWnd::PreCreateWindow(cs);
}

void CSChild::SetMisc(UINT msg, CWnd* parent, bool (*axiscall)(int, WPARAM, LPARAM),
			  int fontsize, bool sidMode, bool bCaption)
{
	m_axiscall = axiscall;
	m_bCaption = bCaption;
	m_sdiMode  = sidMode;

	if (m_bCaption)
	{
		m_xcaption.SetMisc(parent, msg, m_key, fontsize, m_sdiMode);
		(*m_axiscall)(AXI_HIDECAPTIONBUTTON, 0, m_key);
	}

	SendMessage(WM_NCPAINT, (WPARAM) 1);
}

void CSChild::OnNcPaint() 
{
	//**CFrameWnd::OnNcPaint();
	CWindowDC dc(this);
	DrawFrame(&dc);
}

void CSChild::DrawFrame(CDC* pDC) 
{
//	if (GetStyle() & WS_CAPTION)
//		SetWindowRgn(0, FALSE);

	CRect rect;
	GetWindowRect(rect);
	rect.NormalizeRect();
	rect.OffsetRect(-rect.left, -rect.top);

	CRect top = rect;
	CRect bottom = rect;
	CRect right = rect;
	CRect left = rect;

	top.bottom = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYSIZEFRAME) - 1;
	bottom.top = bottom.bottom - GetSystemMetrics(SM_CYFRAME) - GetSystemMetrics(SM_CYSIZEFRAME) - 2;
	right.left = right.right - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXSIZEFRAME) - 1;
	left.right = left.left + GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXSIZEFRAME);

	COLORREF border{};
	if (m_bActive == TRUE)
		border = Axis::GetColor(64);
	else
		border = RGB(153, 153, 153);
	
	const CRect lside(left.left, top.bottom - 2, left.right, GetSystemMetrics(SM_CYCAPTION) + top.bottom + 1),
		rside(right.left, top.bottom - 2, right.right, GetSystemMetrics(SM_CYCAPTION) + top.bottom + 1);


	pDC->FillSolidRect(bottom, border);

	pDC->FillSolidRect(left, border);
	pDC->FillSolidRect(right, border);

	CRect winRc;
	GetWindowRect(winRc);
	m_xcaption.DrawFrame(pDC, winRc);
}

void CSChild::ChangeSkin(CString skinName)
{
	m_xcaption.ChangeSkin();
	SendMessage(WM_NCPAINT, (WPARAM) 1);
	GetActiveView()->SendMessage(WM_NCPAINT, (WPARAM) 1);
	m_xcaption.Invalidate(FALSE);
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
	
	const int vtype = m_vwTYPE & vtypeMSK;
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
	const int	vtype = m_vwTYPE & vtypeMSK;
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


BOOL CSChild::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
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
		break;
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


void CSChild::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if (!IsZoomed() && !IsIconic())
	{
		//lpncsp->rgrc[0].left -= interFrame;
		//lpncsp->rgrc[0].right += interFrame;
		//lpncsp->rgrc[0].top -= GetSystemMetrics(SM_CYFRAME) - 2;
		//lpncsp->rgrc[0].bottom += interFrame;

		if (GetStyle() & WS_CAPTION) {
			lpncsp->rgrc[0].top -= GetSystemMetrics(SM_CYCAPTION) + 1;
			lpncsp->rgrc[0].left -= 1;
			lpncsp->rgrc[0].right += 1;
			lpncsp->rgrc[0].bottom += 1;
		}
	}
	/*
	int interFrame = Axis::interFrame;
	if (Axis::isVista)
	{
		interFrame = GetSystemMetrics(SM_CXFRAME) - 4;
	}

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

void CSChild::SetMapName(bool (*axiscall)(int, WPARAM, LPARAM), CString sMapName)
{
	(*axiscall)(AXI_LASTMAPS, (WPARAM)(LPCTSTR)m_mapN, (LPARAM)(LPCTSTR)sMapName);
	m_mapN = sMapName;

#ifdef DF_MK_CAPTION
	m_xcaption.SetMapNum(m_mapN);

	m_slog.Format("[AXIS][market][SDI][%s]<%d> sMapName=[%s] ", __FUNCTION__, __LINE__, sMapName);
	OutputDebugString(m_slog);
#endif
}


void CSChild::SetMinMaxInfo(MINMAXINFO* lpMinMaxInfo)
{
	memcpy(&m_minMaxInfo.ptReserved, &lpMinMaxInfo->ptReserved, sizeof(POINT));

	ConditionCopyMinMaxInfo(lpMinMaxInfo, &m_minMaxInfo);
}

void CSChild::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	ConditionCopyMinMaxInfo(&m_minMaxInfo, lpMMI);
	
	if (m_vwTYPE & vtypeRSZ && 
		!AfxGetMainWnd()->SendMessage(WM_AXIS, (MAKEWPARAM(axIsDLL, 0), (LPARAM)(LPCTSTR)m_mapN)))
	{
		lpMMI->ptMaxTrackSize.x = m_width;
	}

	// 2010.09.15 예외처리 : 2016 Matrix현재가에 대해 스크린 사이즈와 상관없이 최대 2048pixel까지 허용 by warship
	if (m_mapN=="IB201600")
	{
		lpMMI->ptMaxTrackSize.y = 2048;
	}
		
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CSChild::SetSize(CSize sz)
{
	SetSize(sz.cx, sz.cy);
}

void CSChild::SetSize(int width, int height, BOOL revise)
{
	if (!m_revised && revise)
	{

		m_revised = TRUE;
		CSize reviseFrameSize(Axis::reviseFrameSize);
		if (Axis::isVista)
			reviseFrameSize = CSize(GetSystemMetrics(SM_CXFRAME) + 3, GetSystemMetrics(SM_CYFRAME) + 5);

		m_width = width - reviseFrameSize.cx;
		m_height = height - reviseFrameSize.cy;
	}
	else
	{
		m_width = width;
		m_height = height;
	}
}

CSize CSChild::GetSize()
{
	return CSize(m_width, m_height);
}

void CSChild::SetTabView(bool isTabView)
{
	TRACE("SET TAB VIEW(SCHILD)\n");

	m_tView = isTabView;
}

bool CSChild::IsTabView()
{
	return m_tView;
}

void CSChild::ShowBGCaption(bool f_show)
{
	//AfxMessageBox("ShowBGCaption");
	//m_xcaption.ShowWindow(SW_SHOW);
	//SetSize(m_width+GetSystemMetrics(SM_CXFRAME)*2, m_height+(CAPTION_HEIGHT+GetSystemMetrics(SM_CYFRAME)), true);
	/*
	CRect rect;
	GetClientRect(&rect);
	::SetWindowPos(m_hWnd, HWND_BOTTOM, rect.left, rect.top, rect.right, rect.bottom, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	*/
	
}


BOOL CSChild::OnNcActivate(BOOL bActive)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/*
	if (m_bCaption && m_xcaption.GetSafeHwnd())
		m_xcaption.ActivateApp(bActive);
	m_bActive = bActive;

	OnNcPaint();
	*/
	return CFrameWnd::OnNcActivate(bActive);
}
