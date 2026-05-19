// saveDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "saveDlg.h"
#include "afxdialogex.h"

#include "../h/mainvar.h"

// CSaveDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSaveDlg, CDialogEx)

CSaveDlg::CSaveDlg(CWnd* pParent, CString path)
	: CDialogEx(CSaveDlg::IDD, pParent)
{
	m_path = path;
}

CSaveDlg::~CSaveDlg()
{
}

void CSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PATH, m_path);
}


BEGIN_MESSAGE_MAP(CSaveDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BROWSER, &CSaveDlg::OnBrowser)
END_MESSAGE_MAP()


// CSaveDlg 메시지 처리기입니다.


BOOL CSaveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSaveDlg::OnOK()
{
	CString	root, path, tmps, maps;

	UpdateData(TRUE);
	root = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	path = m_path;
	root.MakeUpper();
	path.MakeUpper();
	tmps = root + _T("\\EXE\\TMP");
	maps = root + _T("\\MAP\\");

	if (!strncmp(tmps.operator LPCTSTR(), path.operator LPCTSTR(), tmps.GetLength()))
	{
		AfxMessageBox(_T("This folder is reserved in program.\n\nPlease select other folder."));
		GetDlgItem(IDCANCEL)->SetFocus();
		return;
	}

	if (!strncmp(maps.operator LPCTSTR(), path.operator LPCTSTR(), maps.GetLength()))
	{
		AfxMessageBox(_T("This folder is reserved as binary map folder in program.\n\nPlease select other folder."));
		GetDlgItem(IDCANCEL)->SetFocus();
		return;
	}

	if (::GetFileAttributes(m_path) & FILE_ATTRIBUTE_DIRECTORY)
		CDialogEx::OnOK();
	else
	{
		AfxMessageBox(_T("This folder is not exist.\n\nPlease select other folder."));
		GetDlgItem(IDCANCEL)->SetFocus();
	}
}

void CSaveDlg::OnBrowser()
{
	CFolderPickerDialog dlg(m_path, OFN_PATHMUSTEXIST);
	if (dlg.DoModal() == IDOK)
	{
		m_path = dlg.GetPathName();
		UpdateData(FALSE);
	}
}
