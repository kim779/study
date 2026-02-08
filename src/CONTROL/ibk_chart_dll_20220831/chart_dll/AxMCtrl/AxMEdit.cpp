// AxMEdit.cpp : implementation file
//

#include "stdafx.h"
#include "AxMCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxMEdit

CAxMEdit::CAxMEdit()
{
	m_messageHandler = NULL;
}

CAxMEdit::~CAxMEdit()
{
}


BEGIN_MESSAGE_MAP(CAxMEdit, CEdit)
	//{{AFX_MSG_MAP(CAxMEdit)
		ON_MESSAGE(AXM_SETMSGHANDLER, OnSetMessageHandler)
		ON_MESSAGE(AXM_GETMSGHANDLER, OnGetMessageHandler)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMEdit message handlers
LRESULT CAxMEdit::OnSetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	SetMessageHandler((CWnd*)wParam);
	return 0L;
}

LRESULT CAxMEdit::OnGetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_messageHandler;
}

LRESULT CAxMEdit::Notify(UINT msg, WPARAM wParam, LPARAM lParam)
{
	NOTIFY(msg, wParam, lParam);
}


void CAxMEdit::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	CString	str;
	GetWindowText(str);
	int	val = atoi(str);

	if (val < 2)
	{
		val = 2;
		SetWindowText("2");
	}
	if (val > 10)
	{
		val = 10;
		SetWindowText("10");
	}

	Notify(WM_AXITEMCLICK, val, (LPARAM)this);
}
