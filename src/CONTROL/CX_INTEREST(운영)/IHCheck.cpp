// IHCheck.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "IHCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIHCheck

CIHCheck::CIHCheck(CString szTip)
{
	m_szTip = szTip;
	m_pToolTip = nullptr;
}

CIHCheck::~CIHCheck()
{
}


BEGIN_MESSAGE_MAP(CIHCheck, CButton)
	//{{AFX_MSG_MAP(CIHCheck)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIHCheck message handlers

int CIHCheck::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_szTip.IsEmpty())
	{
		CRect	rect;
		GetClientRect(rect);
		m_pToolTip = std::make_unique<CToolTipCtrl>();
		m_pToolTip->Create(this);
		m_pToolTip->AddTool(this, "¹Ùº¸", rect, GetDlgCtrlID());
		m_pToolTip->Activate(TRUE);
	}
	return 0;
}

void CIHCheck::OnDestroy() 
{
	CButton::OnDestroy();
	
	if (m_pToolTip)
	{
		m_pToolTip->Activate(FALSE);
		m_pToolTip->DelTool(this);
		//m_pToolTip->RelayEvent(nullptr);
		m_pToolTip->DestroyWindow();
	}
}

LRESULT CIHCheck::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;

	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
	{
		if (m_pToolTip)
			m_pToolTip->RelayEvent(&msg);
	}
	return CButton::WindowProc(message, wParam, lParam);
}
