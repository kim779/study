// popListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "popListCtrl.h"
#include "editListCtrl.h"


// CPopListCtrl

IMPLEMENT_DYNAMIC(CPopListCtrl, CWnd)

CPopListCtrl::CPopListCtrl()
{

}

CPopListCtrl::~CPopListCtrl()
{
}


BEGIN_MESSAGE_MAP(CPopListCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
//	ON_MESSAGE(WM_USER+1, &CPopListCtrl::OnMessage)
END_MESSAGE_MAP()


// CPopListCtrl 메시지 처리기입니다.

int CPopListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_editListCtrl = new CEditListCtrl();
	m_editListCtrl->Create(WS_VISIBLE|WS_TABSTOP|WS_CHILD|WS_BORDER|LVS_REPORT|LVS_NOSORTHEADER|LVS_ALIGNTOP, CRect(0, 0, 260, 150), this, 0);
	m_editListCtrl->SetColumnType(_T("0012"));

	m_editListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_editListCtrl->m_strList.AddTail(_T("Numeric"));
	m_editListCtrl->m_strList.AddTail(_T("Alpha"));
	m_editListCtrl->m_strList.AddTail(_T("MultiByte"));
	m_editListCtrl->m_strList.AddTail(_T("Mixed"));
	m_editListCtrl->m_strList.AddTail(_T("Digit"));

	return 0;
}


void CPopListCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	if (nState == WA_INACTIVE)
		PostMessage(WM_USER, 0, 0);
}


BOOL CPopListCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect	rc;

	GetClientRect(&rc);
	pDC->FillSolidRect(rc, RGB(255, 255, 255));	
	return CWnd::OnEraseBkgnd(pDC);
}


BOOL CPopListCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));		
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


void CPopListCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	MSG	msg;

	for (m_done = FALSE; !m_done; WaitMessage())
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_KILLFOCUS && msg.hwnd == this->m_hWnd)
				|| ((msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN)
					&& (msg.hwnd != this->m_hWnd && msg.hwnd != m_editListCtrl->m_hWnd && ::GetParent(msg.hwnd) != m_editListCtrl->m_hWnd))
				||(msg.message == WM_USER && msg.hwnd == this->m_hWnd))
			{
				if (msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN)
					::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				m_done = TRUE;
				break;
			}

			if (!IsWindow(m_hWnd))
			{
				m_done = TRUE;
				return;
			}
			if (!IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (m_done)
			break;
	}
	
	if (IsWindow(m_hWnd))
		ShowWindow(SW_HIDE);
}


void CPopListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	m_editListCtrl->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE);	
}