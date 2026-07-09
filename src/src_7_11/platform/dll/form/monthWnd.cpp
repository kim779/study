// monthWnd.cpp : implementation file
//

#include "stdafx.h"
#include "monthWnd.h"
#include "month.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CmonthWnd

CmonthWnd::CmonthWnd()
{
	m_month = NULL;
	m_string = AfxRegisterWndClass(0);
}

CmonthWnd::~CmonthWnd()
{
	if (m_month)
		delete m_month;
}


BEGIN_MESSAGE_MAP(CmonthWnd, CWnd)
	//{{AFX_MSG_MAP(CmonthWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CmonthWnd message handlers

int CmonthWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CmonthWnd::OnClose() 
{
	if (m_month)
	{
		if (IsWindow(m_month->GetSafeHwnd()))
			m_month->SendMessage(WM_CLOSE);
		m_month->Detach();
		delete m_month;
		m_month = NULL;
	}
	CWnd::OnClose();
}

LRESULT CmonthWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_HIDE);
	return 0;
}

BOOL CmonthWnd::Create(CWnd* parent, CRect rect, CfmEdit* edit)
{
	m_parent = parent;
	if (CWnd::CreateEx(0, m_string, NULL, WS_POPUP|WS_VISIBLE, rect, parent, 0))
	{
		m_month = new Cmonth;
		if (!m_month->Create(this, rect, edit))
		{
			delete m_month;
			m_month = NULL;
			return FALSE;
		}

		CRect	rc;

		if (m_month->GetMinReqRect(&rc))
		{
			int	cy = GetSystemMetrics(SM_CYSCREEN);
			rc.SetRect(rect.right-rc.Width(), rect.bottom, rect.right, rect.bottom+rc.Height());
			rc.InflateRect(2, 0);
			parent->ClientToScreen(&rc);
			if (rc.left < 0)
				rc.OffsetRect(-rc.left, 0);
			if (rc.bottom > cy)
				rc.OffsetRect(0, rc.top - rect.Height() - rc.bottom);
			MoveWindow(rc);
			m_month->MoveWindow(0, 0, rc.Width(), rc.Height());
		}
		return TRUE;
	}
	
	return FALSE;
}

void CmonthWnd::ShowMonthCal(CRect rect)
{
	CRect	rc;

	if (m_month && m_month->GetMinReqRect(&rc))
	{
		int	cy = GetSystemMetrics(SM_CYSCREEN);
		rc.SetRect(rect.right-rc.Width(), rect.bottom, rect.right, rect.bottom+rc.Height());
		rc.InflateRect(2, 0);
		m_parent->ClientToScreen(&rc);
		if (rc.left < 0)
			rc.OffsetRect(-rc.left, 0);
		if (rc.bottom > cy)
			rc.OffsetRect(0, rc.top - rect.Height() - rc.bottom);
		MoveWindow(rc);
		m_month->MoveWindow(0, 0, rc.Width(), rc.Height());
		ShowWindow(SW_SHOW);
		m_month->SetFocus();
		::SetCapture(m_month->GetSafeHwnd());
	}	
}