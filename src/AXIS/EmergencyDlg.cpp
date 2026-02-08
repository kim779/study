// EmergencyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "EmergencyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmergencyDlg dialog


CEmergencyDlg::CEmergencyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEmergencyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEmergencyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_val = 0;
}


void CEmergencyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmergencyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmergencyDlg, CDialog)
	//{{AFX_MSG_MAP(CEmergencyDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmergencyDlg message handlers

void CEmergencyDlg::OnOk() 
{
	CButton*	pButton = (CButton*)GetDlgItem(IDC_CHK_PROGRAM);
	
	m_val = 0;
	if (pButton->GetCheck())
		m_val = 0x01;

	pButton = (CButton*)GetDlgItem(IDC_CHK_RESOURCE);
	if (pButton->GetCheck())
		m_val |= 0x02;

	CDialog::OnOK();
}
