// RootPathDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RootPathDlg.h"
#include "folderBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRootPathDlg dialog


CRootPathDlg::CRootPathDlg(CWnd* pParent, CString path)
	: CDialog(CRootPathDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRootPathDlg)
	//}}AFX_DATA_INIT
	m_path = path;
}


void CRootPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRootPathDlg)
	DDX_Text(pDX, IDC_PATH, m_path);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRootPathDlg, CDialog)
	//{{AFX_MSG_MAP(CRootPathDlg)
	ON_BN_CLICKED(IDC_BROWSER, OnBrowser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRootPathDlg message handlers

void CRootPathDlg::OnBrowser() 
{
	UpdateData(TRUE);
	CFolderBrowser folder(m_path);
	if (folder.DoModal() == IDOK)
	{
		m_path = folder.GetPathName();
		UpdateData(FALSE);
	}	
}

BOOL CRootPathDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRootPathDlg::OnOK() 
{
	UpdateData();
	if (CFolderBrowser::MakeSurePathExists(m_path) == 0)
		CDialog::OnOK();
}
