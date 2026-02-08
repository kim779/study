// AxMButtonCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AxMCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxMButtonCtrl

CAxMButtonCtrl::CAxMButtonCtrl()
{
	m_down = FALSE;
	m_messageHandler = NULL;
}

CAxMButtonCtrl::~CAxMButtonCtrl()
{
}

BEGIN_MESSAGE_MAP(CAxMButtonCtrl, CButton)
	//{{AFX_MSG_MAP(CAxMButtonCtrl)
	ON_MESSAGE(AXM_SETMSGHANDLER, OnSetMessageHandler)
	ON_MESSAGE(AXM_GETMSGHANDLER, OnGetMessageHandler)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMButtonCtrl message handlers

void CAxMButtonCtrl::SetDown(BOOL down)
{
	m_down = down;
}

BOOL CAxMButtonCtrl::GetDown()
{
	return m_down;
}

LRESULT CAxMButtonCtrl::OnSetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	SetMessageHandler((CWnd*)wParam);
	return 0L;
}

LRESULT CAxMButtonCtrl::OnGetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_messageHandler;
}

LRESULT CAxMButtonCtrl::Notify(UINT msg, WPARAM wParam, LPARAM lParam)
{
	NOTIFY(msg, wParam, lParam);
}

