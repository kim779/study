// PrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axisgview.h"
#include "PrintDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintDlg dialog


CPrintDlg::CPrintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrintDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintDlg message handlers

BOOL CPrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_select = PRINT_CHART;
	((CButton *)GetDlgItem(IDC_RADIOCHART))->SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrintDlg::OnOK() 
{
	int chk = ((CButton *)GetDlgItem(IDC_RADIOWINDOW))->GetCheck();
	if (chk)	m_select = PRINT_WINDOW;
	
	CDialog::OnOK();
}
