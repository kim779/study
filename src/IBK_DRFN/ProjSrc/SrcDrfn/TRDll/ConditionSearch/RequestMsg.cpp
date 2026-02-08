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
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_font);
}

CRequestMsg::~CRequestMsg()
{
	m_font.DeleteObject();
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
	long  lWidth = 10*lLength;		//	6*lLength;
	long lHeight = 30;				//	18;

 	if(pWnd)
 	{
		CRect rectWndFromOutside;
		pWnd->GetWindowRect(rectWndFromOutside);
		m_staticMessage.SetFont(&m_font);
		m_staticMessage.MoveWindow(70,20,lWidth,lHeight);
		this->SetWindowPos(GetParent(), rectWndFromOutside.left + 150, rectWndFromOutside.bottom - 300, 
			rectWndFromOutside.right - 150, rectWndFromOutside.bottom + 50, SWP_NOSIZE);
		this->ShowWindow(SW_SHOW);
		m_staticMessage.SetWindowText(lpMessage);
	}
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

// LRESULT CRequestMsg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
// {
// 	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
// 	if( WM_IDLEUPDATECMDUI == message) return 0;
// 
// 	return CRscDlg::WindowProc(message, wParam, lParam);
// }
