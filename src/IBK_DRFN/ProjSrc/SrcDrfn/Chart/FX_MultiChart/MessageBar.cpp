// MessageBar.cpp : implementation file
//

#include "stdafx.h"
#include "MessageBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageBar dialog


CMessageBar::CMessageBar(CWnd* pParent /*=NULL*/)
	: CRscDlg(CMessageBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessageBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMessageBar::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageBar)
	DDX_Control(pDX, IDC_STMESSAGE, m_staticMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageBar, CRscDlg)
	//{{AFX_MSG_MAP(CMessageBar)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageBar message handlers
void CMessageBar::ShowMessage(LPCTSTR lpMessage,CWnd *pWnd /*= NULL*/)
{
	m_staticMessage.SetWindowText(lpMessage);
	long lLength = m_staticMessage.GetWindowTextLength();
	CRect rectParent;
	CRect rectMsgBar;
	long  lWidth = 6*lLength;
	long lHeight = 18;

	CRect rectWndFromOutside;
	if(pWnd)
	{		
		pWnd->GetWindowRect(rectWndFromOutside);
		rectMsgBar.left = rectWndFromOutside.left;
		rectMsgBar.top = rectWndFromOutside.top - lHeight;
		rectMsgBar.right = rectWndFromOutside.left + lWidth;
		rectMsgBar.bottom = rectWndFromOutside.top;
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
	m_staticMessage.MoveWindow(0,0,lWidth,lHeight);
	this->MoveWindow(rectMsgBar);
	this->ShowWindow(SW_SHOW);
	m_staticMessage.SetWindowText(lpMessage);
	SetTimer(1,1000,NULL);
}

void CMessageBar::OnTimer(UINT nIDEvent) 
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
