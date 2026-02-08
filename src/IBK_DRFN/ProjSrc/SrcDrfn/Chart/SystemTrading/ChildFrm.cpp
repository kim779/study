// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "SystemTrading.h"

#include "MainFrm.h"
#include "SystemTradingDoc.h"

#include "ChildFrm.h"
#include "../include_ST/DeclareDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_MDIACTIVATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	return m_wndSplitter.Create(this, 2, 2, CSize(30, 30), pContext);
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	
	CSystemTradingDoc* pDoc = (CSystemTradingDoc*)GetActiveDocument();
	((CMainFrame*)theApp.m_pMainWnd)->ChangeProperty(bActivate, pDoc->GetScriptName(), pDoc->GetParseFileType());
}

void CChildFrame::OnDestroy() 
{
	CSystemTradingDoc* pDoc = (CSystemTradingDoc*)GetActiveDocument();
	if(pDoc)
		((CMainFrame*)theApp.m_pMainWnd)->DeleteFunctionInStrategy(pDoc->GetScriptName());

	CMDIChildWnd::OnDestroy();
}
