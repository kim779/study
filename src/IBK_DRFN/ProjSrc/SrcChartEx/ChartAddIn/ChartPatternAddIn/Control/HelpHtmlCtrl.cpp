// HelpHtmlCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HelpHtmlCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpHtmlCtrl

IMPLEMENT_DYNCREATE(CHelpHtmlCtrl, CHtmlView)

CHelpHtmlCtrl::CHelpHtmlCtrl()
{
	//{{AFX_DATA_INIT(CHelpHtmlCtrl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CHelpHtmlCtrl::~CHelpHtmlCtrl()
{
}

void CHelpHtmlCtrl::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpHtmlCtrl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpHtmlCtrl, CHtmlView)
	//{{AFX_MSG_MAP(CHelpHtmlCtrl)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpHtmlCtrl diagnostics

/////////////////////////////////////////////////////////////////////////////
// CHelpHtmlCtrl message handlers


BOOL CHelpHtmlCtrl::CreateWnd(CRect rcWnd, CWnd *pParent, UINT nID)
{
	DWORD dwStyle = WS_CHILD | WS_VISIBLE ;
	LPCTSTR lpWndCls;
	lpWndCls = AfxRegisterWndClass(0);
	BOOL bRet = CHtmlView::Create(NULL, lpWndCls, dwStyle, rcWnd, pParent, nID);
	
	return bRet;
}

CHelpHtmlCtrl* CHelpHtmlCtrl::GetHtmlView()
{
	CHelpHtmlCtrl* pView = new CHelpHtmlCtrl;
	return pView;
}


void CHelpHtmlCtrl::OnDestroy() 
{
//	if (m_pBrowserApp) {
//		m_pBrowserApp->Release();
//		m_pBrowserApp = NULL;
//	}
	CWnd::OnDestroy();
}

int CHelpHtmlCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg)
{
	// bypass CView doc/frame stuff
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, msg);
}

void CHelpHtmlCtrl::OnDocumentComplete(LPCTSTR lpszURL) 
{
	// TODO: Add your specialized code here and/or call the base class
	COleVariant vaZoomFactor(1l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
	CHtmlView::OnDocumentComplete(lpszURL);
}

BOOL CHelpHtmlCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message)
	{
	case WM_KEYUP:
		{
			if((GetKeyState(VK_CONTROL) < 0) && pMsg->wParam == 67)
				ExecWB(OLECMDID_COPY, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL); 
			break;
		}
	}

	return CHtmlView::PreTranslateMessage(pMsg);
}

void CHelpHtmlCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CHtmlView::OnShowWindow(bShow, nStatus);
	
	ModifyStyle(WS_HSCROLL, 0);

	Invalidate();
}
