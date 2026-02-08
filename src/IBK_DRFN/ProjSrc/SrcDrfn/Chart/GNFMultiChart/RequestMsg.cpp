// RequestMsg.cpp : implementation file
//

#include "stdafx.h"
#include "RequestMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequestMsg dialog


CRequestMsg::CRequestMsg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CRequestMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequestMsg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRequestMsg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequestMsg)
	DDX_Control(pDX, IDC_STMESSAGE, m_staticMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequestMsg, CRscDlg)
	//{{AFX_MSG_MAP(CRequestMsg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequestMsg message handlers
void CRequestMsg::ShowMessage(LPCTSTR lpMessage,CWnd *pWnd /*= NULL*/)
{
	m_staticMessage.SetWindowText(lpMessage);
	long lLength = m_staticMessage.GetWindowTextLength();
	CRect rectParent;
	CRect rectMsgBar;
	long  lWidth = 10*lLength;		//	6*lLength;
	long lHeight = 30;				//	18;

	CRect rectWndFromOutside;
	if(pWnd)
	{	
		pWnd->GetWindowRect(rectWndFromOutside);
		rectMsgBar.left = rectWndFromOutside.left + ((rectWndFromOutside.right - rectWndFromOutside.left) / 2) - (lWidth / 2);
		rectMsgBar.top = rectWndFromOutside.top + ((rectWndFromOutside.bottom - rectWndFromOutside.top) / 2) - (lHeight / 2);
		rectMsgBar.right = rectMsgBar.left + lWidth;
		rectMsgBar.bottom = rectMsgBar.top + lHeight;

//		pWnd->GetWindowRect(rectWndFromOutside);
//		rectMsgBar.left = rectWndFromOutside.left;
//		rectMsgBar.top = rectWndFromOutside.top - lHeight;
//		rectMsgBar.right = rectWndFromOutside.left + lWidth;
//		rectMsgBar.bottom = rectWndFromOutside.top;
	}
	else
	{
		GetParent()->GetClientRect(rectParent);
		rectMsgBar.left = rectParent.Width()/2-lWidth/2;
		rectMsgBar.top = rectParent.Height()/2-lHeight/2;
		rectMsgBar.right = rectMsgBar.left + lWidth;
		rectMsgBar.bottom = rectMsgBar.top + lHeight;
		GetParent()->ClientToScreen(rectMsgBar);
	}
	m_staticMessage.MoveWindow(0,7,lWidth,lHeight);
	this->MoveWindow(rectMsgBar);
	this->ShowWindow(SW_SHOW);
	m_staticMessage.SetWindowText(lpMessage);
//	SetTimer(1,1000,NULL);
}

void CRequestMsg::HideMessage()
{
	this->ShowWindow(SW_HIDE);
	m_staticMessage.SetWindowText("");
}

void CRequestMsg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)
	{
		this->ShowWindow(SW_HIDE);
		m_staticMessage.SetWindowText("");
		KillTimer(1);
	}
	CRscDlg::OnTimer(nIDEvent);
}
