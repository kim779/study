// AccnAdminDLG.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "AccnAdminDLG.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccnAdminDLG dialog


CAccnAdminDLG::CAccnAdminDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CAccnAdminDLG::IDD, pParent)
{
	m_accnTreeDlg		= NULL;
	m_accnInterestDlg	= NULL;

	m_nTabIndex		= 0;
}


void CAccnAdminDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccnAdminDLG)
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccnAdminDLG, CDialog)
	//{{AFX_MSG_MAP(CAccnAdminDLG)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccnAdminDLG message handlers

BOOL CAccnAdminDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Tab.InsertItem(0, "ÀüÃ¼°èÁÂ");
	m_Tab.InsertItem(1, "°ü½É°èÁÂ");

	CRect	cRc;
	GetClientRect(cRc);

	m_Tab.SetWindowPos(NULL, cRc.left, cRc.top, cRc.Width(), 22, SWP_SHOWWINDOW);

	m_accnTreeDlg		= new CAccnTreeDLG;
	m_accnTreeDlg->Create(IDD_ACCN_TREE, this);
	m_accnTreeDlg->SetWindowPos(&wndTop, cRc.left, cRc.top + 23, cRc.Width(), cRc.Height() - 30, SWP_SHOWWINDOW);
	m_accnTreeDlg->ShowWindow(SW_HIDE);

	m_accnInterestDlg	= new CAccnInterestDLG;
	m_accnInterestDlg->Create(IDD_ACCN_INTEREST, this);
	m_accnInterestDlg->SetWindowPos(&wndTop, cRc.left, cRc.top + 23, cRc.Width(), cRc.Height() - 30, SWP_SHOWWINDOW);
	m_accnInterestDlg->ShowWindow(SW_HIDE);

	SetTab(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAccnAdminDLG::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetTab(m_Tab.GetCurSel());
	
	*pResult = 0;
}

void CAccnAdminDLG::SetTab(int index)
{
	switch(index)
	{
	case 0:
		m_accnInterestDlg->ShowWindow(SW_HIDE);
		m_accnTreeDlg->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_accnTreeDlg->ShowWindow(SW_HIDE);
		m_accnInterestDlg->ShowWindow(SW_SHOW);
		break;		
	}

	m_nTabIndex = index;
}

BOOL CAccnAdminDLG::SetTreeAccn(int vsn)
{
	m_accnTreeDlg->SetTreeAccn(vsn);
	m_accnInterestDlg->SetTreeAccn(vsn);

	return TRUE;
}

void CAccnAdminDLG::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (cx == 0 && cy == 0)
		return;

	if(IsWindowVisible())
	{
		RedrawCtrl(true);
		Invalidate();
	}
	
}

void CAccnAdminDLG::RedrawCtrl(bool bResize)
{
	CRect	cRc;
	GetClientRect(cRc);

	m_Tab.SetWindowPos(NULL, cRc.left, cRc.top, cRc.Width(), 22, SWP_SHOWWINDOW);

	m_accnTreeDlg->SetWindowPos(&wndTop, cRc.left, cRc.top + 23, cRc.Width(), cRc.Height() - 30, SWP_SHOWWINDOW);
	m_accnInterestDlg->SetWindowPos(&wndTop, cRc.left, cRc.top + 23, cRc.Width(), cRc.Height() - 30, SWP_SHOWWINDOW);

	SetTab(m_nTabIndex);
}

void CAccnAdminDLG::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
	CDialog::OnClose();
}

void CAccnAdminDLG::PostNcDestroy() 
{
	((CMainFrame *)AfxGetMainWnd())->m_AccnDLG = NULL;
	
	delete this;	
	
	CDialog::PostNcDestroy();
}
