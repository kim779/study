// PopFrame.cpp : implementation file
//

#include "stdafx.h"
#include "axisWork.h"
#include "PopFrame.h"
#include "ChildFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE 
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopFrame

IMPLEMENT_DYNCREATE(CPopFrame, CFrameWnd)

CPopFrame::CPopFrame()
{
	m_initSize = CSize(0, 0);
	m_wndType  = NULL;
	m_hAccel   = NULL;
}

CPopFrame::~CPopFrame()
{
}


BEGIN_MESSAGE_MAP(CPopFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CPopFrame)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SYSCOMMAND()
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopFrame message handlers

BOOL CPopFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	LONG style = GetWindowLong(m_pParent->GetSafeHwnd(), GWL_EXSTYLE);
	style = WS_EX_TOOLWINDOW;
	SetWindowLong(m_pParent->GetSafeHwnd(), GWL_EXSTYLE, style);
	cs.hwndParent = m_pVirtual->m_hWnd;	
	return CFrameWnd::PreCreateWindow(cs);
}


int CPopFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ASSERT(m_hAccel == NULL);
	m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	ASSERT(m_hAccel != NULL);

	GetSystemMenu(FALSE)->InsertMenu(0, MF_BYPOSITION|MF_STRING, 0, "MAPNAME");
	
	return 0;
}

void CPopFrame::SetParent(CChildFrame *pChildWnd, CWnd *pVirtualWnd)
{
	m_pParent = pChildWnd;
	m_pVirtual = pVirtualWnd;
}

void CPopFrame::OnClose() 
{
	if (!m_pParent->DeleteFiles())
		return;
	m_pParent->PostMessage(WM_CLOSE);	
}

void CPopFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	if (nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
	{
		if (!pWndOther)
			((CMainFrame*)AfxGetMainWnd())->SDIShowChild();
		((CFrameWnd*)m_pParent)->ActivateFrame();
		m_pParent->OnActivateMe();
	}

	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}


void CPopFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
}

void CPopFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	if (pRect->right - pRect->left <= 120)
	{
		if (GetStyle() & WS_CAPTION)
			ModifyStyle(WS_CAPTION, NULL, SWP_DRAWFRAME);
	}
	else
	{
		if (!(GetStyle() & WS_CAPTION))
			ModifyStyle(NULL, WS_CAPTION, SWP_DRAWFRAME);
	}
	CFrameWnd::OnSizing(fwSide, pRect);
}

BOOL CPopFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_COMMAND && GetFocus() && GetFocus()->m_hWnd == m_hWnd)
	{
		m_pParent->SendMessage(WM_COMMAND, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}

	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (m_hAccel != NULL && ::TranslateAccelerator(AfxGetMainWnd()->m_hWnd, m_hAccel, pMsg))
			return TRUE;
		if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
		{
			if (::TranslateMDISysAccel(m_pParent->m_hWnd, pMsg))
				return TRUE;
			if (pMsg->wParam == VK_F4 && 
				((::GetKeyState(VK_CONTROL) & 0x8000) || 
				(::GetKeyState(VK_MENU) & 0x8000)))
			{
				if ((::GetKeyState(VK_CONTROL) & 0x8000))
					PostMessage(WM_CLOSE);
				else if ((::GetKeyState(VK_MENU) & 0x8000))
					AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return TRUE;
			}
			if (pMsg->wParam == VK_TAB && (::GetKeyState(VK_CONTROL) & 0x8000))
			{
				((CMDIFrameWnd*)AfxGetMainWnd())->MDINext();
				return TRUE;
			}
		}
	}
	return FALSE;
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CPopFrame::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	if (lpwndpos->cx == 0 && lpwndpos->cy == 0)
		return;
	CFrameWnd::OnWindowPosChanging(lpwndpos);
	if (!(lpwndpos->flags & SWP_NOSIZE))
		return;

	CRect lRect = CRect(lpwndpos->x, lpwndpos->y, lpwndpos->x + 1, lpwndpos->y + lpwndpos->cy),
		rRect = CRect(lpwndpos->x + lpwndpos->cx, lpwndpos->y, lpwndpos->x + lpwndpos->cx + 1, lpwndpos->y + lpwndpos->cy),
		tRect = CRect(lpwndpos->x, lpwndpos->y, lpwndpos->x + lpwndpos->cx, lpwndpos->y + 1), 
		bRect = CRect(lpwndpos->x, lpwndpos->y + lpwndpos->cy, lpwndpos->x + lpwndpos->cx, lpwndpos->y + lpwndpos->cy + 1);
	CChildFrame* pChild;
	int cnt = 0;
	do
	{
		cnt++; 
		pChild = (CChildFrame*)((CWorkApp*)AfxGetApp())->GetNextMDIChildWnd();

		if (pChild && pChild->m_pPopFrame != this)
		{
			CRect rc, mRc, lRc, rRc, tRc, bRc;
			pChild->m_pPopFrame->GetWindowRect(&rc);
			mRc = CRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			rc.OffsetRect(-mRc.left, -mRc.top);
			lRc.SetRect(rc.left - 10, rc.top, rc.left + 10, rc.bottom);
			rRc.SetRect(rc.right - 10, rc.top, rc.right + 10, rc.bottom);
			tRc.SetRect(rc.left, rc.top - 10, rc.right, rc.top + 10);
			bRc.SetRect(rc.left, rc.bottom - 10, rc.right, rc.bottom + 10);
			if (mRc.IntersectRect(lRect, rRc))
				lpwndpos->x = rRc.left + 10;
			if (mRc.IntersectRect(rRect, lRc))
				lpwndpos->x = lRc.left + 10- lpwndpos->cx;
			if (mRc.IntersectRect(tRect, bRc))
				lpwndpos->y = bRc.top + 10;
			if (mRc.IntersectRect(bRect, tRc))
				lpwndpos->y = tRc.top + 10 - lpwndpos->cy;
		}
	}while (pChild);
	CFrameWnd* pMain = (CFrameWnd*)AfxGetMainWnd();
	{
		CRect rc, mRc, lRc, rRc, tRc, bRc;
		pMain->GetWindowRect(&rc);
		mRc = CRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		rc.OffsetRect(-mRc.left, -mRc.top);
		lRc.SetRect(rc.left - 10, rc.top, rc.left + 10, rc.bottom);
		rRc.SetRect(rc.right - 10, rc.top, rc.right + 10, rc.bottom);
		tRc.SetRect(rc.left, rc.top - 10, rc.right, rc.top + 10);
		bRc.SetRect(rc.left, rc.bottom - 10, rc.right, rc.bottom + 10);
		if (mRc.IntersectRect(lRect, rRc))
			lpwndpos->x = rRc.left + 10;
		if (mRc.IntersectRect(rRect, lRc))
			lpwndpos->x = lRc.left + 10- lpwndpos->cx;
		if (mRc.IntersectRect(tRect, bRc))
			lpwndpos->y = bRc.top + 10;
		if (mRc.IntersectRect(bRect, tRc))
			lpwndpos->y = tRc.top + 10 - lpwndpos->cy;
	}

}

BOOL CPopFrame::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CFrameWnd::OnEraseBkgnd(pDC);
}

BOOL CPopFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (m_pParent->m_view.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CPopFrame::OnSetFocus(CWnd* pOldWnd) 
{
	if (m_pParent->m_view)
		m_pParent->m_view.SetFocus();
}

void CPopFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CFrameWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	
}
