// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MacExec.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(Config* cfg, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	m_alias = _T("");
	m_fileName = _T("");
	m_ip = _T("");
	m_port = 0;
	m_regKey = _T("");
	m_password = _T("");
	m_userID = _T("");
	//}}AFX_DATA_INIT

	if (cfg)
	{
		m_alias = cfg->alias;
		m_fileName = cfg->fileName;
		m_ip = cfg->ip;
		m_port = cfg->port;
		m_regKey = cfg->regKey;
		m_userID = cfg->userID;
		m_password = cfg->password;
		m_addMode = FALSE;
	}
	else
	{
		m_addMode = TRUE;
	}
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Text(pDX, IDC_ALIAS, m_alias);
	DDX_Text(pDX, IDC_FILENAME, m_fileName);
	DDX_Text(pDX, IDC_IPADDRESS, m_ip);
	DDX_Text(pDX, IDC_PORTNO, m_port);
	DDX_Text(pDX, IDC_REGKEY, m_regKey);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Text(pDX, IDC_USERID, m_userID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (!m_addMode)
		UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::OnBrowse() 
{
	UpdateData();
	CFileDialog dlg(TRUE, "exe", "axis.exe", OFN_HIDEREADONLY, "axis.exe| (Axis Execute File)||");
	if (dlg.DoModal() == IDOK)
	{
		m_fileName =  dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CConfigDlg::OnOK() 
{
	UpdateData();
	
	if (m_alias.IsEmpty())
	{
		MessageBox("별칭이 필요합니다.", "오류", MB_OK | MB_ICONEXCLAMATION);
		GetDlgItem(IDC_ALIAS)->SetFocus();
		return;
	}

	if (m_fileName.IsEmpty())
	{
		MessageBox("실행파일이름이 반드시 필요합니다.", "오류", MB_OK | MB_ICONEXCLAMATION);
		GetDlgItem(IDC_BROWSE)->SetFocus();
		return;
	}

	CDialog::OnOK();
}
