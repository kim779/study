// TooltipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TooltipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTooltipDlg dialog


CTooltipDlg::CTooltipDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CTooltipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTooltipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nLastCmdShow = NULL;
}


void CTooltipDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTooltipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTooltipDlg, CRscDlg)
	//{{AFX_MSG_MAP(CTooltipDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTooltipDlg message handlers

void CTooltipDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==100)
	{
		ShowWindow(SW_HIDE);		
	}
	CRscDlg::OnTimer(nIDEvent);
}

BOOL CTooltipDlg::ShowWindow(int nCmdShow)
{
	if(nCmdShow==SW_SHOW&&m_nLastCmdShow!=nCmdShow)
	{
		KillTimer(100);
		SetTimer(100,1000,NULL);
	}
	else if(nCmdShow==SW_HIDE)
	{
		KillTimer(100);
	}
	m_nLastCmdShow = nCmdShow;
	return CRscDlg::ShowWindow(nCmdShow);
}