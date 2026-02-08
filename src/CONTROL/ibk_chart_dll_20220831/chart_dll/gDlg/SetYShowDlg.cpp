// SetYShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axisgdlg.h"
#include "SetYShowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetYShowDlg dialog


CSetYShowDlg::CSetYShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetYShowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetYShowDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_iRet	= -1;
}


void CSetYShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetYShowDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetYShowDlg, CDialog)
	//{{AFX_MSG_MAP(CSetYShowDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetYShowDlg message handlers

void CSetYShowDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (((CButton*)GetDlgItem(IDC_RADIO_RIGHT))->GetCheck())
		m_iRet = 1;
	else if (((CButton*)GetDlgItem(IDC_RADIO_LEFT))->GetCheck())
		m_iRet = 2;
	else if (((CButton*)GetDlgItem(IDC_RADIO_BOTH))->GetCheck())
		m_iRet = 3;
	else
		m_iRet = 0;
	
	CDialog::OnOK();
}

BOOL CSetYShowDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CButton*)GetDlgItem(IDC_RADIO_NONE))->SetCheck(true);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
