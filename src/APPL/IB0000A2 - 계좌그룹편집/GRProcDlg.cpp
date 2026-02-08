// GRProcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000A2.h"
#include "GRProcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGRProcDlg dialog


CGRProcDlg::CGRProcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGRProcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGRProcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGRProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGRProcDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGRProcDlg, CDialog)
	//{{AFX_MSG_MAP(CGRProcDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGRProcDlg message handlers

void CGRProcDlg::OnOK() 
{
	GetDlgItem(IDC_GRNAME_EDIT)->GetWindowText(m_pParentWnd->m_sRGrName);
	
	CDialog::OnOK();
}

BOOL CGRProcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
