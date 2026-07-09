// saveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "saveDlg.h"
#include "folderBrowser.h"

#include "../mainvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveDlg dialog

CSaveDlg::CSaveDlg(CWnd* pParent, CString path)
	: CDialog(CSaveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveDlg)
	//}}AFX_DATA_INIT
	m_path = path;
}


void CSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveDlg)
	DDX_Text(pDX, IDC_PATH, m_path);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveDlg)
	ON_BN_CLICKED(IDC_BROWSER, OnBrowser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveDlg message handlers

BOOL CSaveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSaveDlg::OnOK() 
{
	UpdateData(TRUE);

	CString sRoot = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), sPath = m_path, sTmpPath, sMapPath;
	sRoot.MakeUpper();
	sPath.MakeUpper();
	sTmpPath = sRoot + "\\EXE\\TMP";
	sMapPath = sRoot + "\\MAP\\";

	if (!strncmp(sTmpPath.operator LPCTSTR(), sPath.operator LPCTSTR(), sTmpPath.GetLength()))
	{
		AfxMessageBox("This folder is reserved in program.\n\nPlease select other folder.");
		GetDlgItem(IDCANCEL)->SetFocus();
		return;
	}

	if (!strncmp(sMapPath.operator LPCTSTR(), sPath.operator LPCTSTR(), sMapPath.GetLength()))
	{
		AfxMessageBox("This folder is reserved as binary map folder in program.\n\nPlease select other folder.");
		GetDlgItem(IDCANCEL)->SetFocus();
		return;
	}

	if (CFolderBrowser::MakeSurePathExists(m_path) == 0)
		CDialog::OnOK();
}

void CSaveDlg::OnBrowser() 
{
	UpdateData(TRUE);
	CFolderBrowser folder(m_path);
	if (folder.DoModal() == IDOK)
	{
		m_path = folder.GetPathName();
		UpdateData(FALSE);
	}
}
