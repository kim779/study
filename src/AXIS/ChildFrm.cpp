// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "axis.h"
#include "axisView.h"
#include "axisdoc.h"
#include "ChildFrm.h"
#include "axmsg.hxx"
#include "axtabview.h"
#include "axMisc.h"

#include "Helper.h"

#define _WINIX
//#define	XGAP	GetSystemMetrics(SM_CXFRAME)

#include "MainFrm.h"
#include "MapHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame
#pragma warning (disable : 26409)
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)
#pragma warning (default : 26409)
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
	ON_WM_NCCALCSIZE()
	ON_WM_NCHITTEST()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_AXIS, OnAXIS)
	ON_MESSAGE(WM_CHILDMSG, OnCHILDMSG)
	ON_COMMAND_RANGE(ID_GROUP_BASE, ID_GROUP_END, OnGroup)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

#define TM_NCPAINT 9898

CChildFrame::CChildFrame()
{
	m_axiscall	= NULL;
	m_bActive	= FALSE;
	m_fontsize  = -1;

	m_key		= 0;
	m_cursor	= 0;
	m_tView		= false;
	m_bIconic	= false;
	m_bCaption	= true;
	m_suspend	= false;
	m_bSingle	= false;
	m_fixW		= false;
	m_fixH		= false;
	m_bBackGround = false;

	m_usrData = "";

	m_iconpos	= CPoint(-1, -1);
	m_rectSDI.SetRectEmpty();
	m_restoreRc.SetRectEmpty();	// 20070719

	memset(&m_minMaxInfo, 0x00, sizeof(MINMAXINFO));

	m_minMaxInfo.ptMinTrackSize.x = MMI_MINWIDTH;
	m_minMaxInfo.ptMinTrackSize.y = MMI_MINHEIGHT;

	m_width = m_height = 0;
	m_revised = FALSE;
}

CChildFrame::~CChildFrame()
{
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
	SetWindowPos(NULL, lpCreateStruct->x, lpCreateStruct->y, lpCreateStruct->cx, lpCreateStruct->cy, 
		lpCreateStruct->style);

	WINDOWPLACEMENT pl;	GetWindowPlacement(&pl);

	m_SP = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
	CRect wRc;
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
	
	return 0;
}

void CChildFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CRect*	prect = (CRect *) pRect;
	const int	vtype = m_vwTYPE & vtypeMSK;

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
	CMDIChildWnd::OnSizing(fwSide, pRect);
}

void CChildFrame::SetMisc(UINT msg, CWnd* parent, bool (*axiscall)(int, WPARAM, LPARAM),
				int fontsize, bool bCaption)
{
	m_axiscall = axiscall;
	m_bCaption = bCaption;
	m_fontsize = fontsize;
	//::SetWindowLong(m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) | SWP_NOSIZE);
	//::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREDRAW|SWP_FRAMECHANGED);

	if (m_bCaption)
	{
		m_xcaption.SetMisc(parent, msg, m_key, fontsize, false);
		(*m_axiscall)(AXI_HIDECAPTIONBUTTON, 0, m_key);
	}
	
	SendMessage(WM_NCPAINT, (WPARAM) 1);

}

void CChildFrame::OnGroup(UINT uid)  //vc2019
{
}

BOOL CChildFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	const int	index = LOWORD(wParam);
	if (index >= ID_GROUP_BASE && index < ID_GROUP_END)
	{
		if (runCommand(LOWORD(wParam), lParam))	return TRUE;
		return CMDIChildWnd::OnCommand(wParam, lParam);
	}
	
	return CMDIChildWnd::OnCommand(wParam, lParam);
}

bool CChildFrame::runCommand(int comm, LPARAM lParam)
{
	const int	triggerN = comm - ID_GROUP_BASE;
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
//#if 0
//	if (m_bBackGround)
//	{
//		/*
//		CRect rect;
//		GetClientRect(&rect);
//		::SetWindowPos(m_hWnd, HWND_BOTTOM, rect.left, rect.top, rect.right, rect.bottom, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
//		*/
//		::SetWindowPos(m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
//	}
//#else
//	CWnd *pBkWnd = NULL, *pLastWnd = NULL;
//	if ((CWnd*)this==pActivateWnd)
//	{
//		CWnd *pWnd = GetWindow(GW_HWNDLAST);
//		while(pWnd)
//		{
//			const CChildFrame *pChild = (CChildFrame*)pWnd;
//			if (pChild->m_bBackGround) pBkWnd = pWnd;
//			if (!pLastWnd && pWnd->IsWindowVisible()) pLastWnd = pWnd;
//			pWnd = pWnd->GetWindow(GW_HWNDPREV);
//		}
//
//		if (pBkWnd && pBkWnd!=this)
//		{
//			::SetWindowPos(pBkWnd->m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
//		}
//		else if (pBkWnd==this && pLastWnd)
//		{
//			const BOOL bCtrl = GetKeyState(VK_LCONTROL) & 0x80000;
//			const BOOL bShft = GetKeyState(VK_LSHIFT) & 0x80000;
//			const BOOL bTab  = GetKeyState(VK_TAB) & 0x80000;
//			if (bCtrl && bShft && bTab)		// Ctrl+Shift+Tab키를 이용해 돌다가 활성화될경우 건너뛰게한다.
//				::SetWindowPos(pLastWnd->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);	
//			else							// 마우스로 직접 바탕화면 클릭했을경우 Activation만 해제한다.
//				::SetWindowPos(pBkWnd->m_hWnd, pLastWnd->m_hWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
//		}
//		return;
//	}
//#endif
//
//	if(bActivate)
//		CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

		CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

LONG CChildFrame::OnAXIS(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case axCLOSEX:	break;
	case axWAIT:
		m_cursor = (int) lParam;
		if (m_cursor)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		else	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	case axLINKEDIT:
		{
		//	OutputDebugString("CAPTION LINK==========\n");
			if (HIWORD(wParam) == 1)				
				AfxGetMainWnd()->SendMessage(WM_AXIS, wParam, (LPARAM)(const char*)m_mapN);
			else
				AfxGetMainWnd()->SendMessage(WM_AXIS, wParam, lParam);
		}
		break;
	default:
		AfxGetMainWnd()->SendMessage(WM_AXIS, wParam, lParam);
		break;
	}
	return 0;
}

LONG CChildFrame::OnCHILDMSG(WPARAM wParam, LPARAM lParam)
{
	AfxGetMainWnd()->SendMessage(WM_CHILDMSG, wParam, lParam);
	return 0;
}

void CChildFrame::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	const int	vtype = m_vwTYPE & vtypeMSK;
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
		TRACE("m_iconpos.........[%d]\n", m_key);
	}
	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
}

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);

	CRect form;
	GetWindowRect(form);
	form.NormalizeRect();
	form.OffsetRect(-form.TopLeft());

	const int gap = 4;
	const int xx = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXSIZEFRAME);
	const int yy = GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYSIZEFRAME);

	form.DeflateRect(xx , yy );
	form.right += 4;
	form.left -= 4;
	form.bottom += 1;
	
	CRgn rgn;
	rgn.CreateRectRgn(form.left, form.top, form.right, form.bottom);
	SetWindowRgn(rgn, TRUE);
	DeleteObject(rgn);

	if (m_tView)
	{
		CView*	view = GetActiveView();
		((CAxTabView *)view)->JustWindow(m_mapN, true);
	}	
}

void CChildFrame::OnNcPaint()
{
	/**
	CMDIChildWnd::OnNcPaint();
		return;
	**/
	//CWindowDC dc(this);
	//DrawFrame(&dc);  
	DrawFrame();
}

BOOL CChildFrame::OnNcActivate(BOOL bActive) 
{
	m_xcaption.ActivateApp(bActive);
	m_bActive = bActive;
	
	//OnNcPaint();  
	//  TM_NCPAINT
	SetTimer(TM_NCPAINT, 5, nullptr);
	return CMDIChildWnd::OnNcActivate(bActive);
}

void CChildFrame::DrawFrame() 
{
	if (m_bBackGround) return;

	CWindowDC dc(this);

	CRect rect;
	GetWindowRect(rect);
	rect.NormalizeRect();
	rect.OffsetRect(-rect.left, -rect.top);

	CRect right = rect;
	CRect left = rect;
	CRect bottom = rect;
	COLORREF border{};

	//if (m_bActive == TRUE)
	if(m_xcaption.IsActive())
		border = Axis::GetColor(64);
	else
		border = RGB(153, 153, 153);

	right.left = right.right - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXSIZEFRAME) -2;
	left.right = left.left + GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXSIZEFRAME);
	bottom.top = bottom.bottom - GetSystemMetrics(SM_CYFRAME) - GetSystemMetrics(SM_CYSIZEFRAME) - 1;

	dc.FillSolidRect(left, border);
	dc.FillSolidRect(right, border);
	dc.FillSolidRect(bottom, border);
	/*InvalidateRect(left, 0);
	InvalidateRect(right, 0);
	InvalidateRect(bottom, 0);*/
}

void CChildFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CMDIChildWnd::OnPaint()을(를) 호출하지 마십시오.
	SetTimer(TM_NCPAINT, 5, nullptr);
}


void CChildFrame::ChangeSkin(CString skinName)
{
	m_xcaption.ChangeSkin();

	SendMessage(WM_NCPAINT, (WPARAM) 1);

	GetActiveView()->SendMessage(WM_NCPAINT, (WPARAM) 1);
	m_xcaption.Invalidate(FALSE);
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
		//	OutputDebugString("Call MINIMIZE\n");
			CProfile p1(pkVTCode);
			
			CString map(m_mapN);
			CString s;
			s.Format("ACTIONCAPTION [%s]\n",map);
		//	OutputDebugString(s);
			//최소화 처리 dkkim 2015.10.28
			//DLL은 실제로 최소화하지 않고 화면만 숨긴다.OnSize를 보내지 않음.
			if(!p1.GetString("DLL Controls", map).IsEmpty())
			{
			//	OutputDebugString("WINDOW HIDE\n");
				this->ShowWindow(SW_HIDE);
				m_bIconic = true;
				m_xcaption.SetIconic(m_bIconic);
				m_xcaption.SetMax(false);
				if (!m_xcaption.IsMax())
				{
					m_restoreRc.SetRect(pl.rcNormalPosition.left, pl.rcNormalPosition.top,
						pl.rcNormalPosition.right, pl.rcNormalPosition.bottom);
				}
				return;
			}

			m_suspend = true;
			const int cx = GetSystemMetrics(SM_CXMINIMIZED);
			 int cy = GetSystemMetrics(SM_CYMINIMIZED);
			cy = GetSystemMetrics(SM_CYFRAME) + XCAP_HEIGHT;
			cy += GetSystemMetrics(SM_CYBORDER);
			
			if (!m_xcaption.IsMax())
			{
				m_restoreRc.SetRect(pl.rcNormalPosition.left, pl.rcNormalPosition.top,
						pl.rcNormalPosition.right, pl.rcNormalPosition.bottom);
			}
			SetMinimize();
			//**SetWindowPos(&wndBottom, pt.x, pt.y, cx, cy, SWP_SHOWWINDOW);
			SetWindowPos(&wndBottom, pt.x, -100, cx, cy, SWP_SHOWWINDOW);
			m_suspend = false;
			//WD_MINIMIZE 메시지를 윈도우에 던진다.최소화 단계 종료 메시지
			//0의 값을 받아서 각화면에서 OnSize를 처리하게 코딩해야함.
// 			if(!p1.GetString("DLL Controls", map).IsEmpty())
// 			{
// 				CWnd* base = GetActiveView()->GetWindow(GW_CHILD);
// 				base->SendMessage(WD_MINIMIZED,(WPARAM)0,(LPARAM)(INT)0);
// 			}
		}
		break;
	case IDX_RESTORE:
		{
			m_suspend = true;
			CProfile p1(pkVTCode);
			
			CString map(m_mapN);
			//최소화 처리 dkkim 2015.10.28
			//숨긴화면을 다시 보이게 한다.
			if(!p1.GetString("DLL Controls", map).IsEmpty())
			{
				const CWnd* base = GetActiveView()->GetWindow(GW_CHILD);
				
				this->ShowWindow(SW_SHOW);
				SetRestore();

				this->BringWindowToTop();
				SetWindowPos(&wndTop, m_restoreRc.left, m_restoreRc.top, m_restoreRc.Width(),
					m_restoreRc.Height(), SWP_SHOWWINDOW);
				break;
			}
			SetRestore();
			SetWindowPos(&wndTop, m_restoreRc.left, m_restoreRc.top, m_restoreRc.Width(),
					m_restoreRc.Height(), SWP_SHOWWINDOW);
			m_suspend = false;
		}
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
			
			if (m_fixH || m_fixW)	// 20070702 수정
			{	
				if (!m_restoreRc.IsRectEmpty())
				{
					m_suspend = true;
					SetRestore();
					SetWindowPos(&wndTop, m_restoreRc.left, m_restoreRc.top, m_restoreRc.Width(),
						m_restoreRc.Height(), SWP_SHOWWINDOW);
					m_suspend = false;
				}
				break;
			}
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
	if (m_axiscall)
	{
		RECT rectFrom;
		RECT rectTo;

		::GetWindowRect(m_hWnd, &rectFrom);
		(*m_axiscall)(AXI_GETSCRBUTTONPOS, m_key, (LPARAM)&rectTo);
		//윈도우 작업 표시줄 영역을 제외한 윈도우 전체 영역을 얻은 후, 트레이 창의 영역 구함
		//::SystemParametersInfo (SPI_GETWORKAREA, 0, &rectTo, 0);    
		rectTo.left = rectTo.right  - 70;
		rectTo.top  = rectTo.bottom - 10;
		// 트레이아이콘 방향으로 최소화 시키는 효과
		::DrawAnimatedRects(m_hWnd, IDANI_CAPTION, &rectFrom, &rectTo);
		::ShowWindow(m_hWnd, SW_HIDE);	
	}

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

void CChildFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	//int interFrame = Axis::interFrame;   //vc2019 test
	//if (Axis::isVista)
	//{
	//	interFrame = GetSystemMetrics(SM_CXFRAME) - 3;
	//}
	//if (m_bBackGround)	interFrame = GetSystemMetrics(SM_CXFRAME);
	//if (!IsZoomed() && !IsIconic())
	//{
	//	lpncsp->rgrc[0].left -= interFrame;
	//	lpncsp->rgrc[0].right += interFrame;
	//	if (!m_bBackGround)
	//		lpncsp->rgrc[0].top -= GetSystemMetrics(SM_CYFRAME) - 2;//Axis::interFrame;
	//	else
	//		lpncsp->rgrc[0].top -= GetSystemMetrics(SM_CYFRAME) +4 ;//Axis::interFrame;
	//	lpncsp->rgrc[0].bottom += interFrame;

	//	if (GetStyle() & WS_CAPTION) {
	//		lpncsp->rgrc[0].top -= GetSystemMetrics(SM_CYCAPTION) + 1;
	//		lpncsp->rgrc[0].left -= 1;
	//		lpncsp->rgrc[0].right += 1;
	//		lpncsp->rgrc[0].bottom += 1;
	//	}
	//}
	//
	//CMDIChildWnd::OnNcCalcSize(bCalcValidRects, lpncsp);

	if (!IsZoomed() && !IsIconic())
	{
		if (GetStyle() & WS_CAPTION) {
			lpncsp->rgrc[0].top -= GetSystemMetrics(SM_CYCAPTION) + 1;
			lpncsp->rgrc[0].left -= 1;
			lpncsp->rgrc[0].right += 1;
			lpncsp->rgrc[0].bottom += 1;
		}
	}
	CMDIChildWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

LRESULT CChildFrame::OnNcHitTest(CPoint point) //vc2019 UINT->LRESULT
{
	const UINT result = CMDIChildWnd::OnNcHitTest(point);

	return result;
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);	
}

void CChildFrame::SetMapName(bool (*axiscall)(int, WPARAM, LPARAM), CString sMapName)
{
	(*axiscall)(AXI_LASTMAPS, (WPARAM)(LPCTSTR)m_mapN, (LPARAM)(LPCTSTR)sMapName);
	m_mapN = sMapName;

#ifdef DF_MK_CAPTION
	m_xcaption.SetMapNum(m_mapN);

	m_slog.Format("[AXIS][market][%s]<%d> sMapName=[%s] ", __FUNCTION__, __LINE__, sMapName);
	OutputDebugString(m_slog);
#endif
}

void CChildFrame::OnClose() 
{
	if (m_axiscall)
		(*m_axiscall)(AXI_LASTMAPS, (WPARAM)(LPCTSTR)m_mapN, (LPARAM)(LPCTSTR)"");
	
	CMDIChildWnd::OnClose();
}

BOOL CChildFrame::OnEraseBkgnd(CDC* pDC) 
{
	return 0;
	/*return TRUE;	
	return CMDIChildWnd::OnEraseBkgnd(pDC);*/
}

void CChildFrame::SetMinMaxInfo(MINMAXINFO* lpMinMaxInfo)
{
	memcpy(&m_minMaxInfo.ptReserved, &lpMinMaxInfo->ptReserved, sizeof(POINT));

	ConditionCopyMinMaxInfo(lpMinMaxInfo, &m_minMaxInfo);
}

void CChildFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
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

	CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
}


CSize CChildFrame::GetSize()
{
	return CSize(m_width, m_height);
}

void CChildFrame::SetSize(CSize sz)
{
	SetSize(sz.cx, sz.cy);
}

void CChildFrame::SetSize(int width, int height, BOOL revise)
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

void CChildFrame::SetTabView(bool isTabView)
{
	if (isTabView)
		TRACE("SET TASB VIEW \n");
	m_tView = isTabView;
}

bool CChildFrame::IsTabView()
{
	return m_tView;
}

void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	switch (nID)
	{
	case SC_MAXIMIZE:
		actionCaption(IDX_MAX, CPoint(0, 0));
		return;
	case SC_RESTORE:
		actionCaption(IDX_RESTORE, CPoint(0, 0));
		return;
	}
	CMDIChildWnd::OnSysCommand(nID, lParam);
}


void CChildFrame::ShowBackGround(bool f_show)
{
	if (f_show)
	{
		m_bBackGround = true;
		m_xcaption.ShowWindow(SW_HIDE);
		
		SetSize(m_width-GetSystemMetrics(SM_CXFRAME)*2, m_height-(CAPTION_HEIGHT+GetSystemMetrics(SM_CYFRAME)), false);

		::SetWindowPos(m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
	else
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW);
		
		SetSize(m_width+GetSystemMetrics(SM_CXFRAME)*2, m_height+(CAPTION_HEIGHT+GetSystemMetrics(SM_CYFRAME)), false);
		
		

	}
	
	
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE | ~WS_EX_STATICEDGE;
	return CMDIChildWnd::PreCreateWindow(cs);
}


void CChildFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TM_NCPAINT:
		{
			KillTimer(TM_NCPAINT);
			DrawFrame();
		}
		break;
	}
	CMDIChildWnd::OnTimer(nIDEvent);
}
