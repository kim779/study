// MoveSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB202200.h"
#include "MoveSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMoveSetupDlg dialog


CMoveSetupDlg::CMoveSetupDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CMoveSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMoveSetupDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT

	m_bAlert  = FALSE;
}


void CMoveSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoveSetupDlg)
	DDX_Control(pDX, IDC_CHECK_ALERT, m_cbAlert);
	DDX_Radio(pDX, IDC_RADIO_DISP, m_nOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoveSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CMoveSetupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoveSetupDlg message handlers

BOOL CMoveSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cbAlert.SetCheck(m_bAlert);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMoveSetupDlg::OnOK() 
{
	UpdateData(TRUE);
	m_bAlert  = m_cbAlert.GetCheck();
	
	CDialog::OnOK();
}
