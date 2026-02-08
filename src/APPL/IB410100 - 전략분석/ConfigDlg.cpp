// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB410100.h"
#include "ConfigDlg.h"
#include "message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bMulti = true;

}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	LoadConfig();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CConfigDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(false);

	m_bMulti = true;

	SetConfig();
}

void CConfigDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(true);

	m_bMulti = false;

	SetConfig();
}

void CConfigDlg::SetConfig()
{
	CString strMulti;
	strMulti.Format("%d", m_bMulti ? 1:0);

	WritePrivateProfileString(MULTICODE, MULTICODE, strMulti, m_strPath);
}

void CConfigDlg::LoadConfig()
{
	int nConfig = 1;
	nConfig = GetPrivateProfileInt(MULTICODE, MULTICODE, 1, m_strPath);

	if(nConfig == 0)
	{
		OnRadio2();
	}
	else
	{
		OnRadio1();	//멀티종목창
	}
}

bool CConfigDlg::GetConfig()
{
	int nConfig = 1;
	nConfig = GetPrivateProfileInt(MULTICODE, MULTICODE, 1, m_strPath);

	if(nConfig == 0)
	{
		m_bMulti = false;
	}
	else
	{
		m_bMulti = true;
	}

	return m_bMulti;
}