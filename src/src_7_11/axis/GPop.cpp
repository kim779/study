// GPop.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "GPop.h"
#include "axMsg.hxx"
#include "axMisc.h"

#include "../h/axisfire.h"
#include "../appl/appl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGPop

//IMPLEMENT_DYNCREATE(CGPop, CFrameWnd)

CGPop::CGPop(CWnd* pMain, bool stealth, bool depend)
{
	m_pMain = pMain;
	m_viewparent = NULL;
	m_mdiView = NULL;
	m_bstealth = stealth;
	m_cursor = 0;
	m_closex = false;
	m_bStoploss = false;
	m_depend = depend;
	m_enableHook = false;
	m_wndHook = NULL;
}

CGPop::~CGPop()
{
}


BEGIN_MESSAGE_MAP(CGPop, CFrameWnd)
	//{{AFX_MSG_MAP(CGPop)
	ON_WM_SIZING()
	ON_WM_ACTIVATE()
	ON_WM_SETCURSOR()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CHILDMSG, OnCHILDMSG)
	ON_MESSAGE(WM_AXIS, OnAXIS)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGPop message handlers

BOOL CGPop::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style &= ~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
	if (m_depend)	return CFrameWnd::PreCreateWindow(cs);
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
}

LRESULT CGPop::OnCHILDMSG(WPARAM wParam, LPARAM lParam)
{
	AfxGetMainWnd()->SendMessage(WM_CHILDMSG, wParam, lParam);
	return 0;
}

LRESULT CGPop::OnAXIS(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case axCLOSEX:
		m_closex = lParam ? true : false;
		break;
	case axWAIT:
		m_cursor = (int) lParam;
		if (m_cursor)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		else	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		break;
	case apHOOKED:
		if (lParam)
		{
			m_enableHook = HIWORD(wParam) ? true : false;
			m_wndHook = (CWnd *) lParam;
		}
		break;
	default:
		m_pMain->SendMessage(WM_AXIS, wParam, lParam);
		break;
	}
	return 0;
}

void CGPop::OnSizing(UINT fwSide, LPRECT pRect)
{
	CRect*	prect = (CRect *) pRect;
	int	vtype = m_vwTYPE & vtypeMSK;

	switch (vtype)
	{
	case vtypeFIX:
	//case vtypeWND:
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
	default:break;
	}
	
	CFrameWnd::OnSizing(fwSide, pRect);
}

CView* CGPop::AttachView(CCreateContext context)
{
	CView* pNewView = STATIC_DOWNCAST(CView, CreateView(&context));
	if (pNewView != NULL)
	{
		pNewView->ShowWindow(SW_SHOW);
		pNewView->OnInitialUpdate();
		SetActiveView(pNewView);
		RecalcLayout();
		SetOwner(NULL);
	}
	return pNewView;
}

void CGPop::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	switch (nState)
	{
	case WA_ACTIVE:
	case WA_CLICKACTIVE:
		m_active = true;
		break;
	case WA_INACTIVE:
		if (m_closex)
			PostMessage(WM_CLOSE);
	default:
		m_active = false;
		break;
	}
}

BOOL CGPop::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_cursor == 1)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		return TRUE;	
	}
	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CGPop::OnClose() 
{
	if (m_bStoploss)
	{
		ShowWindow(SW_HIDE);
		m_pMain->SetFocus();
		return;
	}

	CFrameWnd::OnClose();
}

long CGPop::runHookedKey(WPARAM wParam, LPARAM lParam)
{
	if (m_enableHook && m_wndHook)
		return(long) m_wndHook->SendMessage(WM_AXCHILD, (WPARAM)wParam, (LPARAM)lParam);

	return 0;
}