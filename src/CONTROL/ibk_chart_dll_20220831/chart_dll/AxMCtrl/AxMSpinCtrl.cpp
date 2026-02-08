// AxMSpinCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AxMCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAxMSpinCtrl

CAxMSpinCtrl::CAxMSpinCtrl()
{
	m_messageHandler = NULL;
}

CAxMSpinCtrl::~CAxMSpinCtrl()
{
}

BEGIN_MESSAGE_MAP(CAxMSpinCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CAxMSpinCtrl)
	ON_MESSAGE(AXM_SETMSGHANDLER, OnSetMessageHandler)
	ON_MESSAGE(AXM_GETMSGHANDLER, OnGetMessageHandler)
	ON_NOTIFY_REFLECT_EX(UDN_DELTAPOS, OnDeltapos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMSpinCtrl message handlers

void CAxMSpinCtrl::SetMessageHandler(CWnd* wnd)
{
	m_messageHandler = wnd;
}

BOOL CAxMSpinCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pud = (NM_UPDOWN*)pNMHDR;

	Notify(WM_AXITEMCLICK, pud->iDelta, (LPARAM)this);

	*pResult = 0;
	return FALSE;
}

LRESULT CAxMSpinCtrl::Notify(UINT msg, WPARAM wParam, LPARAM lParam)
{
	NOTIFY(msg, wParam, lParam);
}

LRESULT CAxMSpinCtrl::OnSetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	SetMessageHandler((CWnd*)wParam);
	return 0L;
}

LRESULT CAxMSpinCtrl::OnGetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)GetMessageHandler();
}

