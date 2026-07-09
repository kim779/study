// PopListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "PopListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopListCtrl

CPopListCtrl::CPopListCtrl()
{
}

CPopListCtrl::~CPopListCtrl()
{
}


BEGIN_MESSAGE_MAP(CPopListCtrl, CWnd)
	//{{AFX_MSG_MAP(CPopListCtrl)
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 1, OnMessage1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopListCtrl message handlers


void CPopListCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	MSG msg;

	for(m_fDone = FALSE; !m_fDone; WaitMessage())
	{
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_KILLFOCUS && msg.hwnd == this->m_hWnd) || 
				((msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN) && 
				(msg.hwnd != this->m_hWnd && msg.hwnd != m_pListCtrl->m_hWnd && ::GetParent(msg.hwnd) != m_pListCtrl->m_hWnd)) ||
				(msg.message == WM_USER && msg.hwnd == this->m_hWnd))
			{
				if (msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN)
					::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				m_fDone = TRUE;
				break;
			}

			if (!IsWindow(m_hWnd))
			{
				m_fDone = TRUE;
				return;
			}
			if (!IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (m_fDone)
			break;
	}
	
	if (IsWindow(m_hWnd))
		ShowWindow(SW_HIDE);
}

void CPopListCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE)
		PostMessage(WM_USER, 0, 0);	
}

BOOL CPopListCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));		
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CPopListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	m_pListCtrl->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE);	
}

BOOL CPopListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc, RGB(255, 255, 255));	
	return CWnd::OnEraseBkgnd(pDC);
}

int CPopListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pListCtrl = new CEditList();
	m_pListCtrl->Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER | 
		LVS_REPORT | LVS_NOSORTHEADER | LVS_ALIGNTOP ,
		CRect(0, 0, 260, 150), this, 0);
	m_pListCtrl->SetColumnType("0012");

	m_pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_pListCtrl->m_strList.AddTail("Numeric");
	m_pListCtrl->m_strList.AddTail("Alpha");
	m_pListCtrl->m_strList.AddTail("MultiByte");
	m_pListCtrl->m_strList.AddTail("Mixed");
	m_pListCtrl->m_strList.AddTail("Digit");
	return 0;
}

long CPopListCtrl::OnMessage1(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case 0:
		{
			/*int nItem = LOWORD(lParam), nSubItem = HIWORD(lParam);
			int nIdx  = m_oList.GetItemData(nItem);
			int kind  = m_pFormItem->getFormPoint(nIdx)->kind;
			if (kind == FM_BUTTON || kind == FM_BROWSER || 
				kind == FM_TABLE || kind == FM_GRID ||
				kind == FM_OBJECT)
				return FALSE;
			if (nSubItem == 5 && getDataKind(m_pFormItem->getFormPoint(nIdx)->type).IsEmpty())
				return FALSE;*/
		}
		return TRUE;
		break;
	}
	return TRUE;
}
