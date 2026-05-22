// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "AxisChaser.h"
#include "chaser.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_COPYDATA()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(CHASERDISPLAY, OnChaserDisplay)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bStart = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString	title;
	CAxisChaserApp* app = (CAxisChaserApp*) AfxGetApp();
	title.Format("AxisChaser for %s", app->m_TargetKey);
	SetWindowText(title);

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	int *num, ii, jj= m_ToolBar.GetToolBarCtrl().GetButtonCount();
	num = new int[jj+1];
	memset(num,0,(jj+1)*sizeof(int));
	num[0] = jj;

	for (ii = 0; ii < jj; ii++)
	{
		UINT nID = m_ToolBar.GetItemID(ii);
		if(ID_SEPARATOR == nID)	num[ii+1]=1;
	}
	m_ToolBar.SetFullColorImage(IDR_TOOLBAR24, RGB(255, 0, 255), num);
	delete []num;

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);
	m_wndView.setTrace();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
LRESULT CMainFrame::OnChaserDisplay(WPARAM wParam, LPARAM lParam)
{
#if 0
	ShowWindow(SW_SHOW);
#else
	CAxisChaserApp* app = (CAxisChaserApp*) AfxGetApp();
	int x  = app->GetProfileInt(TRACESETTING, "win_x", 0);
	int y  = app->GetProfileInt(TRACESETTING, "win_y", 0);
	int cx = app->GetProfileInt(TRACESETTING, "win_cx", 0);
	int cy = app->GetProfileInt(TRACESETTING, "win_cy", 0);
	
	if (cx <= 300 || cy <= 100)
		ShowWindow(SW_SHOW);
	else	
		SetWindowPos(&wndTop, x, y, cx, cy, SWP_SHOWWINDOW);

	ShowWindow(SW_SHOW);
#endif
	m_bStart = true;

	return 0;
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	if (!m_bStart)	return TRUE;
	m_wndView.CopyData(pWnd, pCopyDataStruct);	
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrame::OnClose() 
{
	CRect	wRc;
	GetWindowRect(wRc);
	
	int	x  = wRc.left;
	int	y  = wRc.top;
	int	cx = wRc.Width();
	int	cy = wRc.Height();
	
	CAxisChaserApp* app = (CAxisChaserApp*) AfxGetApp();

	app->WriteProfileInt(TRACESETTING, "win_x", x);
	app->WriteProfileInt(TRACESETTING, "win_y", y);
	app->WriteProfileInt(TRACESETTING, "win_cx", cx);
	app->WriteProfileInt(TRACESETTING, "win_cy", cy);
	
	CFrameWnd::OnClose();
}
